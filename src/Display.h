
#ifndef __TEMPERATURE_LOGGER_DISPLAY_H__
#define __TEMPERATURE_LOGGER_DISPLAY_H__

#include "config.h"
#include "PowerManager.h"
#include "Status.h"
#include "Peripherals.h"
#include <DEV_Config.h>
#include <EPD.h>
#include <GUI_Paint.h>

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
    unsigned long lastRefreshTime = 0;

protected:
    void printHeader();
    void printValue(const char *buffer, int x, int y, sFONT *font);

    void displayFramebuffer();

    void printLabelledValue(const char *label,
                            uint16_t x, uint16_t y,
                            uint8_t options,
                            float value, char *unit,
                            const char *v1Label = NULL, float v1 = 0,
                            const char *v2Label = NULL, float v2 = 0,
                            const char *v3Label = NULL, float v3 = 0,
                            const char *v4Label = NULL, float v4 = 0,
                            const char *v5Label = NULL, float v5 = 0,
                            bool extraWideLabels = false);

    void plotGraph(const char *label, uint16_t x, uint16_t y, uint16_t timeRangeMinutes, uint8_t valueIndex, float maxValue);

    virtual void refreshDisplay() = 0;
   
public:
    void loop();
    void showShutdown();  
};

#endif