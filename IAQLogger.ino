#include <Arduino.h>
#include <Wire.h>

#include <FS.h>
#include <SPIFFS.h>

#include "src/Button.h"
#include "src/Peripherals.h"
#include "src/ModeManager.h"
#include "src/DataLog.h"

DataLog dataLog;

void onButtonPress()
{
    PowerManager::onUserInteratcion();

    ModeManager::currentDisplay->loop();
}

void onButtonAClick()
{
    DIAGNOSTIC("BTN,A");

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

void setup()
{
    DIAGNOSTIC("SETUP")

    if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER)
    {
        if (!SPIFFS.begin(true))
        {
            Serial.println("SPIFFS Mount Failed");
            return;
        }

        delay(500);

        Serial.begin(115200);

        //dataLog.dump();

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
    PowerManager::enterL1();

    PowerManager::loop();
    Peripherals::loop();
    ModeManager::currentDisplay->loop();

    Status::loop();
    dataLog.loop();

    printf("seconds since the Epoch: %ld\n", (long)Status::getUnixTime());

    dataLog.startRetrieval();
    Serial.println(dataLog.getValue(60 * 10, 1));
    Serial.println(dataLog.getValue(60, 2));
    Serial.println(dataLog.getValue(10, 3));
    Serial.println(dataLog.getValue(1, 4));
    dataLog.stopRetrieval();

    PowerManager::enterL2();
}
