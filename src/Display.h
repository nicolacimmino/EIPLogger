
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
#define DIS_VCENTER 32

class Display
{
private:
protected:
    void printValue(char *buffer, int x, int y, int width, int height, const GFXfont *font, uint8_t options = DIS_NONE);
    void displayFramebuffer();
    unsigned long lastRefreshTime = 0;
    
    void printLabelledValue(char *label,
                            uint16_t x, uint16_t y,
                            uint8_t options,
                            float value, char *unit,
                            char *v1Label = NULL, float v1 = 0,
                            char *v2Label = NULL, float v2 = 0,
                            char *v3Label = NULL, float v3 = 0);

    void showIcon(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *data);

public:
    virtual void loop() = 0;
    virtual void onBClick() = 0;
    virtual void onBLongPress() = 0;
    void powerDown();
};

#endif