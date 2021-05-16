#include "SHT2x.h"

void SHT2x::loop()
{
    this->temperature = -46.85 + 175.72 / 65536.0 * SHT2x::read(SHT2x_CMD_READ_TEMP);

    if (!this->validData)
    {
        return;
    }

    this->humidity = (uint8_t)(-6.0 + 125.0 / 65536.0 * SHT2x::read(SHT2x_CMD_READ_HUM));
}

bool SHT2x::idDataValid()
{
    return this->validData;
}

uint16_t SHT2x::read(uint8_t command)
{
    Wire.beginTransmission(SHT2x_ADDR);
    Wire.write(command);
    delay(100);
    Wire.endTransmission();
    delay(100);

    Wire.requestFrom(SHT2x_ADDR, 3);
    unsigned long poolStart = millis();
    while (Wire.available() < 3)
    {
        if ((millis() - poolStart) > SHT2x_TOUT)
        {
            this->validData = false;
            return 0;
        }
    }

    uint16_t result = Wire.read() << 8;
    result += Wire.read();
    result &= ~0x0003;

    Wire.read();

    this->validData = true;

    return result;
}
