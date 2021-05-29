
#ifndef __MAIN_DISPLAY_H__
#define __MAIN_DISPLAY_H__

#include "Display.h"
#include "icons/thunder.h"

class MainDisplay : public Display
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
