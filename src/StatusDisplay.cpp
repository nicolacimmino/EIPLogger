#include "StatusDisplay.h"

void StatusDisplay::refreshDisplay()
{
    Paint_ClearWindows(0, 15, 300, 400, WHITE);

    this->printValue("Status", 0, 25, STATUS_DISPLAY_MID_FONT);
}
