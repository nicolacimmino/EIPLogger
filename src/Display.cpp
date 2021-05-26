#include "Display.h"

void Display::onBLongPress()
{
    return;
}

void Display::printValue(char *buffer, int x, int y, int width, int height, const GFXfont *font, bool center, bool skipFrameBuffer)
{
    int x0 = x;
    int y0 = y;
    int x1;
    int y1;
    int w;
    int h;

    if (skipFrameBuffer)
    {

        Rect_t area = {
            .x = x0,
            .y = y0 - height,
            .width = width,
            .height = height,
        };

        epd_clear_area(area);
    }

    get_text_bounds(font, buffer, &x, &y, &x1, &y1, &w, &h, NULL);

    if (center)
    {
        x0 = x0 + (width - w) / 2;
    }

    if (skipFrameBuffer)
    {
        epd_poweron();
    }

    write_mode(font, buffer, &x0, &y0, (skipFrameBuffer ? NULL : Peripherals::framebuffer), BLACK_ON_WHITE, NULL);

    if (skipFrameBuffer)
    {
        epd_poweroff();
    }
}

void Display::displayFramebuffer()
{
    DIAGNOSTIC("DIS,DISPLAY_FRAMEBUFFER");

    epd_poweron();

    epd_clear();

    epd_draw_grayscale_image(epd_full_screen(), Peripherals::framebuffer);

    epd_poweroff();

    memset(Peripherals::framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
}

void Display::powerDown()
{
    this->printValue("Power down", 0, EPD_HEIGHT / 2, EPD_WIDTH, EPD_HEIGHT, (GFXfont *)&BIG_POWER_DOWN_SCREEN_FONT, true, true);
}
