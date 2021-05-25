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
    char buffer[32];

    if (this->lastRefreshTime != 0 && millis() - this->lastRefreshTime < 60000)
    {
        return;
    }

    this->lastRefreshTime = millis();

    this->runI2CScan();
}

void StatusDisplay::runI2CScan()
{
    int y = 180;
    char buffer[64];

    uint8_t addresses[] = {AS3935_ADDR, SHT2x_ADDR, IAQ_ADDR, EEPROM_ADDR, RTC_ADDR, BMP280_ADDR};

    sprintf(buffer, "Status");
    this->printValue(buffer, 0, 90, EPD_WIDTH, 90, (GFXfont *)&FiraSans, true);

    this->displayFramebuffer();

    for (uint8_t ix = 0; ix < sizeof(addresses); ix++)
    {
        Wire.beginTransmission(addresses[ix]);

        snprintf(buffer, 64, " I2C 0x%02x...........[%s]", addresses[ix], Wire.endTransmission() == 0 ? "OK" : "FAIL");

        this->printValue(buffer, 10, y, EPD_WIDTH / 2, 50, (GFXfont *)&FiraSans, false, true);

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

        snprintf(buffer, 64, " WiFi...............[%d/10]", attempts);

        this->printValue(buffer, 10, y, EPD_WIDTH / 2, 50, (GFXfont *)&FiraSans, false, true);
        
        delay(3000);
    }

    snprintf(buffer, 64, " WiFi...............[%s]    ", Peripherals::isWiFiConnected() ? "OK" : "FAIL");

    this->printValue(buffer, 10, y, EPD_WIDTH / 2, 50, (GFXfont *)&FiraSans, false, true);

    PowerManager::enterL1();
}
