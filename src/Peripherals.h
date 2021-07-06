#ifndef __TEMPERATURE_PERIPHERALS_H__
#define __TEMPERATURE_PERIPHERALS_H__

#include <Arduino.h>
#include <uRTCLib.h>
#include <iAQCoreI2C.h>
#include "drivers/SHT2x.h"
#include <sps30.h>
#include <WiFi.h>
#include "config.h"
#include "Button.h"
#include "secrets.h"
#include "Status.h"
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "PowerManager.h"

class Peripherals
{
public:
    static uRTCLib *rtc;
    static Button *buttonA;
    static Button *buttonB;
    static Button *buttonC;
    static Button *buttonD;
    static UBYTE *framebuffer;
    static char *buffer;
    
    static void setup();
    static void loop();
    static void connectWiFi();
    static bool isWiFiConnected();
    static void disconnectWiFi();

    static void setRedLed(bool on);
    static void setYellowLed(bool on);
    static void setBlueLed(bool on);

private:
    static iAQCoreI2C *iaq;
    static SHT2x *sht2x;
};

#endif