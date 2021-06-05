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
        currentDisplay = new LightDisplay();
        break;        
    case 2:
        currentDisplay = new PollenDisplay();
        break;        
    case 3:
        currentDisplay = new ThunderDisplay();
        break;                
    case 4:
        currentDisplay = new StatusDisplay();
        break;
    }
}

void ModeManager::changeMode()
{
    mode = (mode + 1) % DISPLAY_MODES;
    enterMode();
}
