#ifndef __TEMPERATURE_PERIPHERALS_H__
#define __TEMPERATURE_PERIPHERALS_H__

#include <Arduino.h>
#include <uRTCLib.h>
#include <uEEPROMLib.h>
#include <epd_driver.h>
#include <iAQCoreTwoWire.h>
#include <SparkFun_AS3935.h>
#include <BMP280_DEV.h>
#include "drivers/SHT2x.h"
#include "fonts/firasans.h"
#include "drivers/SHT2x.h"
#include "config.h"
#include "Button.h"

class Peripherals
{

public:
    static SHT2x *sht2x;
    static uRTCLib *rtc;
    static uEEPROMLib *eeprom;
    static Button *buttonA;
    static Button *buttonB;
    static Button *buttonC;
    static iAQCoreTwoWire *iaq;
    static SparkFun_AS3935 *lightning;
    static BMP280_DEV *bmp280;
    static uint8_t *framebuffer;

    static void setup();
    static void loop();
};

#endif