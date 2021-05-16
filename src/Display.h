
#ifndef __TEMPERATURE_LOGGER_DISPLAY_H__
#define __TEMPERATURE_LOGGER_DISPLAY_H__

#include "config.h"
#include "PowerManager.h"
#include "Status.h"
#include "ui.h"
#include "Peripherals.h"
#include "fonts/sevenSeg_70.h"

class Display
{
private:
protected:
    void plotBatterLevel();
    bool replotNeeded = true;
    void printValue(char *buffer, int x, int y, int width, int height, const GFXfont *font, bool clearField = false);
public:
    void clearDisplay(uint16_t x = 0, uint16_t y = 0, uint16_t width = EPD_WIDTH, uint16_t height = EPD_HEIGHT);
    virtual void loop() = 0;
    virtual void onBClick() = 0;
    virtual void onBLongPress() = 0;
    void onDataStoreChange();
    void onDisplayAwaken();
    void powerDown();
};

#endif