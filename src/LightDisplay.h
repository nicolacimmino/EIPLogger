
#ifndef __LIGHT_DISPLAY_H__
#define __LIGHT_DISPLAY_H__

#include "Display.h"

class LightDisplay : public Display
{
protected:
    void refreshDisplay();

private:
    void printLightValue(const char *label, uint16_t x, uint16_t y, int value, const char *unit);
    void printTimeValue(const char *label, uint16_t x, uint16_t y, Time time);
    uint16_t getAreaX(uint8_t areaNumber);
    uint16_t getAreaY(uint8_t areaNumber);
    float calculateColorTemperature(uint16_t r, uint16_t g, uint16_t b);
    float calculateLux(uint16_t r, uint16_t g, uint16_t b);

public:
    void onBClick();
    void onBLongPress();
};

#endif
