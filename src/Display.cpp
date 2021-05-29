#include "Display.h"

void Display::onBLongPress()
{
    return;
}

void Display::printValue(char *buffer, int x, int y, int width, int height, const GFXfont *font, uint8_t options)
{
    y = y + height;

    int x0 = x;
    int y0 = y;
    int x1;
    int y1;
    int w;
    int h;

    if (options & DIS_DIRECT_PRINT)
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

    if (options & DIS_CENTER)
    {
        x0 += (width - w) / 2;
    }
    else if (options & DIS_RIGHT)
    {
        x0 += (width - w);
    }

    if (options & DIS_DIRECT_PRINT)
    {
        epd_poweron();
        writeln(font, buffer, &x0, &y0, NULL);
        epd_poweroff();
    }
    else
    {
        writeln(font, buffer, &x0, &y0, Peripherals::framebuffer);
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
    this->printValue("Power down", 0, EPD_HEIGHT / 2, EPD_WIDTH, EPD_HEIGHT, (GFXfont *)&BIG_POWER_DOWN_SCREEN_FONT, DIS_CENTER | DIS_DIRECT_PRINT);
}

void Display::printVHLValue(char *label, uint16_t x, uint16_t y, uint8_t options, float value, char *unit, float low, float high)
{
    uint16_t midSectionOffset = 115;
    if (options & DIS_LARGE_VALUE)
    {
        midSectionOffset = 230;
    }

    this->printValue(label, x, y + 20, midSectionOffset + 310, 40, (GFXfont *)&MAIN_DISPLAY_LABEL_FONT);

    if (value != NO_VALUE)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%0.0f", value);
    }
    else
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "---");
    }
    this->printValue(Peripherals::buffer, x, y + 50, (options & DIS_LARGE_VALUE) ? 220 : 110, 110, (GFXfont *)&MAIN_DISPLAY_LARGE_FONT, DIS_RIGHT);

    if (!options & DIS_NO_DECIMAL && value != NO_VALUE)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, ".%d", (uint16_t)(10 * (value - floor(value))));
        this->printValue(Peripherals::buffer, x + 115, y + 100, 105, 55, (GFXfont *)&MAIN_DISPLAY_MID_FONT);
    }

    this->printValue(unit, x + midSectionOffset, y + 56, 105, 55, (GFXfont *)&MAIN_DISPLAY_MID_FONT);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "H:%0.0f" : "H:%0.1f", high);
    this->printValue(Peripherals::buffer, x + midSectionOffset + 95, y + 56, 215, 55, (GFXfont *)&MAIN_DISPLAY_MID_FONT);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "L:%0.0f" : "L:%0.1f", low);
    this->printValue(Peripherals::buffer, x + midSectionOffset + 95, y + 100, 215, 55, (GFXfont *)&MAIN_DISPLAY_MID_FONT);
}

void Display::showIcon(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *data)
{
    Rect_t area = {
        .x = x,
        .y = y,
        .width = width,
        .height = height,
    };

    epd_copy_to_framebuffer(area, data, Peripherals::framebuffer);
}