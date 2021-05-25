
#ifndef __TEMPERATURE_LOGGER_STATUS_DISPLAY_H__
#define __TEMPERATURE_LOGGER_STATUS_DISPLAY_H__

#include "Display.h"
#include <ESP32Ping.h>

class StatusDisplay : public Display
{
private:
    void runI2CScan();

public:
    void loop();
    void onBClick();
    void onBLongPress();
};

#endif