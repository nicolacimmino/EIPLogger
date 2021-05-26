
#ifndef __TEMPERATURE_LOGGER_DISPLAY_H__
#define __TEMPERATURE_LOGGER_DISPLAY_H__

#include "config.h"
#include "PowerManager.h"
#include "Status.h"
#include "ui.h"
#include "Peripherals.h"
#include "fonts/sevenSeg_70.h"
#include "fonts/sevenSeg_25.h"
#include "fonts/mono15.h"

class Display
{
private:
protected:
    void printValue(char *buffer, int x, int y, int width, int height, const GFXfont *font, bool center, bool skipFrameBuffer = false);
    void displayFramebuffer();
    unsigned long lastRefreshTime = 0;
public:    
    virtual void loop() = 0;
    virtual void onBClick() = 0;
    virtual void onBLongPress() = 0;    
    void powerDown();
};

#endif