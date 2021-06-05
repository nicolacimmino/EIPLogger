
#include "PowerManager.h"
#include "ModeManager.h"

unsigned long PowerManager::lastUserInteractionTime = millis();
uint8_t PowerManager::level = PS_LEVEL_0;
uint8_t PowerManager::previousLevel = PS_LEVEL_0;

void PowerManager::enterL0()
{
    DIAGNOSTIC("PMA,L0")

    PowerManager::level = PS_LEVEL_0;
    Peripherals::connectWiFi();    
}

void PowerManager::enterL1()
{
    DIAGNOSTIC("PMA,L1");

    PowerManager::level = PS_LEVEL_1;
    Peripherals::disconnectWiFi();
}

void PowerManager::enterL2()
{
    DIAGNOSTIC("PMA,L2");

    PowerManager::level = PS_LEVEL_2;
    esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_BUTTON_B, 0);
    esp_sleep_enable_ext1_wakeup(PIN_BUTTON_A_SEL, ESP_EXT1_WAKEUP_ALL_LOW);
    esp_sleep_enable_timer_wakeup(60 * 1000000); // 60s
    esp_light_sleep_start();
}

void PowerManager::enterL3()
{
    DIAGNOSTIC("PMA,L3");

    PowerManager::level = PS_LEVEL_3;
    epd_poweroff_all();    
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    esp_sleep_enable_ext1_wakeup(PIN_BUTTON_A_SEL, ESP_EXT1_WAKEUP_ALL_LOW);
    esp_deep_sleep_start();
}

void PowerManager::loop()
{
    // if (level > PS_LEVEL_0)
    // {
    //     return;
    // }

    // if (millis() - lastUserInteractionTime > POWER_SAVE_TIMEOUT_MS)
    // {
    //     enterL2();
    // }
}

void PowerManager::onUserInteratcion()
{
    // lastUserInteractionTime = millis();

    // if (level != PS_LEVEL_0)
    // {
    //     enterL0();

    //     ModeManager::currentDisplay->onDisplayAwaken();
    // }
}

void PowerManager::restoreLevel()
{
    // if (previousLevel != level)
    // {
    //     switch (previousLevel)
    //     {
    //     case (PS_LEVEL_0):
    //         enterL0();
    //         break;
    //     case (PS_LEVEL_1):
    //         enterL1();
    //         break;
    //     case (PS_LEVEL_2):
    //         enterL2();
    //         break;
    //     }
    // }
}
