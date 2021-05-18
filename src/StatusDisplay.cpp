#include "StatusDisplay.h"

void StatusDisplay::onBClick()
{
    return;
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

    sprintf(buffer, "STATUS");
    this->printValue(buffer, EPD_WIDTH / 2, 480, EPD_WIDTH / 2, 90, (GFXfont *)&FiraSans);

    this->displayFramebuffer();

    this->lastRefreshTime = millis();
}

void StatusDisplay::displayStatus()
{
}
