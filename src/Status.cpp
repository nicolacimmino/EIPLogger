#include "Status.h"

long Status::batteryVoltage = 0;
bool Status::abortLoopRequested = false;
bool Status::timeSyncRequsted = true;
bool Status::locked = false;
MinMaxVal<float> *Status::temperature = new MinMaxVal<float>();
MinMaxVal<int> *Status::co2 = new MinMaxVal<int>();
MinMaxVal<int> *Status::tvoc = new MinMaxVal<int>();
MinMaxVal<int> *Status::humidity = new MinMaxVal<int>();
MinMaxVal<int> *Status::pm1 = new MinMaxVal<int>();
MinMaxVal<int> *Status::pm2p5 = new MinMaxVal<int>();
MinMaxVal<int> *Status::pm4 = new MinMaxVal<int>();
MinMaxVal<int> *Status::pm10 = new MinMaxVal<int>();

void Status::setup()
{
}

void Status::loop()
{
    DataLog::instance()->loop();
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
    return max(max(max(Status::getCO2QI(), Status::getTVOCQI()), Status::getClimateQI()), Status::getPMAQI());
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

time_t Status::getUnixTime()
{
    return Utilities::calculateUnixTime(
        Peripherals::rtc->year(),
        Peripherals::rtc->month(),
        Peripherals::rtc->day(),
        Peripherals::rtc->hour(),
        Peripherals::rtc->minute(),
        Peripherals::rtc->second(),
        Status::isDST());
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

uint8_t Status::getPMAQI()
{
    uint8_t pm2p5Index = 1;
    if (Status::pm2p5->get() > 250)
    {
        pm2p5Index = 6;
    }
    else if (Status::pm2p5->get() > 150)
    {
        pm2p5Index = 5;
    }
    else if (Status::pm2p5->get() > 55)
    {
        pm2p5Index = 4;
    }
    else if (Status::pm2p5->get() > 35)
    {
        pm2p5Index = 3;
    }
    else if (Status::pm2p5->get() > 12)
    {
        pm2p5Index = 2;
    }

    uint8_t pm10Index = 1;
    if (Status::pm10->get() > 425)
    {
        pm10Index = 6;
    }
    else if (Status::pm10->get() > 355)
    {
        pm10Index = 5;
    }
    else if (Status::pm10->get() > 255)
    {
        pm10Index = 4;
    }
    else if (Status::pm10->get() > 155)
    {
        pm10Index = 3;
    }
    else if (Status::pm10->get() > 55)
    {
        pm10Index = 2;
    }

    return max(pm10Index, pm2p5Index);
}