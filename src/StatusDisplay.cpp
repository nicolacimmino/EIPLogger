#include "StatusDisplay.h"

void StatusDisplay::onBClick()
{
    this->runI2CScan();
}

void StatusDisplay::onBLongPress()
{
    return;
}

void StatusDisplay::loop()
{
    if (this->lastRefreshTime != 0 && millis() - this->lastRefreshTime < 60000)
    {
        return;
    }

    this->lastRefreshTime = millis();

    this->printHeader();

    this->printValue("Press B to scan I2C bus.", 0, 40, EPD_WIDTH / 2, 90, (GFXfont *)&SMALL_STATUS_SCREEN_FONT, true);

    this->displayFramebuffer();
}

void StatusDisplay::runI2CScan()
{
    uint8_t addresses[] = {AS3935_ADDR, APDS_9960_ADDR, SHT2x_ADDR, IAQ_ADDR, EEPROM_ADDR, RTC_ADDR, BMP280_ADDR};

    this->displayFramebuffer();

    for (uint8_t ix = 0; ix < sizeof(addresses); ix++)
    {
        Wire.beginTransmission(addresses[ix]);

        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, " I2C 0x%02x...........[%s]", addresses[ix], Wire.endTransmission() == 0 ? "OK" : "FAIL");

        this->printValue(Peripherals::buffer, 10, 100 + (50 * ix), EPD_WIDTH / 2, 50, (GFXfont *)&SMALL_STATUS_SCREEN_FONT, DIS_DIRECT_PRINT);

        delay(200);
    }
}
