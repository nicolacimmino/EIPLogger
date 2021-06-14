#include "Display.h"

void Display::onBLongPress()
{
    return;
}

void Display::forceFullDraw()
{
    this->lastRefreshTime = 0;
    this->lastHeaderRefreshTime = 0;
    this->loop();
}

void Display::loop()
{
    this->printHeader();

    if (this->lastRefreshTime != 0 && millis() - this->lastRefreshTime < 60000)
    {
        return;
    }

    this->lastRefreshTime = millis();

    this->refreshDisplay();

    this->displayFramebuffer();
}

void Display::setWiFiIcon(bool visible)
{
    //this->showIcon(800, 15, WIFI_WIDTH, WIFI_HEIGHT, visible ? (uint8_t *)wifi_data : NULL, DIS_DIRECT_PRINT);
}

void Display::setLockedIcon(bool visible)
{
    //this->showIcon(730, 15, PADLOCK_WIDTH, PADLOCK_WIDTH, visible ? (uint8_t *)padlock_data : NULL, DIS_DIRECT_PRINT);
}

void Display::printValue(const char *buffer, int x, int y, int width, int height, sFONT *font, uint8_t options)
{
    //y = y + height;

    int x0 = x;
    int y0 = y;
    int x1;
    int y1;
    int w;
    int h;

    // if (options & DIS_DIRECT_PRINT)
    // {

    //     Rect_t area = {
    //         .x = x0,
    //         .y = y0 - height,
    //         .width = width,
    //         .height = height,
    //     };

    //     epd_poweron();
    //     epd_clear_area(area);
    //     epd_poweroff();
    // }

    // get_text_bounds(font, buffer, &x, &y, &x1, &y1, &w, &h, NULL);

    // if (options & DIS_CENTER)
    // {
    //     x0 += (width - w) / 2;
    // }
    // else if (options & DIS_RIGHT)
    // {
    //     x0 += (width - w);
    // }

    // if (options & DIS_VCENTER)
    // {
    //     y0 -= (height - h) / 2;
    // }

    // if (options & DIS_DIRECT_PRINT)
    // {
    //     epd_poweron();
    //     writeln(font, buffer, &x0, &y0, NULL);
    //     epd_poweroff();
    // }
    // else
    // {
    //     writeln(font, buffer, &x0, &y0, Peripherals::framebuffer);
    // }

    //Paint_Clear(WHITE);

    Paint_DrawString_EN(x0, y0, buffer, font, WHITE, BLACK);
}

void Display::displayFramebuffer()
{
    DIAGNOSTIC("DIS,DISPLAY_FRAMEBUFFER");

    EPD_4IN2_Init();
    // EPD_4IN2_Clear();
    DEV_Delay_ms(500);

    EPD_4IN2_Display(Peripherals::framebuffer);

    EPD_4IN2_Sleep();

    //memset(Peripherals::framebuffer, 0xFF, (EPD_4IN2_WIDTH / 8 + 1) * EPD_4IN2_HEIGHT);
    Paint_Clear(WHITE);
}

void Display::printHeader()
{
    if (this->lastWiFiStatus != Peripherals::isWiFiConnected())
    {
        this->setWiFiIcon(Peripherals::isWiFiConnected());
        this->setLockedIcon(Status::locked);
        this->lastWiFiStatus = Peripherals::isWiFiConnected();
    }

    if (this->lastHeaderRefreshTime != 0 && millis() - this->lastHeaderRefreshTime < 60000)
    {
        return;
    }

    this->lastHeaderRefreshTime = millis();

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%s %02i-%02i-%02i %02i:%02i %s",
             "MON\0TUE\0WED\0THU\0FRI\0SAT\0SUN\0" + ((Status::getDayOfWeek() - 1) * 4),
             Status::getDay(),
             Status::getMonth(),
             Status::getYear(),
             Status::getHour(),
             Status::getMinute(),
             Status::isDST() ? "DST" : "");
    this->printValue(Peripherals::buffer, 10, 0, EPD_4IN2_WIDTH, 10, MAIN_DISPLAY_MID_FONT);
}

void Display::powerDown()
{
    this->printValue("Power down", 0, 0, EPD_4IN2_WIDTH, EPD_4IN2_HEIGHT, BIG_POWER_DOWN_SCREEN_FONT, DIS_CENTER | DIS_VCENTER);
    this->displayFramebuffer();
}

void Display::printSimpleValue(const char *label, uint16_t x, uint16_t y, int value, const char *unit)
{
    this->printValue(label, x, y + 20, 240, 40, MAIN_DISPLAY_LABEL_FONT);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%04d", value);
    this->printValue(Peripherals::buffer, x, y + 50, 220, 110, MAIN_DISPLAY_LARGE_FONT);

    this->printValue(unit, x + 230, y + 50, 110, 110, MAIN_DISPLAY_LABEL_FONT, DIS_NONE);
}

void Display::printTimeValue(const char *label, uint16_t x, uint16_t y, Time time)
{
    this->printValue(label, x, y + 20, 240, 40, MAIN_DISPLAY_LABEL_FONT);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%02d:%02d", time.h, time.m);
    this->printValue(Peripherals::buffer, x, y + 50, 220, 110, MAIN_DISPLAY_LARGE_FONT);
}

void Display::printLabelledValue(const char *label, uint16_t x, uint16_t y, uint8_t options, float value, char *unit, const char *v1Label, float v1, const char *v2Label, float v2, const char *v3Label, float v3)
{
    this->printValue(label, x, y, 100, 22, MAIN_DISPLAY_LABEL_FONT);

    if (value != NO_VALUE)
    {
        if (options & DIS_NO_DECIMAL)
        {
            snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%*d", 4, (int)floor(value));
        }
        else
        {
            snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%0.1f", value);
        }
    }
    else
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "---");
    }
    this->printValue(Peripherals::buffer, x, y + 22, 80, 22, MAIN_DISPLAY_LARGE_FONT, DIS_RIGHT);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%s", unit);
    this->printValue(Peripherals::buffer, x + 70, y + 22, 80, 22, MAIN_DISPLAY_MID_FONT, DIS_RIGHT);

    if (v1Label != NULL)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "%s:%0.0f" : "%s:%0.1f", v1Label, v1);
        this->printValue(Peripherals::buffer, x + 110, y + 22, 100, 22, MAIN_DISPLAY_MID_FONT);
    }

    if (v2Label != NULL)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "%s:%0.0f" : "%s:%0.1f", v2Label, v2);
        this->printValue(Peripherals::buffer, x + 110, y + 44, 100, 22, MAIN_DISPLAY_MID_FONT);
    }

    if (v3Label != NULL)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "%s:%0.0f" : "%s:%0.1f", v3Label, v3);
        this->printValue(Peripherals::buffer, x + 110, y + 66, 100, 22, MAIN_DISPLAY_MID_FONT);
    }
}

void Display::showIcon(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *data, uint8_t options)
{
    // Rect_t area = {
    //     .x = x,
    //     .y = y,
    //     .width = width,
    //     .height = height,
    // };

    // if (options & DIS_DIRECT_PRINT)
    // {
    //     epd_poweron();
    //     epd_clear_area(area);
    //     if (data != NULL)
    //     {
    //         epd_draw_grayscale_image(area, data);
    //     }
    //     epd_poweroff();

    //     return;
    // }

    // epd_copy_to_framebuffer(area, data, Peripherals::framebuffer);
}