#include <Arduino.h>
#include <Wire.h>

#include "esp_adc_cal.h"
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

    ModeManager::currentDisplay->powerDown();
    PowerManager::enterL3();
}

int vref = 1100;

void setup()
{
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        Serial.printf("eFuse Vref:%u mV", adc_chars.vref);
        vref = adc_chars.vref;
    }


    if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER)
    {
        delay(500);

        Serial.begin(115200);

        Peripherals::setup();
        ModeManager::setup();
        Status::setup();

        Peripherals::buttonA->registerOnClickCallback(onButtonAClick);
        Peripherals::buttonA->registerOnLongPressCallback(onButtonALongPress);
        Peripherals::buttonB->registerOnClickCallback(onButtonBClick);
        Peripherals::buttonC->registerOnClickCallback(onButtonCClick);
    }

    PowerManager::enterL1();
    //Peripherals::rtc->set(0, 52, 12, 6, 15, 5, 21);
}

void loop()
{
    PowerManager::loop();
    Peripherals::loop();
    ModeManager::currentDisplay->loop();
    
    Status::loop();

    PowerManager::enterL2();

    PowerManager::enterL1();
}
