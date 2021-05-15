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
    if (PowerManager::level != PS_LEVEL_0 || Status::shouldAbortLoop())
    {
        return;
    }

    this->displayStatus();
}

void StatusDisplay::displayStatus()
{
    this->clearDisplay();
   
}
