
#ifndef __TEMPERATURE_LOGGER_DISPLAY_H__
#define __TEMPERATURE_LOGGER_DISPLAY_H__

#include "config.h"
#include "PowerManager.h"
#include "Status.h"
#include "ui.h"
#include "Peripherals.h"
#include "fonts/sevenSeg_60.h"
#include "fonts/sevenSeg_25.h"
#include "fonts/mono15.h"

#define DIS_NONE 0
#define DIS_NO_DECIMAL 1
#define DIS_LARGE_VALUE 2
#define DIS_CENTER 4
#define DIS_DIRECT_PRINT 8
#define DIS_RIGHT 16

class Display
{
private:
protected:
    void printValue(char *buffer, int x, int y, int width, int height, const GFXfont *font, uint8_t options = DIS_NONE);
    void displayFramebuffer();
    unsigned long lastRefreshTime = 0;
    void printVHLValue(char *label, uint16_t x, uint16_t y, uint8_t options, float value, char *unit, float low, float high);
    void showIcon(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *data);

public:
    virtual void loop() = 0;
    virtual void onBClick() = 0;
    virtual void onBLongPress() = 0;
    void powerDown();
};

#endif