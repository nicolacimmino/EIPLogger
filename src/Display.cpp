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

void Display::printValue(const char *buffer, int x, int y, sFONT *font)
{    
    Paint_DrawString_EN(x, y, buffer, font, WHITE, BLACK);
}

void Display::displayFramebuffer()
{
    DIAGNOSTIC("DIS,DISPLAY_FRAMEBUFFER");

    EPD_4IN2_Init();
    DEV_Delay_ms(500);

    EPD_4IN2_Display(Peripherals::framebuffer);

    EPD_4IN2_Sleep();

    Paint_Clear(WHITE);
}

void Display::printHeader()
{   
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
    this->printValue(Peripherals::buffer, 10, 0, MAIN_DISPLAY_MID_FONT);
}

void Display::printLabelledValue(const char *label, uint16_t x, uint16_t y, uint8_t options, float value, char *unit, const char *v1Label, float v1, const char *v2Label, float v2, const char *v3Label, float v3)
{
    this->printValue(label, x, y, MAIN_DISPLAY_LABEL_FONT);

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
    this->printValue(Peripherals::buffer, x, y + 22, MAIN_DISPLAY_LARGE_FONT);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%s", unit);
    this->printValue(Peripherals::buffer, x + 70, y + 22, MAIN_DISPLAY_MID_FONT);

    if (v1Label != NULL)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "%s:%0.0f" : "%s:%0.1f", v1Label, v1);
        this->printValue(Peripherals::buffer, x + 110, y + 22, MAIN_DISPLAY_MID_FONT);
    }

    if (v2Label != NULL)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "%s:%0.0f" : "%s:%0.1f", v2Label, v2);
        this->printValue(Peripherals::buffer, x + 110, y + 44, MAIN_DISPLAY_MID_FONT);
    }

    if (v3Label != NULL)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "%s:%0.0f" : "%s:%0.1f", v3Label, v3);
        this->printValue(Peripherals::buffer, x + 110, y + 66, MAIN_DISPLAY_MID_FONT);
    }
}
