
#ifndef __TEMPERATURE_LOGGER_DISPLAY_H__
#define __TEMPERATURE_LOGGER_DISPLAY_H__

#include "config.h"
#include "PowerManager.h"
#include "Status.h"
#include "ui.h"
#include "Peripherals.h"

class Display
{
private:
protected:
    void plotBatterLevel();
    bool replotNeeded = true;

public:
    void clearDisplay(uint16_t x = 0, uint16_t y = 0, uint16_t width = EPD_WIDTH, uint16_t height = EPD_HEIGHT);
    virtual void loop() = 0;
    virtual void onBClick() = 0;
    virtual void onBLongPress() = 0;
    void onDataStoreChange();
    void onDisplayAwaken();
};

#endif