#include "Peripherals.h"

uRTCLib *Peripherals::rtc = NULL;
uEEPROMLib *Peripherals::eeprom = NULL;
Button *Peripherals::buttonA = NULL;
Button *Peripherals::buttonB = NULL;
SHT2xClass *Peripherals::sht2x = NULL;
iAQCoreTwoWire *Peripherals::iaq = NULL;
SparkFun_AS3935 *Peripherals::lightning = NULL;

void Peripherals::setup()
{
    //iAQ-Core can operate at a maximum of 100kHz clock speed
    Wire.setClock(80000L);
    Wire.begin(PIN_SDA, PIN_SCL);
    
    epd_init();
    
    Peripherals::sht2x = new SHT2xClass();
    Peripherals::iaq = new iAQCoreTwoWire(&Wire);
    Peripherals::lightning = new SparkFun_AS3935(AS3935_ADDR);
    Peripherals::buttonA = new Button(PIN_BUTTON_A);
    Peripherals::buttonB = new Button(PIN_BUTTON_B);
    Peripherals::rtc = new uRTCLib(RTC_ADDR);
    Peripherals::eeprom = new uEEPROMLib(EEPROM_ADDR);

    Peripherals::iaq->begin();

    Peripherals::lightning->begin();
    Peripherals::lightning->resetSettings();
    Peripherals::lightning->setIndoorOutdoor(0x12); // Indoor
    Peripherals::lightning->spikeRejection(2);
}