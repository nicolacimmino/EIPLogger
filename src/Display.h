
#ifndef __TEMPERATURE_LOGGER_DISPLAY_H__
#define __TEMPERATURE_LOGGER_DISPLAY_H__

#include "config.h"
#include "PowerManager.h"
#include "Status.h"
#include "ui.h"
#include "Peripherals.h"
#include "icons/thunder.h"
#include "icons/battery_100.h"
#include "icons/wifi.h"
#include "icons/padlock.h"
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"

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
    unsigned long lastHeaderRefreshTime = 0;  
    unsigned long lastRefreshTime = 0;  
    bool lastWiFiStatus = false;

protected:
    void printHeader();
    void printValue(const char *buffer, int x, int y, int width, int height, sFONT *font, uint8_t options = DIS_NONE);
    void printSimpleValue(const char *label, uint16_t x, uint16_t y, int value, const char *unit);
    void printTimeValue(const char *label, uint16_t x, uint16_t y, Time time);
    
    void displayFramebuffer();
    
    void printLabelledValue(const char *label,
                            uint16_t x, uint16_t y,
                            uint8_t options,
                            float value, char *unit,
                            const char *v1Label = NULL, float v1 = 0,
                            const char *v2Label = NULL, float v2 = 0,
                            const char *v3Label = NULL, float v3 = 0);

    void showIcon(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *data, uint8_t options = DIS_NONE);

    virtual void refreshDisplay() = 0;
    void forceFullDraw();

public:
    void loop();    
    virtual void onBClick() = 0;
    virtual void onBLongPress() = 0;
    void setWiFiIcon(bool visible);
    void setLockedIcon(bool visible);
    void powerDown();

};

#endif