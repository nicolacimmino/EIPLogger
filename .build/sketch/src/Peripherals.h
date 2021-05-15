#ifndef __TEMPERATURE_PERIPHERALS_H__
#define __TEMPERATURE_PERIPHERALS_H__

#include <Arduino.h>
#include <Sodaq_SHT2x.h>
#include <uRTCLib.h>
#include <uEEPROMLib.h>
#include <epd_driver.h>
#include <iAQCoreTwoWire.h>
#include "SparkFun_AS3935.h"
#include "fonts/firasans.h"
#include "config.h"
#include "Button.h"

class Peripherals
{

public:
    static uRTCLib *rtc;
    static uEEPROMLib *eeprom;
    static Button *buttonA;
    static Button *buttonB;
    static SHT2xClass *sht2x;
    static iAQCoreTwoWire *iaq;
    static SparkFun_AS3935 *lightning;

    static void setup();
};

#endif