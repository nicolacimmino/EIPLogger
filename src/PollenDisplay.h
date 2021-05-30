
#ifndef __POLLEN_DISPLAY_H__
#define __POLLEN_DISPLAY_H__

#include "Display.h"
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "icons/trend_uu.h"
#include "icons/trend_u.h"
#include "icons/trend_dd.h"
#include "icons/trend_d.h"
#include "icons/trend_eq.h"
#include "icons/trend_end.h"

class PollenDisplay : public Display
{
private:
    uint16_t getAreaX(uint8_t areaNumber);
    uint16_t getAreaY(uint8_t areaNumber);
    void printPollen(int pollenId, uint16_t x, uint16_t y, int value, const char *trend);
    
public:
    void loop();
    void onBClick();
    void onBLongPress();
};

#endif
