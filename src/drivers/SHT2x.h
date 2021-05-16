#ifndef __SHT2X_H__
#define __SHT2X_H__

#include <Wire.h>

#define SHT2x_ADDR 0x40
#define SHT2x_TOUT 600
#define SHT2x_CMD_READ_TEMP 0xE3
#define SHT2x_CMD_READ_HUM 0xE5

class SHT2x
{
private:
    static uint16_t read(uint8_t command);
    static bool validData;

public:
    static float temperature;
    static float humidity;
    static void loop();
    static bool idDataValid();
};

#endif