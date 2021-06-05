#ifndef __TEMPERATURE_STATUS_H__
#define __TEMPERATURE_STATUS_H__

#include <Arduino.h>
#include "MinMaxVal.h"
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "PowerManager.h"
#include "icons/trend_uu.h"
#include "icons/trend_u.h"
#include "icons/trend_dd.h"
#include "icons/trend_d.h"
#include "icons/trend_eq.h"
#include "icons/trend_end.h"

#define POLLEN_TREND_EQ 1
#define POLLEN_TREND_U 2
#define POLLEN_TREND_UU 3
#define POLLEN_TREND_D 4
#define POLLEN_TREND_DD 5
#define POLLEN_TREND_END 6

#define POLLEN_CLADOSPORIUM 20
#define POLLEN_ALTERNARIA 23
#define POLLEN_OAK 29
#define POLLEN_GRASS 31
#define POLLEN_SORREL 34
#define POLLEN_PINE 32
#define POLLEN_PLATAN 33
#define POLLEN_PLANTAGO 36
#define POLLEN_OTHER 99

struct PollenCount
{
    uint8_t count;
    uint8_t trend;
    uint8_t type;
};

class Status
{
private:
    static unsigned long lastPollenSync;
    static long batteryVoltage;
    static bool abortLoopRequested;
    static bool timeSyncRequsted;
    static void syncPollen();

public:
    static void setup();
    static void loop();
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
    static PollenCount pollenCounts[MAX_POLLEN_COUNTS];
};

#endif