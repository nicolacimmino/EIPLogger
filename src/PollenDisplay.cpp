#include "PollenDisplay.h"

void PollenDisplay::onBClick()
{
    this->lastRefreshTime = 0;
}

void PollenDisplay::onBLongPress()
{
}

void PollenDisplay::loop()
{
    if (this->lastRefreshTime != 0 && millis() - this->lastRefreshTime < 60000)
    {
        return;
    }

    this->lastRefreshTime = millis();

    PowerManager::enterL0();

    // TODO: move wait to L0 enter
    uint8_t count = 0;
    while (!Peripherals::isWiFiConnected() && count < 20)
    {
        count++;
        delay(500);
    }

    if (!Peripherals::isWiFiConnected())
    {
        this->printValue("WiFi Error", 10, 0, EPD_WIDTH, 50, (GFXfont *)&MAIN_DISPLAY_MID_FONT);
        this->displayFramebuffer();
        return;
    }

    // Top bar
    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%s %02i-%02i-%02i %02i:%02i",
             "MON\0TUE\0WED\0THU\0FRI\0SAT\0SUN\0" + ((Peripherals::rtc->dayOfWeek() - 1) * 4),
             Peripherals::rtc->day(),
             Peripherals::rtc->month(),
             Peripherals::rtc->year(),
             Peripherals::rtc->hour(),
             Peripherals::rtc->minute());

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

        Serial.println(sizeof(responseObject) / (sizeof(*responseObject)));

        for (uint8_t ix = 0; ix < responseObject.length(); ix++)
        {
            Serial.println(responseObject[ix]["allergen"]["name"]);

            this->printLabelledValue((const char *)responseObject[ix]["allergen"]["name"],
                                     this->getAreaX(ix),
                                     this->getAreaY(ix),
                                     DIS_NONE,
                                     (int)responseObject[ix]["value"], "",
                                     (const char *)responseObject[ix]["trend"], 0,
                                     (const char *)responseObject[ix]["level"], 0);
        }
    }
    http.end();

    PowerManager::enterL1();

    this->displayFramebuffer();
}

uint16_t PollenDisplay::getAreaX(uint8_t areaNumber)
{
    return 10 + (areaNumber % 2) * 450;
}

uint16_t PollenDisplay::getAreaY(uint8_t areaNumber)
{
    return 50 + ((areaNumber / 2) * 150);
}