
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
    DIAGNOSTIC("PMA,L1");

    if (millis() < 60000 || (Status::getMinute() % 10) == 0)
    {
        sps30_start_measurement();
    }

    digitalWrite(PIN_LED_YELLOW, HIGH);

    PowerManager::level = PS_LEVEL_1;
}

void PowerManager::enterL2()
{
    DIAGNOSTIC("PMA,L2");

    if (millis() > 120000 && (Status::getMinute() % 10) != 0)
    {
        sps30_stop_measurement();
    }

    digitalWrite(PIN_LED_YELLOW, LOW);

    PowerManager::level = PS_LEVEL_2;

    esp_sleep_enable_timer_wakeup(60 * 1000000); // 60s
    esp_light_sleep_start();
}

void PowerManager::enterL3()
{
    DIAGNOSTIC("PMA,L3");

    PowerManager::level = PS_LEVEL_3;

    ModeManager::currentDisplay->showShutdown();

    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    esp_deep_sleep_start();
}

void PowerManager::loop()
{
}

void PowerManager::onUserInteratcion()
{
    lastUserInteractionTime = millis();
}
