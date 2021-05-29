#include "Display.h"

void Display::onBLongPress()
{
    return;
}

void Display::printValue(char *buffer, int x, int y, int width, int height, const GFXfont *font, bool center, bool skipFrameBuffer)
{
    y = y + height;

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

    writeln(font, buffer, &x0, &y0, (skipFrameBuffer ? NULL : Peripherals::framebuffer));

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

void Display::printVHLValue(uint16_t x, uint16_t y, uint8_t options, float value, char *unit, float low, float high)
{
    uint16_t midSectionOffset = 115;
    if (options & DIS_LARGE_VALUE)
    {
        midSectionOffset = 230;
    }

    if (value != NO_VALUE)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%0.0f", value);
    }
    else
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "---");
    }
    this->printValue(Peripherals::buffer, x, y, 215, 110, (GFXfont *)&MAIN_DISPLAY_LARGE_FONT, false, false);

    if (!options & DIS_NO_DECIMAL && value != NO_VALUE)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, ".%d", (uint16_t)(10 * (value - floor(value))));
        this->printValue(Peripherals::buffer, x + 115, y + 50, 105, 55, (GFXfont *)&MAIN_DISPLAY_MID_FONT, false, false);
    }

    this->printValue(unit, x + midSectionOffset, y + 6, 105, 55, (GFXfont *)&MAIN_DISPLAY_MID_FONT, false, false);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "H:%0.0f" : "H:%0.1f", high);
    this->printValue(Peripherals::buffer, x + midSectionOffset + 95, y + 6, 215, 55, (GFXfont *)&MAIN_DISPLAY_MID_FONT, false, false);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "L:%0.0f" : "L:%0.1f", low);
    this->printValue(Peripherals::buffer, x + midSectionOffset + 95, y + 50, 215, 55, (GFXfont *)&MAIN_DISPLAY_MID_FONT, false, false);
}
