#include <Arduino.h>
#include <Wire.h>

#include <FS.h>
#include <SPIFFS.h>

#include "src/Button.h"
#include "src/Peripherals.h"
#include "src/ModeManager.h"
#include "src/ApiServer.h"

ApiServer *apiServer = NULL;

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
    }

    // Peripherals::rtc->set(0, 55, 15, 7, 20, 6, 21);
}

unsigned long lastAppLoopTime = 0;

void appLoop()
{
    PowerManager::loop();
    Peripherals::loop();
    ModeManager::currentDisplay->loop();
    DataLog::instance()->loop();
}

void loop()
{
    if (Status::serverMode)
    {
        if (lastAppLoopTime == 0 || millis() - lastAppLoopTime > 60000)
        {
            lastAppLoopTime = millis();

            appLoop();
        }

        if (!apiServer)
        {
            PowerManager::enterL0();
            apiServer = new ApiServer();
        }

        apiServer->loop();

        return;
    }

    PowerManager::enterL1();

    appLoop();

    PowerManager::enterL2();
}
