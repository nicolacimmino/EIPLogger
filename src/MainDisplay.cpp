#include "MainDisplay.h"

void MainDisplay::onBClick()
{
    this->lastRefreshTime = 0;
}

void MainDisplay::onBLongPress()
{
}

void MainDisplay::loop()
{
    if (this->lastRefreshTime != 0 && millis() - this->lastRefreshTime < 60000)
    {
        return;
    }

    this->lastRefreshTime = millis();

#define MD_TOP_MARGIN 8
#define MD_TOP_X 0
#define MD_TOP_Y 0
#define MD_TOP_WIDTH EPD_WIDTH
#define MD_TOP_HEIGHT 50

#define MD_TEMP_HUM_TOP_X 0
#define MD_TEMP_HUM_TOP_Y 50
#define MD_TEMP_HUM_WIDTH EPD_WIDTH / 3
#define MD_TEMP_HUM_HEIGHT 110

    //#define MAIN_DISPLAY_SMALL_FONT_PX_V 30

    // Top bar
    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "%02i-%02i-%02i %02i:%02i", Peripherals::rtc->day(), Peripherals::rtc->month(), Peripherals::rtc->year(), Peripherals::rtc->hour(), Peripherals::rtc->minute());
    this->printValue(Peripherals::buffer, MD_TOP_X, MD_TOP_Y, EPD_WIDTH, MD_TOP_HEIGHT, (GFXfont *)&MAIN_DISPLAY_SMALL_FONT);
    epd_draw_hline(MD_TOP_X, MD_TOP_Y + MD_TOP_HEIGHT, MD_TOP_WIDTH, 0, Peripherals::framebuffer);

    this->printVHLValue(0, 50, DIS_NONE, Peripherals::sht2x->temperature, "C", 21.3, 29.6);
    this->printVHLValue(0, 160, DIS_NO_DECIMAL, Peripherals::sht2x->humidity, "%RH", 45, 85);

    this->printVHLValue(400, 50, DIS_NO_DECIMAL | DIS_LARGE_VALUE, Status::co2, "ppm", 450, 6700);
    this->printVHLValue(400, 160, DIS_NO_DECIMAL | DIS_LARGE_VALUE, Status::tvoc, "ppb", 100, 640);
    this->printVHLValue(400, 270, DIS_NO_DECIMAL | DIS_LARGE_VALUE, Status::barometricPressure, "HPa", 940, 1007);

    this->displayFramebuffer();
}
