#include <Arduino.h>
#include <Wire.h>

#include <FS.h>
#include <SPIFFS.h>

#include "src/Button.h"
#include "src/Peripherals.h"
#include "src/ModeManager.h"
#include "src/ApiServer.h"

ApiServer *apiServer;

void onButtonPress()
{
    PowerManager::onUserInteratcion();
}

void onButtonAClick()
{
    DIAGNOSTIC("BTN,A");

    onButtonPress();
}

void onButtonBClick()
{
    DIAGNOSTIC("BTN,B");

    onButtonPress();
}

void onButtonCClick()
{
    DIAGNOSTIC("BTN,C");

    onButtonPress();
}

void onButtonDClick()
{
    DIAGNOSTIC("BTN,D");

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
        delay(500);

        Serial.begin(115200);

        DataLog::instance()->setup();
        Peripherals::setup();
        Peripherals::loop();
        ModeManager::setup();

        Peripherals::buttonA->registerOnClickCallback(onButtonAClick);
        Peripherals::buttonA->registerOnLongPressCallback(onButtonALongPress);
        Peripherals::buttonB->registerOnClickCallback(onButtonBClick);
        Peripherals::buttonC->registerOnClickCallback(onButtonCClick);
        Peripherals::buttonD->registerOnClickCallback(onButtonDClick);

        PowerManager::enterL1();

        DataLog::instance()->showContent();
        
        if (Status::serverMode)
        {
            PowerManager::enterL0();
            apiServer = new ApiServer();
        }
    }

    // Peripherals::rtc->set(0, 55, 15, 7, 20, 6, 21);
}

void loop()
{
    if (apiServer)
    {
        apiServer->loop();
    }

    if (!Status::serverMode)
    {
        PowerManager::enterL1();
    }

    PowerManager::loop();
    Peripherals::loop();
    ModeManager::currentDisplay->loop();

    DataLog::instance()->loop();

    if (!Status::serverMode)
    {
        PowerManager::enterL2();
    }
}
