#include "Peripherals.h"

SHT2x *Peripherals::sht2x = NULL;
uRTCLib *Peripherals::rtc = NULL;
Button *Peripherals::buttonA = NULL;
Button *Peripherals::buttonB = NULL;
Button *Peripherals::buttonC = NULL;
Button *Peripherals::buttonD = NULL;
iAQCoreI2C *Peripherals::iaq = NULL;
UBYTE *Peripherals::framebuffer = NULL;
char *Peripherals::buffer = NULL;
ApiServer *Peripherals::apiServer = NULL;

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

    sensirion_i2c_init();

    sps30_set_fan_auto_cleaning_interval_days(4);

    sps30_start_measurement();

    //iAQ-Core can operate at a maximum of 100kHz clock speed
    Wire.setClock(80000L);

    Peripherals::sht2x = new SHT2x();
    Peripherals::iaq = new iAQCoreI2C();
    Peripherals::buttonA = new Button(PIN_BUTTON_A);
    Peripherals::buttonB = new Button(PIN_BUTTON_B);
    Peripherals::buttonC = new Button(PIN_BUTTON_C);
    Peripherals::buttonD = new Button(PIN_BUTTON_D);
    Peripherals::rtc = new uRTCLib(RTC_ADDR);

    Peripherals::iaq->begin();

    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_YELLOW, OUTPUT);
    pinMode(PIN_LED_BLUE, OUTPUT);
    pinMode(PIN_SWITCH_SENSE, INPUT_PULLDOWN);

    ledcSetup(PWM_CHANNEL_RED_LED, 4000, 8);
    ledcAttachPin(PIN_LED_RED, PWM_CHANNEL_RED_LED);

    ledcSetup(PWM_CHANNEL_BLUE_LED, 4000, 8);
    ledcAttachPin(PIN_LED_BLUE, PWM_CHANNEL_BLUE_LED);

    Peripherals::setRedLed(true);
    Peripherals::setYellowLed(true);

    Status::serverMode = digitalRead(PIN_SWITCH_SENSE);
    Peripherals::setBlueLed(Status::serverMode);
}

void Peripherals::setRedLed(bool on)
{
    ledcWrite(PWM_CHANNEL_RED_LED, on ? 15 : 0);
}

void Peripherals::setYellowLed(bool on)
{
    digitalWrite(PIN_LED_YELLOW, on);
}

void Peripherals::setBlueLed(bool on)
{
    ledcWrite(PWM_CHANNEL_BLUE_LED, on ? 5 : 0);
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
    Peripherals::buttonD->loop();

    Peripherals::sht2x->loop();
    Status::temperature->set(sht2x->temperature);
    Status::humidity->set(sht2x->humidity);

    Peripherals::rtc->refresh();

    if (millis() < 60000 || (Status::getMinute() % 10) == 0)
    {
        sps30_start_measurement();
    }

    uint16_t ready = false;
    sps30_read_data_ready(&ready);

    if (ready)
    {
        DIAGNOSTIC("PM,READY")

        struct sps30_measurement m;
        sps30_read_measurement(&m);

        Status::pm1->set(m.mc_1p0);
        Status::pm2p5->set(m.mc_2p5);
        Status::pm4->set(m.mc_4p0);
        Status::pm10->set(m.mc_10p0);
    }

    if (millis() > 120000 && (Status::getMinute() % 10) > 0)
    {
        sps30_stop_measurement();
    }

    Wire.setClock(80000L);
    if (Peripherals::iaq->hasValue() && Peripherals::iaq->isValid())
    {
        Status::co2->set(Peripherals::iaq->getCO2());
        Status::tvoc->set(Peripherals::iaq->getTVOC());
    }

    Status::batteryVoltage = analogRead(PIN_VBAT_SENSE) * 1.77;

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "BAT,%d", Status::batteryVoltage);
    DIAGNOSTIC(Peripherals::buffer);

    if (Status::batteryVoltage != 0 && Status::batteryVoltage < 3000)
    {
        PowerManager::enterL3();
    }
}