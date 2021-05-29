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
    this->printValue(Peripherals::buffer, 0, 0, EPD_WIDTH, 50, (GFXfont *)&MAIN_DISPLAY_SMALL_FONT);
    epd_draw_hline(0, 50, EPD_WIDTH, 0, Peripherals::framebuffer);

    this->printVHLValue("Temperature", this->getAreaX(0), this->getAreaY(0), DIS_NONE, Peripherals::sht2x->temperature, "C", 21.3, 29.6);
    this->printVHLValue("Humidity", this->getAreaX(2), this->getAreaY(2), DIS_NO_DECIMAL, Peripherals::sht2x->humidity, "%RH", 45, 85);

    this->printVHLValue("CO2", this->getAreaX(1), this->getAreaY(1), DIS_NO_DECIMAL | DIS_LARGE_VALUE, Status::co2, "ppm", 450, 6700);
    this->printVHLValue("TVOC", this->getAreaX(3), this->getAreaY(3), DIS_NO_DECIMAL | DIS_LARGE_VALUE, Status::tvoc, "ppb", 100, 640);
    this->printVHLValue("Barometer", this->getAreaX(5), this->getAreaY(5), DIS_NO_DECIMAL | DIS_LARGE_VALUE, Status::barometricPressure, "HPa", 940, 1007);

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