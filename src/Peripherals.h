#ifndef __TEMPERATURE_PERIPHERALS_H__
#define __TEMPERATURE_PERIPHERALS_H__

#include <Arduino.h>
#include <uRTCLib.h>
#include <uEEPROMLib.h>
#include <iAQCoreI2C.h>
#include <SparkFun_AS3935.h>
#include <BMP280_DEV.h>
#include "Adafruit_APDS9960.h"
#include <WiFi.h>
#include "drivers/SHT2x.h"
#include "config.h"
#include "Button.h"
#include "secrets.h"
#include "Status.h"
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"

class Peripherals
{
public:
    static uRTCLib *rtc;
    static uEEPROMLib *eeprom;
    static Button *buttonA;
    static Button *buttonB;
    static Button *buttonC;
    static SparkFun_AS3935 *lightning;
    static Adafruit_APDS9960 *apds;
    static UBYTE *framebuffer;
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