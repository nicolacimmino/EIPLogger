#include <Arduino.h>
#include <Wire.h>

#include "src/Button.h"
#include "src/Peripherals.h"
#include "src/ModeManager.h"

void onButtonPress()
{
    PowerManager::onUserInteratcion();

    ModeManager::currentDisplay->loop();
}

void onButtonAClick()
{
    DIAGNOSTIC("BTN,A");

    if (Status::locked)
    {
        Status::locked = false;
        ModeManager::currentDisplay->setLockedIcon(Status::locked);

        onButtonPress();
        
        return;
    }

    ModeManager::changeMode();
    onButtonPress();
}

void onButtonBClick()
{
    DIAGNOSTIC("BTN,B");

    ModeManager::currentDisplay->onBClick();
    onButtonPress();
}

void onButtonCClick()
{
    DIAGNOSTIC("BTN,C");

    onButtonPress();
}

void onButtonALongPress()
{
    DIAGNOSTIC("BTN,AL");

    // ModeManager::currentDisplay->powerDown();
    // PowerManager::enterL3();
}

int vref = 1100;
volatile bool interrupt = false;

void ISR()
{
    interrupt = true;
}

void setup()
{
    DIAGNOSTIC("SETUP")

    if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER)
    {
        delay(500);

        // pinMode(PIN_INT, INPUT);
        // attachInterrupt(digitalPinToInterrupt(PIN_INT), ISR, RISING);

        Serial.begin(115200);

        Peripherals::setup();        
        ModeManager::setup();
        Status::setup();

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
    Peripherals::loop();
    ModeManager::currentDisplay->loop();

    Status::loop();

    PowerManager::enterL1();

    if (interrupt)
    {
        Status::readThunderEvent();

        interrupt = false;
    }

    //PowerManager::enterL2();
}
