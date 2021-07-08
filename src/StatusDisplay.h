
#ifndef __STATUS_DISPLAY_H__
#define __STATUS_DISPLAY_H__

#include "Display.h"

class StatusDisplay : public Display
{
protected:
    void refreshDisplay();

private:
    uint16_t getLineY(uint8_t line);

public:
};

#endif
