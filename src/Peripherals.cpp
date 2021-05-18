#include "Peripherals.h"

SHT2x *Peripherals::sht2x = NULL;
uRTCLib *Peripherals::rtc = NULL;
uEEPROMLib *Peripherals::eeprom = NULL;
Button *Peripherals::buttonA = NULL;
Button *Peripherals::buttonB = NULL;
Button *Peripherals::buttonC = NULL;
iAQCoreTwoWire *Peripherals::iaq = NULL;
SparkFun_AS3935 *Peripherals::lightning = NULL;
BMP280_DEV *Peripherals::bmp280 = NULL;
uint8_t *Peripherals::framebuffer = NULL;

void Peripherals::setup()
{
    Peripherals::framebuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
    memset(Peripherals::framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);

    epd_init();

    delay(500);

    //iAQ-Core can operate at a maximum of 100kHz clock speed
    Wire.setClock(70000L);
    Wire.begin(PIN_SDA, PIN_SCL);

    Peripherals::sht2x = new SHT2x();
    Peripherals::iaq = new iAQCoreTwoWire(&Wire);
    Peripherals::lightning = new SparkFun_AS3935(AS3935_ADDR);
    Peripherals::buttonA = new Button(PIN_BUTTON_A);
    Peripherals::buttonB = new Button(PIN_BUTTON_B);
    Peripherals::buttonC = new Button(PIN_BUTTON_C);
    Peripherals::rtc = new uRTCLib(RTC_ADDR);
    Peripherals::eeprom = new uEEPROMLib(EEPROM_ADDR);
    Peripherals::bmp280 = new BMP280_DEV();
    
    Peripherals::lightning->begin();
    Peripherals::lightning->resetSettings();
    Peripherals::lightning->setIndoorOutdoor(0x12); // Indoor
    Peripherals::lightning->spikeRejection(2);

    Peripherals::bmp280->begin(BMP280_ADDR);
    Peripherals::bmp280->setTimeStandby(TIME_STANDBY_2000MS);
    Peripherals::bmp280->startNormalConversion();

    Peripherals::iaq->begin();
}

void Peripherals::loop()
{
    Peripherals::buttonA->loop();
    Peripherals::buttonB->loop();
    Peripherals::buttonC->loop();

    uint8_t intVal = Peripherals::lightning->readInterruptReg();
    if (intVal)
    {
        if (intVal == DISTURBER_INT)
        {
            Status::thunderInterferers++;
        }
        else if (intVal == LIGHTNING_INT)
        {
            Status::thunderStrikes++;
            Status::thunderEnergy = Peripherals::lightning->lightningEnergy();
            Status::thunderDistance = Peripherals::lightning->distanceToStorm();
        }

        while (Peripherals::lightning->readInterruptReg())
        {
            delay(1);
        }
    }

    float pressure;
    if(Peripherals::bmp280->getPressure(pressure)) {
        Status::barometricPressure = pressure;
    }

    Peripherals::sht2x->loop();
}