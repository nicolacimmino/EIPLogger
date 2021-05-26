#include "TimeDisplay.h"

void TimeDisplay::onBClick()
{
    this->lastRefreshTime = 0;
}

void TimeDisplay::onBLongPress()
{
    //Status::timeSync();
}

void TimeDisplay::loop()
{
    this->displayTime();
}

void TimeDisplay::displayTime()
{
    if (this->lastRefreshTime != 0 && millis() - this->lastRefreshTime < 60000)
    {
        return;
    }

    Peripherals::rtc->refresh();

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%02i-%02i-%02i %02i:%02i", Peripherals::rtc->day(), Peripherals::rtc->month(), Peripherals::rtc->year(), Peripherals::rtc->hour(), Peripherals::rtc->minute());

    this->printValue(Peripherals::buffer, 0, 120, EPD_WIDTH, 120, (GFXfont *)&BIG_TIME_SCREEN_FONT, true, false);

    if (Peripherals::sht2x->idDataValid())
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "Temp: %0.2f C", Peripherals::sht2x->temperature);

        this->printValue(Peripherals::buffer, 0, 210, EPD_WIDTH / 2, 90, (GFXfont *)&SMALL_TIME_SCREEN_FONT, true, false);

        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "Humidity: %d %%RH", Peripherals::sht2x->humidity);

        this->printValue(Peripherals::buffer, EPD_WIDTH / 2, 210, EPD_WIDTH / 2, 90, (GFXfont *)&SMALL_TIME_SCREEN_FONT, true, false);
    }
    else
    {
        this->printValue("Temp: ---", 0, 210, EPD_WIDTH / 2, 90, (GFXfont *)&SMALL_TIME_SCREEN_FONT, true, false);
        this->printValue("Humidity: ---", EPD_WIDTH / 2, 210, EPD_WIDTH / 2, 90, (GFXfont *)&SMALL_TIME_SCREEN_FONT, true, false);
    }

    Wire.setClock(80000L);
    if (Peripherals::iaq->hasValue() && Peripherals::iaq->isValid())
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "CO2: %i ppm", Peripherals::iaq->getCO2());
        this->printValue(Peripherals::buffer, 0, 300, EPD_WIDTH / 2, 90, (GFXfont *)&SMALL_TIME_SCREEN_FONT, true, false);

        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "TVOC: %i ppb", Peripherals::iaq->getTVOC());
        this->printValue(Peripherals::buffer, EPD_WIDTH / 2, 300, EPD_WIDTH / 2, 90, (GFXfont *)&SMALL_TIME_SCREEN_FONT, true, false);
    }
    else
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "CO2: ---");
        this->printValue(Peripherals::buffer, 0, 300, EPD_WIDTH / 2, 90, (GFXfont *)&SMALL_TIME_SCREEN_FONT, true, false);

        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "TVOC: ---");
        this->printValue(Peripherals::buffer, EPD_WIDTH / 2, 300, EPD_WIDTH / 2, 90, (GFXfont *)&SMALL_TIME_SCREEN_FONT, true, false);
    }

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "INT: %i", Status::thunderInterferers);
    this->printValue(Peripherals::buffer, 0, 390, EPD_WIDTH / 2, 90, (GFXfont *)&SMALL_TIME_SCREEN_FONT, true, false);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "STK: %i", Status::thunderStrikes);
    this->printValue(Peripherals::buffer, EPD_WIDTH / 2, 390, EPD_WIDTH / 2, 90, (GFXfont *)&SMALL_TIME_SCREEN_FONT, true, false);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "DST: %i km", Status::thunderDistance);
    this->printValue(Peripherals::buffer, 0, 480, EPD_WIDTH / 2, 90, (GFXfont *)&SMALL_TIME_SCREEN_FONT, true, false);

    if (Status::barometricPressure > 0)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "Baro: %i hPa", Status::barometricPressure);
    }
    else
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "Baro: ---", Status::barometricPressure);
    }

    this->printValue(Peripherals::buffer, EPD_WIDTH / 2, 480, EPD_WIDTH / 2, 90, (GFXfont *)&SMALL_TIME_SCREEN_FONT, true, false);

    this->displayFramebuffer();

    this->lastRefreshTime = millis();
}