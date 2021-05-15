#include "Display.h"

void Display::onDataStoreChange()
{
    this->replotNeeded = true;
}

void Display::onDisplayAwaken()
{
    this->replotNeeded = true;
}

void Display::clearDisplay(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    Rect_t area = {
        .x = x,
        .y = y,
        .width = width,
        .height = height,
    };

    epd_poweron();
    epd_clear_area_cycles(area, 3, 40);
    epd_poweroff();
}

void Display::plotBatterLevel()
{
    // uint8_t batteryLevel = Status::getBatteryLevel();
    // Peripherals::oled->setCursor(80, 5);
    // Peripherals::oled->print(batteryLevel);
    // Peripherals::oled->print("%");

    // Peripherals::oled->drawBitmap(110, 5, batteryLogo[round(batteryLevel / 25.0)], BATTERY_LOGO_W, BATTERY_LOGO_H, SSD1306_WHITE);
}

void Display::onBLongPress()
{
    return;
}