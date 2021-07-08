#include "ModeManager.h"

Display *ModeManager::currentDisplay = NULL;
uint8_t ModeManager::mode = 0;

void ModeManager::setup()
{
    mode = 0;
    enterMode();
}

void ModeManager::enterMode()
{
    if (currentDisplay != NULL)
    {
        delete currentDisplay;
    }

    switch (mode)
    {
    case 0:
        currentDisplay = new MainDisplay();
        break;
    case 1:
        currentDisplay = new StatusDisplay();
        break;
    }

    currentDisplay->loop();
}

void ModeManager::changeMode()
{
    mode = (mode + 1) % DISPLAY_MODES;
    enterMode();
}
