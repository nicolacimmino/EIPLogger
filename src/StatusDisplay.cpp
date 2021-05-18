#include "StatusDisplay.h"

void StatusDisplay::onBClick()
{
//    this->runI2CScan();
}

void StatusDisplay::onBLongPress()
{
    return;
}

void StatusDisplay::loop()
{
    char buffer[32];

    if (millis() - this->lastRefreshTime < 60000)
    {
        return;
    }

    sprintf(buffer, "I2C Bus");
    this->printValue(buffer, 0, 90, EPD_WIDTH, 90, (GFXfont *)&FiraSans);

    this->displayFramebuffer();

    this->lastRefreshTime = millis();

    this->runI2CScan();
}

void StatusDisplay::runI2CScan()
{
    int x = 0, y = 200;
    char buffer[32];

    for (byte addr = 1; addr <= 127; addr++)
    {
        Wire.beginTransmission(addr);
Serial.println(addr);        
        if (Wire.endTransmission() == 0)
        {            
            snprintf(buffer, 32, "ADD: %02x", addr);
            epd_poweron();
            writeln((GFXfont *)&FiraSans, buffer, &x, &y, NULL);
            epd_poweroff();

            x = 0;
            y += 50;
        }
        delay(200);
    }
}
