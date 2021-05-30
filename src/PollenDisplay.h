
#ifndef __POLLEN_DISPLAY_H__
#define __POLLEN_DISPLAY_H__

#include "Display.h"
#include <HTTPClient.h>
#include <Arduino_JSON.h>

class PollenDisplay : public Display
{
private:
    uint16_t getAreaX(uint8_t areaNumber);
    uint16_t getAreaY(uint8_t areaNumber);

public:
    void loop();
    void onBClick();
    void onBLongPress();
};

#endif
