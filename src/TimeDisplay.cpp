#include "TimeDisplay.h"

void TimeDisplay::onBClick()
{
    // this->mode = (this->mode + 1) % TIME_MODES_COUNT;
    // this->lastModeChangeTime = millis();
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
    char buffer[32];
    static uint8_t lastMinute = 255;

    Peripherals::rtc->refresh();

    Serial.println(Peripherals::rtc->second());

    if (lastMinute == Peripherals::rtc->minute())
    {
        return;
    }

    this->clearDisplay();

    sprintf(buffer, "%02i-%02i-%02i %02i:%02i", Peripherals::rtc->day(), Peripherals::rtc->month(), Peripherals::rtc->year(), Peripherals::rtc->hour(), Peripherals::rtc->minute());

    this->printValue(buffer, 0, 120, EPD_WIDTH, 120, (GFXfont *)&sevenSeg70);

    sprintf(buffer, "Temp: %0.2f", Peripherals::sht2x->GetTemperature());

    this->printValue(buffer, 0, 210, EPD_WIDTH / 2, 90, (GFXfont *)&FiraSans);

    sprintf(buffer, "Humidity: %i", Peripherals::sht2x->GetHumidity());

    this->printValue(buffer, EPD_WIDTH / 2, 210, EPD_WIDTH / 2, 90, (GFXfont *)&FiraSans);

    if (Peripherals::iaq->hasValue())
    {
        snprintf(buffer, 128, "CO2: %i", Peripherals::iaq->getCO2());
        this->printValue(buffer, 0, 300, EPD_WIDTH / 2, 90, (GFXfont *)&FiraSans);

        snprintf(buffer, 128, "TVOC: %i", Peripherals::iaq->getTVOC());
        this->printValue(buffer, EPD_WIDTH / 2, 300, EPD_WIDTH / 2, 90, (GFXfont *)&FiraSans);
    }
    else
    {
        snprintf(buffer, 128, "CO2: ---");
        this->printValue(buffer, 0, 300, EPD_WIDTH / 2, 90, (GFXfont *)&FiraSans);

        snprintf(buffer, 128, "TVOC: ---");
        this->printValue(buffer, EPD_WIDTH / 2, 300, EPD_WIDTH / 2, 90, (GFXfont *)&FiraSans);
    }

    snprintf(buffer, 128, "INT: %i", Status::thunderInterferers);
    this->printValue(buffer, 0, 390, EPD_WIDTH / 2, 90, (GFXfont *)&FiraSans);

    snprintf(buffer, 128, "STK: %i", Status::thunderStrikes);
    this->printValue(buffer, EPD_WIDTH / 2, 390, EPD_WIDTH / 2, 90, (GFXfont *)&FiraSans);

    snprintf(buffer, 128, "DST: %i", Status::thunderDistance);
    this->printValue(buffer, 0, 480, EPD_WIDTH / 2, 90, (GFXfont *)&FiraSans);

    snprintf(buffer, 128, "ENE: %i", Status::thunderEnergy);
    this->printValue(buffer, EPD_WIDTH / 2, 480, EPD_WIDTH / 2, 90, (GFXfont *)&FiraSans);

    lastMinute = Peripherals::rtc->minute();
}