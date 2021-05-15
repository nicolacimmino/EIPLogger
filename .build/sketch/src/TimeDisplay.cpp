#include "TimeDisplay.h"

void TimeDisplay::onBClick()
{
    this->mode = (this->mode + 1) % TIME_MODES_COUNT;
    this->lastModeChangeTime = millis();
}

void TimeDisplay::onBLongPress()
{
    //Status::timeSync();
}

void TimeDisplay::loop()
{
    if (PowerManager::level != PS_LEVEL_0 || Status::shouldAbortLoop())
    {
        return;
    }

    if (millis() - this->lastModeChangeTime > TIME_REVERT_TO_DEFAULT_DELAY_MS)
    {
        this->mode = TIME_MODE_TIME;
    }

    this->displayTime();
}

void TimeDisplay::displayTime()
{
    char buffer[16];

    Peripherals::rtc->refresh();

    this->clearDisplay(0, 0, 300, 100);
    
    if (this->mode == TIME_MODE_TIME)
    {
        int x = 50;
        int y = 50;

        sprintf(buffer, "%02i:%02i", Peripherals::rtc->hour(), Peripherals::rtc->minute());
        writeln((GFXfont *)&FiraSans, buffer, &x, &y, NULL);
    }
    else if (this->mode == TIME_MODE_DATE)
    {
    }
}