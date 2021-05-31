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
    if (this->lastRefreshTime != 0 && millis() - this->lastRefreshTime < (60 *60000))
    {
        return;
    }

    // Top bar
    // TODO: move to common base function with optional header.
    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%s %02i-%02i-%02i %02i:%02i     Fetching Data....",
             "MON\0TUE\0WED\0THU\0FRI\0SAT\0SUN\0" + ((Peripherals::rtc->dayOfWeek() - 1) * 4),
             Peripherals::rtc->day(),
             Peripherals::rtc->month(),
             Peripherals::rtc->year(),
             Peripherals::rtc->hour(),
             Peripherals::rtc->minute());
    this->printValue(Peripherals::buffer, 10, 0, EPD_WIDTH, 50, (GFXfont *)&MAIN_DISPLAY_MID_FONT);
    this->displayFramebuffer();

    this->lastRefreshTime = millis();

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
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%s %02i-%02i-%02i %02i:%02i     Failed.",
                 "MON\0TUE\0WED\0THU\0FRI\0SAT\0SUN\0" + ((Peripherals::rtc->dayOfWeek() - 1) * 4),
                 Peripherals::rtc->day(),
                 Peripherals::rtc->month(),
                 Peripherals::rtc->year(),
                 Peripherals::rtc->hour(),
                 Peripherals::rtc->minute());
        this->printValue(Peripherals::buffer, 10, 0, EPD_WIDTH, 50, (GFXfont *)&MAIN_DISPLAY_MID_FONT);
        this->displayFramebuffer();

        return;
    }

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%s %02i-%02i-%02i %02i:%02i",
             "MON\0TUE\0WED\0THU\0FRI\0SAT\0SUN\0" + ((Peripherals::rtc->dayOfWeek() - 1) * 4),
             Peripherals::rtc->day(),
             Peripherals::rtc->month(),
             Peripherals::rtc->year(),
             Peripherals::rtc->hour(),
             Peripherals::rtc->minute());
    this->printValue(Peripherals::buffer, 10, 0, EPD_WIDTH, 50, (GFXfont *)&MAIN_DISPLAY_MID_FONT);

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

            this->printPollen((int)responseObject[ix]["allergen"]["id"],
                              this->getAreaX(ix),
                              this->getAreaY(ix),
                              (int)responseObject[ix]["value"],
                              (const char *)responseObject[ix]["trend"]);
        }
    }
    http.end();

    PowerManager::enterL1();

    this->displayFramebuffer();
}

void PollenDisplay::printPollen(int pollenId, uint16_t x, uint16_t y, int value, const char *trend)
{
    uint16_t midSectionOffset = 115;

    if (pollenId == 20)
    {
        this->printValue("Cladosporium", x, y + 20, midSectionOffset + 310, 40, (GFXfont *)&MAIN_DISPLAY_LABEL_FONT);
    }
    else if (pollenId == 23)
    {
        this->printValue("Alternaria", x, y + 20, midSectionOffset + 310, 40, (GFXfont *)&MAIN_DISPLAY_LABEL_FONT);
    }
    else if (pollenId == 29)
    {
        this->printValue("Oak", x, y + 20, midSectionOffset + 310, 40, (GFXfont *)&MAIN_DISPLAY_LABEL_FONT);
    }
    else if (pollenId == 31)
    {
        this->printValue("Grass", x, y + 20, midSectionOffset + 310, 40, (GFXfont *)&MAIN_DISPLAY_LABEL_FONT);
    }
    else if (pollenId == 34)
    {
        this->printValue("Sorrel", x, y + 20, midSectionOffset + 310, 40, (GFXfont *)&MAIN_DISPLAY_LABEL_FONT);
    }
    else if (pollenId == 32)
    {
        this->printValue("Pine", x, y + 20, midSectionOffset + 310, 40, (GFXfont *)&MAIN_DISPLAY_LABEL_FONT);
    }
    else if (pollenId == 33)
    {
        this->printValue("Platan", x, y + 20, midSectionOffset + 310, 40, (GFXfont *)&MAIN_DISPLAY_LABEL_FONT);
    }
    else if (pollenId == 36)
    {
        this->printValue("Plantago", x, y + 20, midSectionOffset + 310, 40, (GFXfont *)&MAIN_DISPLAY_LABEL_FONT);
    }
    else
    {
        this->printValue("???", x, y + 20, midSectionOffset + 310, 40, (GFXfont *)&MAIN_DISPLAY_LABEL_FONT);
    }

    if (strcmp("Bez zmian", trend) == 0)
    {
        this->showIcon(x + midSectionOffset, y + 80, trend_eq_width, trend_eq_height, (uint8_t *)trend_eq_data);
    }
    else if (strcmp("Wzrost", trend) == 0)
    {
        this->showIcon(x + midSectionOffset, y + 80, trend_u_width, trend_u_height, (uint8_t *)trend_u_data);
    }
    else if (strcmp("Silny wzrost", trend) == 0)
    {
        this->showIcon(x + midSectionOffset, y + 80, trend_uu_width, trend_uu_height, (uint8_t *)trend_uu_data);
    }
    else if (strcmp("Spadek", trend) == 0)
    {
        this->showIcon(x + midSectionOffset, y + 80, trend_d_width, trend_d_height, (uint8_t *)trend_d_data);
    }
    else if (strcmp("Silny spadek", trend) == 0)
    {
        this->showIcon(x + midSectionOffset, y + 80, trend_dd_width, trend_dd_height, (uint8_t *)trend_dd_data);
    }
    else if (strcmp("Koniec sezonu", trend) == 0)
    {
        this->showIcon(x + midSectionOffset, y + 80, trend_end_width, trend_end_height, (uint8_t *)trend_end_data);
    }

    //this->showIcon(x + midSectionOffset, y + 80, trend_uu_width, trend_uu_height, (uint8_t *)trend_uu_data);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%d", value);
    this->printValue(Peripherals::buffer, x, y + 50, 110, 110, (GFXfont *)&MAIN_DISPLAY_LARGE_FONT, DIS_RIGHT);

    // this->printValue(trend, x + midSectionOffset + 95, y + 80, 215, 55, (GFXfont *)&MAIN_DISPLAY_LABEL_FONT);
}

uint16_t PollenDisplay::getAreaX(uint8_t areaNumber)
{
    return 105 + (areaNumber % 3) * 300;
}

uint16_t PollenDisplay::getAreaY(uint8_t areaNumber)
{
    return 50 + ((areaNumber / 3) * 150);
}