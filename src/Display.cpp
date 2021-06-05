#include "Display.h"

void Display::onBLongPress()
{
    return;
}

void Display::setWiFiIcon(bool visible)
{
    this->showIcon(800, 15, WIFI_WIDTH, WIFI_HEIGHT, visible ? (uint8_t *)wifi_data : NULL, DIS_DIRECT_PRINT);
}

void Display::printValue(const char *buffer, int x, int y, int width, int height, const GFXfont *font, uint8_t options)
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

    if (options & DIS_VCENTER)
    {
        y0 -= (height - h) / 2;
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

void Display::printHeader()
{
     snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%s %02i-%02i-%02i %02i:%02i %s",
             "MON\0TUE\0WED\0THU\0FRI\0SAT\0SUN\0" + ((Status::getDayOfWeek() - 1) * 4),
             Status::getDay(),
             Status::getMonth(),
             Status::getYear(),
             Status::getHour(),
             Status::getMinute(),
             Status::isDST() ? "DST" : "");
    this->printValue(Peripherals::buffer, 10, 0, EPD_WIDTH, 50, (GFXfont *)&MAIN_DISPLAY_MID_FONT);
}

void Display::powerDown()
{
    this->printValue("Power down", 0, 0, EPD_WIDTH, EPD_HEIGHT, (GFXfont *)&BIG_POWER_DOWN_SCREEN_FONT, DIS_CENTER | DIS_VCENTER);
    this->displayFramebuffer();
}

void Display::printLabelledValue(const char *label, uint16_t x, uint16_t y, uint8_t options, float value, char *unit, const char *v1Label, float v1, const char *v2Label, float v2, const char *v3Label, float v3)
{
    uint16_t midSectionOffset = 115;
    if (options & DIS_LARGE_VALUE)
    {
        midSectionOffset = 230;
    }

    this->printValue(label, x, y + 20, midSectionOffset + 310, 40, (GFXfont *)&MAIN_DISPLAY_LABEL_FONT);

    if (value != NO_VALUE)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%d", (int)floor(value));
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

    this->printValue(unit, x + midSectionOffset, y + 50, 105, 55, (GFXfont *)&MAIN_DISPLAY_LABEL_FONT);

    if (v1Label != NULL)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "%s:%0.0f" : "%s:%0.1f", v1Label, v1);
        this->printValue(Peripherals::buffer, x + midSectionOffset + 95, y + 36, 215, 55, (GFXfont *)&MAIN_DISPLAY_MID_FONT);
    }

    if (v2Label != NULL)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "%s:%0.0f" : "%s:%0.1f", v2Label, v2);
        this->printValue(Peripherals::buffer, x + midSectionOffset + 95, y + 80, 215, 55, (GFXfont *)&MAIN_DISPLAY_MID_FONT);
    }

    if (v3Label != NULL)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "%s:%0.0f" : "%s:%0.1f", v3Label, v3);
        this->printValue(Peripherals::buffer, x + midSectionOffset + 95, y + 124, 215, 55, (GFXfont *)&MAIN_DISPLAY_MID_FONT);
    }
}

void Display::showIcon(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *data, uint8_t options)
{
    Rect_t area = {
        .x = x,
        .y = y,
        .width = width,
        .height = height,
    };

    if (options & DIS_DIRECT_PRINT)
    {
        epd_poweron();
        epd_clear_area(area);
        if (data != NULL)
        {
            epd_draw_grayscale_image(area, data);
        }
        epd_poweroff();

        return;
    }

    epd_copy_to_framebuffer(area, data, Peripherals::framebuffer);
}