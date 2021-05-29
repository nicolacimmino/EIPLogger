#include "Status.h"

unsigned long Status::lastTimeSync = 0;
long Status::batteryVoltage = 0;
bool Status::abortLoopRequested = false;
bool Status::timeSyncRequsted = true;
int Status::thunderStrikes = 0;
int Status::thunderDistance = 0;
int Status::thunderEnergy = 0;
int Status::thunderInterferers = 0;
MinMaxVal<float> *Status::temperature = new MinMaxVal<float>();
MinMaxVal<int> *Status::barometricPressure = new MinMaxVal<int>();
MinMaxVal<int> *Status::co2 = new MinMaxVal<int>();
MinMaxVal<int> *Status::tvoc = new MinMaxVal<int>();
MinMaxVal<int> *Status::humidity = new MinMaxVal<int>();

void Status::abortLoop()
{
    Status::abortLoopRequested = true;
}

bool Status::shouldAbortLoop()
{
    return Status::abortLoopRequested;
}

void Status::loopAborted()
{
    Status::abortLoopRequested = false;
}

bool Status::timeSync()
{
    Status::timeSyncRequsted = true;
}

bool Status::shouldTimeSync()
{
    return Status::timeSyncRequsted;
}

bool Status::isTimeSynced()
{
    return (lastTimeSync > 0) && (millis() - lastTimeSync) < 86400000;
}

void Status::timeSynced()
{
    lastTimeSync = millis();
    Status::timeSyncRequsted = false;
}

void Status::timeSyncFailed()
{
    Status::timeSyncRequsted = false;
}

long Status::getBatteryVoltage()
{
    return Status::batteryVoltage;
}

uint8_t Status::getBatteryLevel()
{
    // static uint8_t batteryLevel = 0;
    // static unsigned long lastMeasurementTime = 0;

    // if (batteryLevel != 0 && millis() - lastMeasurementTime < 1000)
    // {
    //     return batteryLevel;
    // }

    // lastMeasurementTime = millis();

    // // See this article for an in-depth explanation.
    // // https://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
    // // tl;dr: we switch the ADC to measure the internal 1.1v reference using Vcc as reference, the rest is simple math.

    // ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);

    // delay(2);
    // ADCSRA |= _BV(ADSC);
    // while (bit_is_set(ADCSRA, ADSC))
    //     ;

    // Status::batteryVoltage = 1125300L / (ADCL | (ADCH << 8));
    // analogReference(DEFAULT);

    // // We assume 3900mV max and 2900 being the safe discharge level. 3900-2900 => 1000
    // // 1000 / 10 => 100 (%).
    // uint8_t measuredLevel = min(max((Status::batteryVoltage - 2900) / 10, 0), 100);

    // // Init the IIR filter with the first sample otherwise the % indicator will ramp up slowly at power on.
    // if (batteryLevel == 0)
    // {
    //     batteryLevel = measuredLevel;
    // }

    // batteryLevel = (0.9 * (float)batteryLevel) + (0.1 * (float)measuredLevel);

    // return batteryLevel;
    return 0;
}

/**
 * See https://playground.arduino.cc/Code/AvailableMemory/
 **/
int Status::getFreeRamBytes()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

uint8_t Status::getIAQI()
{
    return max(max(Status::getCO2QI(), Status::getTVOCQI()), Status::getClimateQI());
}

uint8_t Status::getCO2QI()
{
    uint8_t co2I = 1;
    if (Status::co2->get() > 5000)
    {
        co2I = 6;
    }
    else if (Status::co2->get() > 2000)
    {
        co2I = 5;
    }
    else if (Status::co2->get() > 1500)
    {
        co2I = 4;
    }
    else if (Status::co2->get() > 100)
    {
        co2I = 3;
    }
    else if (Status::co2->get() > 400)
    {
        co2I = 2;
    }

    return co2I;
}

uint8_t Status::getTVOCQI()
{
    uint8_t tvocI = 1;
    if (Status::tvoc->get() > 5000)
    {
        tvocI = 6;
    }
    else if (Status::tvoc->get() > 1500)
    {
        tvocI = 5;
    }
    else if (Status::tvoc->get() > 500)
    {
        tvocI = 4;
    }
    else if (Status::tvoc->get() > 150)
    {
        tvocI = 3;
    }
    else if (Status::tvoc->get() > 50)
    {
        tvocI = 2;
    }

    return tvocI;
}

uint8_t Status::getClimateQI()
{
    uint8_t humBin = min(max(1, (int)floor(Status::humidity->get() / 10.0)), 9) - 1; // 0%-100% => 0 - 8
    uint8_t tempBin = min(max(16, (int)Status::temperature->get()), 28) - 16;        // -40-+40 => 16-28 => 0-12
    uint8_t lookup[9][13] = {
        {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
        {6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6},
        {6, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 5, 6},
        {6, 5, 5, 4, 3, 3, 3, 2, 2, 3, 4, 5, 6},
        {5, 5, 4, 3, 2, 1, 1, 1, 2, 3, 4, 5, 6},
        {5, 4, 3, 2, 1, 1, 1, 2, 2, 3, 4, 5, 6},
        {5, 4, 3, 2, 1, 1, 1, 2, 3, 4, 5, 5, 6},
        {5, 4, 2, 2, 2, 2, 2, 3, 4, 5, 5, 5, 6},
        {6, 5, 4, 3, 3, 3, 3, 4, 5, 5, 6, 6, 6}};

    return lookup[humBin][tempBin];
}