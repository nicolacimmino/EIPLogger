#ifndef __TEMPERATURE_POWERMANAGER_H__
#define __TEMPERATURE_POWERMANAGER_H__

#include <Arduino.h>
#include "config.h"
#include "Peripherals.h"

// L0   Fully operational (as L1 but also WiFi connected)
// L1   Operational (main loop running, display refresh)
// L2   Sleep (button interrupt wakeup, timer wakeup) 
// L3   Power off (button interrupt wakeup)

#define PS_LEVEL_0 0
#define PS_LEVEL_1 1
#define PS_LEVEL_2 2
#define PS_LEVEL_3 3

// Some I2C devices can interfere with the bus
// for a short time during power up. This is a
// guard between powring back up aux devs and
// accessing the bus.
#define PS_BUS_GUARD_TIME_MS 100

class PowerManager
{
private:
    static unsigned long lastUserInteractionTime;
    static uint8_t previousLevel;

public:
    static uint8_t level;

    static bool enterL0();
    static void enterL1();
    static void enterL2();
    static void enterL3();
    static void loop();
    static void onUserInteratcion();    
};

#endif