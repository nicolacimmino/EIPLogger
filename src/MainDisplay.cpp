#include "MainDisplay.h"

void MainDisplay::onBClick()
{
    this->lastRefreshTime = 0;
}

void MainDisplay::onBLongPress()
{
}

void MainDisplay::loop()
{
    if (this->lastRefreshTime != 0 && millis() - this->lastRefreshTime < 60000)
    {
        return;
    }

    this->lastRefreshTime = millis();

    // Top bar
    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%02i-%02i-%02i %02i:%02i", Peripherals::rtc->day(), Peripherals::rtc->month(), Peripherals::rtc->year(), Peripherals::rtc->hour(), Peripherals::rtc->minute());
    this->printValue(Peripherals::buffer, 10, 0, EPD_WIDTH, 50, (GFXfont *)&MAIN_DISPLAY_MID_FONT);
    this->showIcon(870, 20, battery_100_width, battery_100_height, (uint8_t *)battery_100_data);
    this->showIcon(800, 15, wifi_width, wifi_height, (uint8_t *)wifi_data);

    this->printVHLValue("Temperature", this->getAreaX(0), this->getAreaY(0), DIS_NONE, Status::temperature, "C", 21.3, 29.6);
    this->printVHLValue("Humidity", this->getAreaX(2), this->getAreaY(2), DIS_NO_DECIMAL, Status::humidity, "%RH", 45, 85);

    this->printVHLValue("CO2", this->getAreaX(1), this->getAreaY(1), DIS_NO_DECIMAL | DIS_LARGE_VALUE, Status::co2, "ppm", 450, 6700);
    this->printVHLValue("TVOC", this->getAreaX(3), this->getAreaY(3), DIS_NO_DECIMAL | DIS_LARGE_VALUE, Status::tvoc, "ppb", 100, 640);
    this->printVHLValue("Barometer", this->getAreaX(5), this->getAreaY(5), DIS_NO_DECIMAL | DIS_LARGE_VALUE, Status::barometricPressure, "HPa", 940, 1007);

    this->showIcon(this->getAreaX(4), this->getAreaY(4) + 50, thunder_width, thunder_height, (uint8_t *)thunder_data);

    epd_draw_hline(0, 70, EPD_WIDTH, 0, Peripherals::framebuffer);
    epd_draw_hline(0, 220, EPD_WIDTH, 0, Peripherals::framebuffer);
    epd_draw_hline(0, 370, EPD_WIDTH, 0, Peripherals::framebuffer);
    epd_draw_vline(430, 70, EPD_HEIGHT - 50, 0, Peripherals::framebuffer);

    this->displayFramebuffer();
}

uint16_t MainDisplay::getAreaX(uint8_t areaNumber)
{
    return 10 + (areaNumber % 2) * 450;
}

uint16_t MainDisplay::getAreaY(uint8_t areaNumber)
{
    return 50 + ((areaNumber / 2) * 150);
}