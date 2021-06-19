#include "Peripherals.h"

SHT2x *Peripherals::sht2x = NULL;
uRTCLib *Peripherals::rtc = NULL;
Button *Peripherals::buttonA = NULL;
Button *Peripherals::buttonB = NULL;
Button *Peripherals::buttonC = NULL;
iAQCoreI2C *Peripherals::iaq = NULL;
UBYTE *Peripherals::framebuffer = NULL;
char *Peripherals::buffer = NULL;

void Peripherals::setup()
{
    DIAGNOSTIC("PERIPHERALS,SETUP")

    Peripherals::buffer = (char *)malloc(TEXT_BUFFER_SIZE);
    memset(Peripherals::buffer, 0, TEXT_BUFFER_SIZE);
    
    DEV_Module_Init();
    EPD_4IN2_Init();
    EPD_4IN2_Clear();
    DEV_Delay_ms(500);
    EPD_4IN2_Sleep();

    Peripherals::framebuffer = (UBYTE *)malloc((EPD_4IN2_WIDTH / 8 + 1) * EPD_4IN2_HEIGHT);
    Paint_NewImage(Peripherals::framebuffer, EPD_4IN2_WIDTH, EPD_4IN2_HEIGHT, 90, WHITE);
    Paint_SelectImage(Peripherals::framebuffer);
    Paint_Clear(WHITE);

    Wire.begin(PIN_SDA, PIN_SCL);

    //iAQ-Core can operate at a maximum of 100kHz clock speed
    Wire.setClock(80000L);

    Peripherals::sht2x = new SHT2x();
    Peripherals::iaq = new iAQCoreI2C();
    Peripherals::buttonA = new Button(PIN_BUTTON_A);
    Peripherals::buttonB = new Button(PIN_BUTTON_B);
    Peripherals::buttonC = new Button(PIN_BUTTON_C);
    Peripherals::rtc = new uRTCLib(RTC_ADDR);

    Peripherals::iaq->begin();
}

void Peripherals::connectWiFi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

bool Peripherals::isWiFiConnected()
{
    static unsigned long lastReconnectAttempt = millis();

    if (WiFi.status() != WL_CONNECTED && millis() - lastReconnectAttempt > 10000)
    {
        WiFi.disconnect();
        WiFi.reconnect();
    }

    return WiFi.status() == WL_CONNECTED;
}

void Peripherals::disconnectWiFi()
{
    WiFi.disconnect();
}

void Peripherals::loop()
{
    Peripherals::buttonA->loop();
    Peripherals::buttonB->loop();
    Peripherals::buttonC->loop();

    Peripherals::sht2x->loop();
    Status::temperature->set(sht2x->temperature);
    Status::humidity->set(sht2x->humidity);

    Peripherals::rtc->refresh();

    Wire.setClock(80000L);
    if (Peripherals::iaq->hasValue() && Peripherals::iaq->isValid())
    {
        Status::co2->set(Peripherals::iaq->getCO2());
        Status::tvoc->set(Peripherals::iaq->getTVOC());
    }        
}