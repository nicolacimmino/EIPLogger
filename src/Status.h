#ifndef __TEMPERATURE_STATUS_H__
#define __TEMPERATURE_STATUS_H__

#include <Arduino.h>
#include "MinMaxVal.h"

class Status
{
private:
    static unsigned long lastTimeSync;
    static long batteryVoltage;
    static bool abortLoopRequested;
    static bool timeSyncRequsted;

public:
    static void abortLoop();
    static bool shouldAbortLoop();
    static void loopAborted();
    static bool timeSync();
    static bool shouldTimeSync();
    static bool isTimeSynced();
    static void timeSynced();
    static void timeSyncFailed();
    static uint8_t getBatteryLevel();
    static long getBatteryVoltage();
    static int getFreeRamBytes();
    static uint8_t getIAQI();
    static uint8_t getCO2QI();
    static uint8_t getTVOCQI();
    static uint8_t getClimateQI();
    static int thunderStrikes;
    static int thunderDistance;
    static int thunderEnergy;
    static int thunderInterferers;
    static MinMaxVal<int> *barometricPressure;
    static MinMaxVal<int> *co2;
    static MinMaxVal<int> *tvoc;
    static MinMaxVal<int> *humidity;
    static MinMaxVal<float> *temperature;
};

#endif