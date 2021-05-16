
#include "PowerManager.h"
#include "ModeManager.h"

unsigned long PowerManager::lastUserInteractionTime = millis();
uint8_t PowerManager::level = PS_LEVEL_0;
uint8_t PowerManager::previousLevel = PS_LEVEL_0;

void PowerManager::enterL0()
{
    // previousLevel = level;

    // digitalWrite(PIN_PWR_AUX_DEVS, HIGH);
    // delay(PS_BUS_GUARD_TIME_MS);

    level = PS_LEVEL_0;
}

void PowerManager::enterL1()
{
    previousLevel = level;

    // digitalWrite(PIN_PWR_AUX_DEVS, HIGH);
    // delay(PS_BUS_GUARD_TIME_MS);

    //    level = PS_LEVEL_1;
    level = PS_LEVEL_0;
    //
}

void PowerManager::enterL2()
{
    previousLevel = level;

    //digitalWrite(PIN_PWR_AUX_DEVS, LOW);

    //level = PS_LEVEL_2;
    level = PS_LEVEL_0;
    //
}

void PowerManager::enterPOFF()
{
    ModeManager::currentDisplay->powerDown();
    epd_poweroff_all();          
    esp_sleep_enable_ext1_wakeup(POWER_MANAGER_WAKEUP_PINS, ESP_EXT1_WAKEUP_ALL_LOW);
    esp_deep_sleep_start();
}

void PowerManager::loop()
{
    if (level > PS_LEVEL_0)
    {
        return;
    }

    if (millis() - lastUserInteractionTime > POWER_SAVE_TIMEOUT_MS)
    {
        enterL2();
    }
}

void PowerManager::onUserInteratcion()
{
    lastUserInteractionTime = millis();

    if (level != PS_LEVEL_0)
    {
        enterL0();

        ModeManager::currentDisplay->onDisplayAwaken();
    }
}

void PowerManager::restoreLevel()
{
    if (previousLevel != level)
    {
        switch (previousLevel)
        {
        case (PS_LEVEL_0):
            enterL0();
            break;
        case (PS_LEVEL_1):
            enterL1();
            break;
        case (PS_LEVEL_2):
            enterL2();
            break;
        }
    }
}
