#include "Status.h"

unsigned long Status::lastPollenSync = 0;
long Status::batteryVoltage = 0;
bool Status::abortLoopRequested = false;
bool Status::timeSyncRequsted = true;
int Status::thunderStrikes = 0;
int Status::thunderDistance = 0;
int Status::thunderEnergy = 0;
int Status::thunderInterferers = 0;
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
        Status::syncPollen();
        Status::lastPollenSync = millis();
    }
}

void Status::syncPollen()
{
    PowerManager::enterL0();

    // TODO: move wait to L0 enter
    uint8_t count = 0;
    while (!Peripherals::isWiFiConnected() && count < 40)
    {
        count++;
        delay(500);
    }

    if (!Peripherals::isWiFiConnected())
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
            Serial.println(responseObject[ix]["allergen"]["name"]);

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