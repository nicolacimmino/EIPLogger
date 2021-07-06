#ifndef __TEMPERATURE_STATUS_H__
#define __TEMPERATURE_STATUS_H__

#include <Arduino.h>
#include "MinMaxVal.h"
#include "Utilities.h"
#include "Peripherals.h"

struct Time
{
    uint8_t h;
    uint8_t m;
};

class Status
{
private:
public:
    static long batteryVoltage;
    static int getFreeRamBytes();
    static uint8_t getIAQI();
    static uint8_t getCO2QI();
    static uint8_t getTVOCQI();
    static uint8_t getClimateQI();
    static uint16_t getDayOfYear();
    static bool isDST();
    static uint16_t getHour();
    static uint16_t getMinute();
    static uint16_t getDay();
    static uint16_t getMonth();
    static uint16_t getYear();
    static uint16_t getDayOfWeek();
    static time_t getUnixTime();
    static MinMaxVal<int> *co2;
    static MinMaxVal<int> *tvoc;
    static MinMaxVal<int> *humidity;
    static MinMaxVal<float> *temperature;
    static MinMaxVal<int> *pm1;
    static MinMaxVal<int> *pm2p5;
    static MinMaxVal<int> *pm4;
    static MinMaxVal<int> *pm10;
    static uint8_t getPMAQI();
    static uint8_t getBatteryLevel();
    static bool serverMode;
};

#endif