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
    ModeManager::changeMode();
    onButtonPress();

    Serial.println("A");
    // if (PowerManager::level == PS_LEVEL_0)
    // {
    //     ModeManager::changeMode();
    // }
}

void onButtonBClick()
{
    Serial.println("B");
    // if (PowerManager::level == PS_LEVEL_0)
    // {
    ModeManager::currentDisplay->onBClick();
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
    ModeManager::currentDisplay->powerDown();
    PowerManager::enterL3();
}

void setup()
{
    Serial.begin(115200);

    if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER)
    {
        Peripherals::setup();
        ModeManager::setup();

        Peripherals::buttonA->registerOnClickCallback(onButtonAClick);
        Peripherals::buttonA->registerOnLongPressCallback(onButtonALongPress);
        Peripherals::buttonB->registerOnClickCallback(onButtonBClick);
        Peripherals::buttonC->registerOnClickCallback(onButtonCClick);
    }

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

    PowerManager::enterL2();
}
