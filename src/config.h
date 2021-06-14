
#ifndef __TEMPERATURE_LOGGER_CONFIG_H__
#define __TEMPERATURE_LOGGER_CONFIG_H__

#define DIAGNOSTIC_ENABLE

#ifdef DIAGNOSTIC_ENABLE
#define DIAGNOSTIC(S)                      \
    Serial.printf("%d,%s\n", millis(), S); \
    delay(100);
#else
#define DIAGNOSTIC(S) // do nothing
#endif

#define AS3935_ADDR 0x01
#define APDS_9960_ADDR 0x39
#define SHT2x_ADDR 0x40
#define IAQ_ADDR 0x5A
#define EEPROM_ADDR 0x57
#define RTC_ADDR 0x68
#define BMP280_ADDR 0x76

#define PIN_BUTTON_A 39
#define PIN_BUTTON_B 34
#define PIN_BUTTON_C 35

#define PIN_BUTTON_A_SEL GPIO_SEL_39

#define PIN_PERIPHERALS_PWR 12
#define PIN_SCL 18
#define PIN_SDA 19

// Increase by 1 to enable to "hidden" status display.
#define DISPLAY_MODES 4

#define LIGHTNING_INT 0x08
#define DISTURBER_INT 0x04
#define NOISE_INT 0x01

#define SMALL_STATUS_SCREEN_FONT &Font8
#define BIG_POWER_DOWN_SCREEN_FONT &Font24

#define MAIN_DISPLAY_MID_FONT &Font16
#define MAIN_DISPLAY_LARGE_FONT &Font24
#define MAIN_DISPLAY_LABEL_FONT &Font12

#define TEXT_BUFFER_SIZE 64

#define NO_VALUE -9999

#define MAX_POLLEN_COUNTS 16

#define PIN_INT 13

#define POWER_SAVE_TIMEOUT_MS 20000

#endif