
#include "PowerManager.h"
#include "ModeManager.h"

unsigned long PowerManager::lastUserInteractionTime = millis();
uint8_t PowerManager::level = PS_LEVEL_0;
uint8_t PowerManager::previousLevel = PS_LEVEL_0;

bool PowerManager::enterL0()
{
    DIAGNOSTIC("PMA,L0,attempt")

    uint8_t previousLevel = PowerManager::level;

    PowerManager::level = PS_LEVEL_0;
    Peripherals::connectWiFi();

    uint8_t count = 0;
    while (!Peripherals::isWiFiConnected() && count < 40)
    {
        count++;
        DIAGNOSTIC("PMA,L0,wait")
        ModeManager::currentDisplay->loop();
        delay(500);
    }

    if (!Peripherals::isWiFiConnected())
    {
        DIAGNOSTIC("PMA,L0,failed")

        PowerManager::level = previousLevel;

        return false;
    }

    return true;
}

void PowerManager::enterL1()
{
    if (PowerManager::level == PS_LEVEL_1)
    {
        return;
    }

    DIAGNOSTIC("PMA,L1");

    PowerManager::level = PS_LEVEL_1;

    // Peripherals::disconnectWiFi();
    // while (Peripherals::isWiFiConnected())
    // {
    //     delay(10);
    // }

    // ModeManager::currentDisplay->loop();
}

void PowerManager::enterL2()
{
    if (PowerManager::level == PS_LEVEL_2)
    {
        return;
    }

    DIAGNOSTIC("PMA,L2");
    
    PowerManager::level = PS_LEVEL_2;

    //esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_BUTTON_B, 0);
  //  esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_INT, 1);
  //  esp_sleep_enable_ext1_wakeup(PIN_BUTTON_A_SEL, ESP_EXT1_WAKEUP_ALL_LOW);
    esp_sleep_enable_timer_wakeup(60 * 1000000); // 60s
    esp_light_sleep_start();
}

void PowerManager::enterL3()
{
    DIAGNOSTIC("PMA,L3");

    PowerManager::level = PS_LEVEL_3;
    digitalWrite(PIN_PERIPHERALS_PWR, LOW);
 //   esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
 //   esp_sleep_enable_ext1_wakeup(PIN_BUTTON_A_SEL, ESP_EXT1_WAKEUP_ALL_LOW);
    esp_deep_sleep_start();
}

void PowerManager::loop()
{
    if (millis() - lastUserInteractionTime > POWER_SAVE_TIMEOUT_MS)
    {
        enterL2();
    }
}

void PowerManager::onUserInteratcion()
{
    lastUserInteractionTime = millis();
}
