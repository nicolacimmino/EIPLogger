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
    uint16_t read(uint8_t command);
    bool validData = false;

public:
    float temperature;
    uint8_t humidity;
    void loop();
    bool idDataValid();
};

#endif