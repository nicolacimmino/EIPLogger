#ifndef __TEMPERATURE_MODE_MANAGER_H__
#define __TEMPERATURE_MODE_MANAGER_H__

#include <Arduino.h>
#include "config.h"
#include "Display.h"
#include "MainDisplay.h"
#include "StatusDisplay.h"
#include "PollenDisplay.h"
#include "LightDisplay.h"
#include "ThunderDisplay.h"

class ModeManager
{
private:
    static uint8_t mode;
    static void enterMode();

public:
    static Display *currentDisplay;
    static void setup();
    static void changeMode();
};

#endif