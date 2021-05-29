#ifndef __TEMPERATURE_PERIPHERALS_H__
#define __TEMPERATURE_PERIPHERALS_H__

#include <Arduino.h>
#include <uRTCLib.h>
#include <uEEPROMLib.h>
#include <epd_driver.h>
#include <iAQCoreI2C.h>
#include <SparkFun_AS3935.h>
#include <BMP280_DEV.h>
#include <WiFi.h>
#include "drivers/SHT2x.h"
#include "config.h"
#include "Button.h"
#include "secrets.h"

class Peripherals
{
public:
    static uRTCLib *rtc;
    static uEEPROMLib *eeprom;
    static Button *buttonA;
    static Button *buttonB;
    static Button *buttonC;
    static SparkFun_AS3935 *lightning;
    static uint8_t *framebuffer;
    static char *buffer;

    static void setup();
    static void loop();
    static void connectWiFi();
    static bool isWiFiConnected();
    static void disconnectWiFi();

private:
    static iAQCoreI2C *iaq;
    static BMP280_DEV *bmp280;
    static SHT2x *sht2x;
};

#endif