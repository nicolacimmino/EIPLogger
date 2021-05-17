#include <Arduino.h>
#include <Wire.h>

#include "src/Button.h"
#include "src/Peripherals.h"
#include "src/ModeManager.h"
#include "src/DataStore.h"

void onButtonPress()
{
    PowerManager::onUserInteratcion();

    ModeManager::currentDisplay->loop();
}

void onButtonAClick()
{
    Serial.println("A");
    // if (PowerManager::level == PS_LEVEL_0)
    // {
    //     ModeManager::changeMode();
    // }

    // onButtonPress();
}

void onButtonBClick()
{
    Serial.println("B");
    // if (PowerManager::level == PS_LEVEL_0)
    // {
    //     ModeManager::currentDisplay->onBClick();
    // }

    // onButtonPress();
}

void onButtonCClick()
{
    Serial.println("C");
    // if (PowerManager::level == PS_LEVEL_0)
    // {
    //     ModeManager::currentDisplay->onBClick();
    // }

    // onButtonPress();
}

void onButtonALongPress()
{
    PowerManager::enterPOFF();
}

void setup()
{
    Serial.begin(115200);

    if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER)
    {
        Peripherals::setup();
    }

    // if (Peripherals::buttonA->isPressed() && Peripherals::buttonB->isPressed())
    // {
    //     DataStore::wipeStoredData();
    // }

    // attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_A), buttonPressedISR, FALLING);
    // attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_B), buttonPressedISR, FALLING);

    Peripherals::buttonA->registerOnClickCallback(onButtonAClick);
    Peripherals::buttonA->registerOnLongPressCallback(onButtonALongPress);
    Peripherals::buttonB->registerOnClickCallback(onButtonBClick);
    Peripherals::buttonC->registerOnClickCallback(onButtonCClick);

    ModeManager::setup();
    // epd_poweron();
    // ModeManager::currentDisplay->clearDisplay();
    // epd_poweroff();

    //Peripherals::rtc->set(0, 52, 12, 6, 15, 5, 21);
}

void loop()
{
    PowerManager::loop();
    DataStore::loop();
    Peripherals::loop();
    ModeManager::currentDisplay->loop();

    if (Status::shouldAbortLoop())
    {
        Status::loopAborted();

        return;
    }

    esp_sleep_enable_ext1_wakeup(POWER_MANAGER_WAKEUP_PINS, ESP_EXT1_WAKEUP_ALL_LOW);
    esp_sleep_enable_timer_wakeup(60 * 1000000); // 60s
    esp_light_sleep_start();
}
