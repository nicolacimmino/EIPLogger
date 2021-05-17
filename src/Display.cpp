#include "Display.h"

void Display::onDataStoreChange()
{
    this->replotNeeded = true;
}

void Display::onDisplayAwaken()
{
    this->replotNeeded = true;
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

void Display::printValue(char *buffer, int x, int y, int width, int height, const GFXfont *font)
{
    int x0 = x;
    int y0 = y;
    int x1;
    int y1;
    int w;
    int h;

    get_text_bounds(font, buffer, &x, &y, &x1, &y1, &w, &h, NULL);

    x0 = x0 + (width - w) / 2;

    write_mode(font, buffer, &x0, &y0, Peripherals::framebuffer, BLACK_ON_WHITE, NULL);
}

void Display::displayFramebuffer()
{
    epd_poweron();

    epd_clear_area_cycles(epd_full_screen(), 10, 50);
    //epd_push_pixels(epd_full_screen(), 500, 1);

    epd_draw_grayscale_image(epd_full_screen(), Peripherals::framebuffer);

    epd_poweroff();

    memset(Peripherals::framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
}

void Display::powerDown()
{
    this->printValue("Power down", 0, EPD_HEIGHT / 2, EPD_WIDTH, EPD_HEIGHT, (GFXfont *)&FiraSans);
    this->displayFramebuffer();
}
