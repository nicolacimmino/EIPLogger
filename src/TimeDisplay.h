
#ifndef __TEMPERATURE_LOGGER_TIME_DISPLAY_H__
#define __TEMPERATURE_LOGGER_TIME_DISPLAY_H__

#include "Display.h"
#include "DataStore.h"
#include "fonts/sevenSeg_70.h"
#include "fonts/firasans.h"
#include "drivers/SHT2x.h" // TODO: could get it in Perihperals somehow even though it's just a static instance

#define TIME_MODE_TIME 0
#define TIME_MODE_DATE 1
#define TIME_MODES_COUNT 2

#define TIME_REVERT_TO_DEFAULT_DELAY_MS 5000

class TimeDisplay : public Display
{
private:
    uRTCLib *rtc;
    void displayTime();
    uint8_t mode = TIME_MODE_TIME;
    unsigned long lastModeChangeTime = 0;

public:
    void loop();
    void onBClick();
    void onBLongPress();
};

#endif