#include "LightDisplay.h"

void LightDisplay::onBClick()
{
    this->lastRefreshTime = 0;
}

void LightDisplay::onBLongPress()
{
}

void LightDisplay::loop()
{
    if (this->lastRefreshTime != 0 && millis() - this->lastRefreshTime < 60000)
    {
        return;
    }

    // Top bar
    // TODO: move to common base function with optional header.
    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%s %02i-%02i-%02i %02i:%02i     Fetching Data....",
             "MON\0TUE\0WED\0THU\0FRI\0SAT\0SUN\0" + ((Peripherals::rtc->dayOfWeek() - 1) * 4),
             Peripherals::rtc->day(),
             Peripherals::rtc->month(),
             Peripherals::rtc->year(),
             Peripherals::rtc->hour(),
             Peripherals::rtc->minute());
    this->printValue(Peripherals::buffer, 10, 0, EPD_WIDTH, 50, (GFXfont *)&MAIN_DISPLAY_MID_FONT);
    this->displayFramebuffer();

    this->lastRefreshTime = millis();

    uint16_t r, g, b, a;

    Peripherals::apds->getColorData(&r, &g, &b, &a);

    this->printLightValue("Temperature", this->getAreaX(0), this->getAreaY(0), this->calculateColorTemperature(r, g, b), "K");

    this->printLightValue("Luminance", this->getAreaX(1), this->getAreaY(1), this->calculateLux(r, g, b), "LUX");

    this->displayFramebuffer();
}

void LightDisplay::printLightValue(const char *label, uint16_t x, uint16_t y, int value, const char *unit)
{    
    this->printValue(label, x, y + 20, 240, 40, (GFXfont *)&MAIN_DISPLAY_LABEL_FONT, DIS_RIGHT);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%d", value);
    this->printValue(Peripherals::buffer, x, y + 50, 220, 110, (GFXfont *)&MAIN_DISPLAY_LARGE_FONT, DIS_RIGHT);
    
    this->printValue(unit, x + 230, y + 50, 110, 110, (GFXfont *)&MAIN_DISPLAY_LABEL_FONT, DIS_NONE);
}

uint16_t LightDisplay::getAreaX(uint8_t areaNumber)
{
    return 155 + (areaNumber % 3) * 300;
}

uint16_t LightDisplay::getAreaY(uint8_t areaNumber)
{
    return 50 + ((areaNumber / 3) * 150);
}

float LightDisplay::calculateColorTemperature(uint16_t r, uint16_t g, uint16_t b)
{
    float x = (-0.14282 * r) + (1.54924 * g) + (-0.95641 * b);
    float y = (-0.32466 * r) + (1.57837 * g) + (-0.73191 * b);
    float z = (-0.68202 * r) + (0.77073 * g) + (0.56332 * b);
    float xchrome = x / (x + y + z);
    float ychrome = y / (x + y + z);
    float n = (xchrome - 0.3320) / (0.1858 - ychrome);
    return (449.0 * pow(n, 3)) + (3525.0 * pow(n, 2)) + (6823.3 * n) + 5520.33;
}

float LightDisplay::calculateLux(uint16_t r, uint16_t g, uint16_t b)
{
    return (-0.32466 * r) + (1.57837 * g) + (-0.73191 * b);
}