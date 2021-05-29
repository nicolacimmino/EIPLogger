#ifndef __TEMPERATURE_STATUS_H__
#define __TEMPERATURE_STATUS_H__

#include <Arduino.h>

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
    static int thunderStrikes;
    static int thunderDistance;
    static int thunderEnergy;
    static int thunderInterferers;
    static int barometricPressure;
    static int co2;
    static int tvoc;
    static float temperature;
    static int humidity;
};

#endif