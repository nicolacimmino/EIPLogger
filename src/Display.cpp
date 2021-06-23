#include "Display.h"

void Display::onBLongPress()
{
}

void Display::loop()
{
    if (this->lastRefreshTime != 0 && millis() - this->lastRefreshTime < 58000)
    {
        return;
    }

    this->lastRefreshTime = millis();

    this->printHeader();

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
}

void Display::showShutdown()
{
    Paint_ClearWindows(0, 0, 300, 400, WHITE);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%s %02i-%02i-%02i %02i:%02i %0.2f",
             "MON\0TUE\0WED\0THU\0FRI\0SAT\0SUN\0" + ((Status::getDayOfWeek() - 1) * 4),
             Status::getDay(),
             Status::getMonth(),
             Status::getYear(),
             Status::getHour(),
             Status::getMinute(),
             Status::batteryVoltage / 1000.0);

    this->printValue(Peripherals::buffer, 20, 50, MAIN_DISPLAY_MID_FONT);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "Power Down");

    this->printValue(Peripherals::buffer, 20, 70, MAIN_DISPLAY_LARGE_FONT);

    this->displayFramebuffer();
}

void Display::printHeader()
{
    Paint_ClearWindows(0, 0, 300, 15, WHITE);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%s %02i-%02i-%02i %02i:%02i %s %0.2f",
             "MON\0TUE\0WED\0THU\0FRI\0SAT\0SUN\0" + ((Status::getDayOfWeek() - 1) * 4),
             Status::getDay(),
             Status::getMonth(),
             Status::getYear(),
             Status::getHour(),
             Status::getMinute(),
             Status::isDST() ? "DST" : "",
             Status::batteryVoltage / 1000.0);

    this->printValue(Peripherals::buffer, 0, 0, MAIN_DISPLAY_MID_FONT);
}

void Display::printLabelledValue(const char *label, uint16_t x, uint16_t y, uint8_t options, float value, char *unit, const char *v1Label, float v1, const char *v2Label, float v2, const char *v3Label, float v3, const char *v4Label, float v4, const char *v5Label, float v5, bool extraWideLabels)
{
    this->printValue(label, x, y + 45, MAIN_DISPLAY_LABEL_FONT);

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
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, " ---");
    }
    this->printValue(Peripherals::buffer, x, y + 22, MAIN_DISPLAY_LARGE_FONT);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%s", unit);
    this->printValue(Peripherals::buffer, x + 70, y + 22, MAIN_DISPLAY_MID_FONT);

    uint8_t labelsX = extraWideLabels ? 80 : 110;

    if (v1Label != NULL)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "%s%0.0f" : "%s%0.1f", v1Label, v1);
        this->printValue(Peripherals::buffer, x + labelsX, y + 20, MAIN_DISPLAY_SMALL_FONT);
    }

    if (v2Label != NULL)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "%s%0.0f" : "%s%0.1f", v2Label, v2);
        this->printValue(Peripherals::buffer, x + labelsX, y + 32, MAIN_DISPLAY_SMALL_FONT);
    }

    if (v3Label != NULL)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "%s%0.0f" : "%s%0.1f", v3Label, v3);
        this->printValue(Peripherals::buffer, x + labelsX, y + 44, MAIN_DISPLAY_SMALL_FONT);
    }

    if (v4Label != NULL)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "%s%0.0f" : "%s%0.1f", v4Label, v4);
        this->printValue(Peripherals::buffer, x + labelsX, y + 56, MAIN_DISPLAY_SMALL_FONT);
    }

    if (v5Label != NULL)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, (options & DIS_NO_DECIMAL) ? "%s%0.0f" : "%s%0.1f", v5Label, v5);
        this->printValue(Peripherals::buffer, x + labelsX, y + 68, MAIN_DISPLAY_SMALL_FONT);
    }
}

void Display::plotGraph(const char *label, uint16_t x, uint16_t y, uint16_t timeRangeMinutes, uint8_t valueIndex, float maxValue)
{
#define X_AXIS_LEN 130
#define Y_AXIS_LEN 65

    uint16_t axisOriginX = x + 15;
    uint16_t axisOriginY = y + 80;
    uint8_t minutesPerPixel = timeRangeMinutes / X_AXIS_LEN;
    float valuePerPixel = maxValue / Y_AXIS_LEN;

    this->printValue(label, x, y, MAIN_DISPLAY_LABEL_FONT);

    // Draw axis
    Paint_DrawLine(axisOriginX, axisOriginY, axisOriginX + X_AXIS_LEN, axisOriginY, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(axisOriginX, axisOriginY, axisOriginX, axisOriginY - Y_AXIS_LEN, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    for (uint8_t ix = 1; ix < 11; ix++)
    {
        Paint_DrawLine(axisOriginX - 5, axisOriginY - (ix * Y_AXIS_LEN / 10), axisOriginX, axisOriginY - (ix * Y_AXIS_LEN / 10), BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    }

    for (uint8_t ix = 1; ix < 25; ix++)
    {
        Paint_DrawLine(axisOriginX + (ix * X_AXIS_LEN / 24), axisOriginY, axisOriginX + (ix * X_AXIS_LEN / 24), axisOriginY + 5, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    }

    DataLog::instance()->startRetrieval();

    uint16_t lastX = axisOriginX;
    uint16_t lastY = axisOriginY;
    for (uint8_t ix = 0; ix < X_AXIS_LEN; ix++)
    {
        float value = DataLog::instance()->getValue(timeRangeMinutes - (ix * minutesPerPixel), valueIndex);

        if (value == NO_VALUE)
        {
            continue;
        }

        uint8_t y = min((uint8_t)Y_AXIS_LEN, (uint8_t)floor(value / valuePerPixel));

        Paint_DrawLine(lastX, lastY, lastX + 1, axisOriginY - y, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

        lastX++;
        lastY = axisOriginY - y;
    }

    DataLog::instance()->stopRetrieval();
}
