#include "Status.h"

unsigned long Status::lastPollenSync = 0;
long Status::batteryVoltage = 0;
bool Status::abortLoopRequested = false;
bool Status::timeSyncRequsted = true;
int Status::thunderStrikes = 0;
int Status::thunderDistance = 0;
int Status::thunderEnergy = 0;
int Status::thunderInterferers = 0;
bool Status::locked = false;
MinMaxVal<float> *Status::temperature = new MinMaxVal<float>();
MinMaxVal<int> *Status::barometricPressure = new MinMaxVal<int>();
MinMaxVal<int> *Status::co2 = new MinMaxVal<int>();
MinMaxVal<int> *Status::tvoc = new MinMaxVal<int>();
MinMaxVal<int> *Status::humidity = new MinMaxVal<int>();
PollenCount Status::pollenCounts[MAX_POLLEN_COUNTS] = {};

void Status::setup()
{
    for (uint8_t ix = 0; ix < MAX_POLLEN_COUNTS; ix++)
    {
        Status::pollenCounts[ix].type = NULL;
    }
}

void Status::loop()
{
    if (Status::lastPollenSync == 0 || millis() - Status::lastPollenSync > 24 * 60 * 60000)
    {
        //Status::syncPollen();
        Status::lastPollenSync = millis();
    }
}

void Status::readThunderEvent()
{
    uint8_t intVal = Peripherals::lightning->readInterruptReg();

    if (intVal)
    {
        if (intVal == DISTURBER_INT)
        {
            DIAGNOSTIC("THU,interf");
            Status::thunderInterferers++;
        }
        else if (intVal == LIGHTNING_INT)
        {
            DIAGNOSTIC("THU,strike");
            Status::thunderStrikes++;
            Status::thunderEnergy = Peripherals::lightning->lightningEnergy();
            Status::thunderDistance = Peripherals::lightning->distanceToStorm();
        }

        while (Peripherals::lightning->readInterruptReg())
        {
            delay(1);
        }
    }
}
void Status::syncPollen()
{
    if (!PowerManager::enterL0())
    {
        return;
    }

    HTTPClient http;
    http.setConnectTimeout(3000);
    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "https://api.zadnegoale.pl/dusts/public/date/%d-%d-20%d/region/3",
             Peripherals::rtc->day(),
             Peripherals::rtc->month(),
             Peripherals::rtc->year());

    http.begin(Peripherals::buffer);
    int httpResponseCode = http.GET();
    if (httpResponseCode == 200)
    {
        String payload = http.getString();
        JSONVar responseObject = JSON.parse(payload);

        for (uint8_t ix = 0; ix < responseObject.length(); ix++)
        {
            Status::pollenCounts[ix].count = (uint8_t)(int)responseObject[ix]["value"];

            Status::pollenCounts[ix].type = (int)responseObject[ix]["allergen"]["id"];

            const char *trend = (const char *)responseObject[ix]["trend"];

            if (strcmp("Bez zmian", trend) == 0)
            {
                Status::pollenCounts[ix].trend = POLLEN_TREND_EQ;
            }
            else if (strcmp("Wzrost", trend) == 0)
            {
                Status::pollenCounts[ix].trend = POLLEN_TREND_U;
            }
            else if (strcmp("Silny wzrost", trend) == 0)
            {
                Status::pollenCounts[ix].trend = POLLEN_TREND_UU;
            }
            else if (strcmp("Spadek", trend) == 0)
            {
                Status::pollenCounts[ix].trend = POLLEN_TREND_D;
            }
            else if (strcmp("Silny spadek", trend) == 0)
            {
                Status::pollenCounts[ix].trend = POLLEN_TREND_DD;
            }
            else if (strcmp("Koniec sezonu", trend) == 0)
            {
                Status::pollenCounts[ix].trend = POLLEN_TREND_END;
            }
        }
        http.end();

        PowerManager::enterL1();
    }
}

/**
 * See https://playground.arduino.cc/Code/AvailableMemory/
 **/
int Status::getFreeRamBytes()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

uint8_t Status::getIAQI()
{
    return max(max(Status::getCO2QI(), Status::getTVOCQI()), Status::getClimateQI());
}

Time Status::getSunrise()
{
    // Based on this very useful article:
    // https://arduinodiy.wordpress.com/2017/03/07/calculating-sunrise-and-sunset-on-arduino-or-other-microcontroller/
    //
    // These values are for Warsaw.
    // ESR: Earliest sunrise 4:14 => 254 - 60 => 194 (minutes past midnight, ignoring DST)
    // LSR: Latest sunrise 7:45 => 465 (minutes past midnght)
    // ASR: Average sunrise: (ESR + LSR) / 2 => 329
    // DSR: Delta: (LSR - ESR) => 271

    // time = ASR + 0.5*DSR * cos((doy+8)/58.09) + DST
    uint16_t sunriseMinutes = (uint16_t)(329 + 135.0 * cos((Status::getDayOfYear() + 8) / 58.09)) + (Status::isDST() ? 60 : 0);

    Time sunriseTime;
    sunriseTime.h = (uint8_t)floor(sunriseMinutes / 60.0);
    sunriseTime.m = (uint8_t)(sunriseMinutes - (sunriseTime.h * 60));

    return sunriseTime;
}

