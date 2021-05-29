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

    this->runI2CScan();
}

void StatusDisplay::runI2CScan()
{
    int y = 160;

    uint8_t addresses[] = {AS3935_ADDR, SHT2x_ADDR, IAQ_ADDR, EEPROM_ADDR, RTC_ADDR, BMP280_ADDR};

    this->printValue("Status", 0, 90, EPD_WIDTH / 2, 90, (GFXfont *)&SMALL_STATUS_SCREEN_FONT, true);

    this->displayFramebuffer();

    for (uint8_t ix = 0; ix < sizeof(addresses); ix++)
    {
        Wire.beginTransmission(addresses[ix]);

        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, " I2C 0x%02x...........[%s]", addresses[ix], Wire.endTransmission() == 0 ? "OK" : "FAIL");

        this->printValue(Peripherals::buffer, 10, y, EPD_WIDTH / 2, 50, (GFXfont *)&SMALL_STATUS_SCREEN_FONT, DIS_DIRECT_PRINT);

        y += 50;

        delay(200);
    }

    PowerManager::enterL0();

    uint8_t attempts = 0;

    while (!Peripherals::isWiFiConnected())
    {
        if (attempts > 10)
        {
            break;
        }

        attempts++;

        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, " WiFi...............[%d/10]", attempts);

        this->printValue(Peripherals::buffer, 10, y, EPD_WIDTH / 2, 50, (GFXfont *)&SMALL_STATUS_SCREEN_FONT, DIS_DIRECT_PRINT);

        delay(3000);
    }

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, " WiFi...............[%s]", Peripherals::isWiFiConnected() ? "OK" : "FAIL");

    this->printValue(Peripherals::buffer, 10, y, EPD_WIDTH / 2, 50, (GFXfont *)&SMALL_STATUS_SCREEN_FONT, DIS_DIRECT_PRINT);
    y += 50;

    PowerManager::enterL1();
}