Time Status::getSunset()
{
    // Based on this very useful article:
    // https://arduinodiy.wordpress.com/2017/03/07/calculating-sunrise-and-sunset-on-arduino-or-other-microcontroller/
    //
    // These values are for Warsaw.
    // ESS: Earliest sunset 15:24 => 924 (minutes past midnight)
    // LSS: Latest sunset 21:01 => 1201 (minutes past midnght, ignoring DST)
    // ASS: Average set: (ESS + LSS) / 2 => 1062
    // DSS: Delta: (LSS - ESS) => 277

    // time = ASS - 0.5*DSS * cos((doy+8)/58.09) + DST
    uint16_t sunsetMinutes = (uint16_t)(1062 - 138 * cos((Status::getDayOfYear() + 8) / 58.09)) + (Status::isDST() ? 60 : 0);

    Time sunsetTime;
    sunsetTime.h = (uint8_t)floor(sunsetMinutes / 60.0);
    sunsetTime.m = (uint8_t)(sunsetMinutes - (sunsetTime.h * 60));

    return sunsetTime;
}

// TODO: This still need fixing to consider the hours between midnight and 2/3 AM of the switch day.
bool Status::isDST()
{
    // The general gist is from this library, just simplified:
    // https://github.com/vancegroup-mirrors/avr-libc/blob/master/avr-libc/include/util/eu_dst.h

    uint8_t month = Status::getMonth();
    uint8_t day = Status::getDay();
    uint8_t dow = Status::getDayOfWeek() - 1;

    if (month != 3 && month != 10)
    {
        return (month > 3) && (month < 10);
    }

    uint8_t firstSunday = ((day - 1) - dow + 7) % 7;
    uint8_t numberOfSundays = (31 - firstSunday) / 7;
    uint8_t lastSunday = firstSunday + (7 * numberOfSundays);

    if (day < lastSunday)
    {
        return (month == 10);
    }

    return (month == 3);
}

uint16_t Status::getDayOfYear()
{
    return (uint16_t)((Peripherals::rtc->month() - 1) * 30.5) + Peripherals::rtc->day();
}

uint16_t Status::getHour()
{
    return Peripherals::rtc->hour();
}

uint16_t Status::getMinute()
{
    return Peripherals::rtc->minute();
}

uint16_t Status::getDay()
{
    return Peripherals::rtc->day();
}

uint16_t Status::getMonth()
{
    return Peripherals::rtc->month();
}

uint16_t Status::getYear()
{
    return Peripherals::rtc->year();
}

uint16_t Status::getDayOfWeek()
{
    return Peripherals::rtc->dayOfWeek();
}

uint8_t Status::getCO2QI()
{
    uint8_t co2I = 1;
    if (Status::co2->get() > 5000)
    {
        co2I = 6;
    }
    else if (Status::co2->get() > 2000)
    {
        co2I = 5;
    }
    else if (Status::co2->get() > 1500)
    {
        co2I = 4;
    }
    else if (Status::co2->get() > 100)
    {
        co2I = 3;
    }
    else if (Status::co2->get() > 400)
    {
        co2I = 2;
    }

    return co2I;
}

uint8_t Status::getTVOCQI()
{
    uint8_t tvocI = 1;
    if (Status::tvoc->get() > 5000)
    {
        tvocI = 6;
    }
    else if (Status::tvoc->get() > 1500)
    {
        tvocI = 5;
    }
    else if (Status::tvoc->get() > 500)
    {
        tvocI = 4;
    }
    else if (Status::tvoc->get() > 150)
    {
        tvocI = 3;
    }
    else if (Status::tvoc->get() > 50)
    {
        tvocI = 2;
    }

    return tvocI;
}

uint8_t Status::getClimateQI()
{
    uint8_t humBin = min(max(1, (int)floor(Status::humidity->get() / 10.0)), 9) - 1; // 0%-100% => 0 - 8
    uint8_t tempBin = min(max(16, (int)Status::temperature->get()), 28) - 16;        // -40-+40 => 16-28 => 0-12
    uint8_t lookup[9][13] = {
        {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
        {6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6},
        {6, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 5, 6},
        {6, 5, 5, 4, 3, 3, 3, 2, 2, 3, 4, 5, 6},
        {5, 5, 4, 3, 2, 1, 1, 1, 2, 3, 4, 5, 6},
        {5, 4, 3, 2, 1, 1, 1, 2, 2, 3, 4, 5, 6},
        {5, 4, 3, 2, 1, 1, 1, 2, 3, 4, 5, 5, 6},
        {5, 4, 2, 2, 2, 2, 2, 3, 4, 5, 5, 5, 6},
        {6, 5, 4, 3, 3, 3, 3, 4, 5, 5, 6, 6, 6}};

    return lookup[humBin][tempBin];
}