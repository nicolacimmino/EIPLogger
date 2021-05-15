# 1 "c:\\Users\\nicola\\git\\EIPLogger\\EIPLogger.ino"
# 2 "c:\\Users\\nicola\\git\\EIPLogger\\EIPLogger.ino" 2
# 3 "c:\\Users\\nicola\\git\\EIPLogger\\EIPLogger.ino" 2

# 5 "c:\\Users\\nicola\\git\\EIPLogger\\EIPLogger.ino" 2
# 6 "c:\\Users\\nicola\\git\\EIPLogger\\EIPLogger.ino" 2
# 7 "c:\\Users\\nicola\\git\\EIPLogger\\EIPLogger.ino" 2
# 8 "c:\\Users\\nicola\\git\\EIPLogger\\EIPLogger.ino" 2

void onButtonPress()
{
    PowerManager::onUserInteratcion();

    ModeManager::currentDisplay->loop();
}

void onButtonAClick()
{
    if (PowerManager::level == 0)
    {
        ModeManager::changeMode();
    }

    onButtonPress();
}

void onButtonBClick()
{
    if (PowerManager::level == 0)
    {
        ModeManager::currentDisplay->onBClick();
    }

    onButtonPress();
}

void onButtonBLongPress()
{
    if (PowerManager::level == 0)
    {
        ModeManager::currentDisplay->onBLongPress();
    }

    onButtonPress();
}

void setup()
{
    Serial.begin(9600);

    Peripherals::setup();

    if (Peripherals::buttonA->isPressed() && Peripherals::buttonB->isPressed())
    {
        DataStore::wipeStoredData();
    }

    // attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_A), buttonPressedISR, FALLING);
    // attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_B), buttonPressedISR, FALLING);

    Peripherals::buttonA->registerOnClickCallback(onButtonAClick);
    Peripherals::buttonB->registerOnClickCallback(onButtonBClick);
    Peripherals::buttonB->registerOnLongPressCallback(onButtonBLongPress);

    ModeManager::setup();
    ModeManager::currentDisplay->clearDisplay();
}

void loop()
{
    PowerManager::loop();
    DataStore::loop();
    Peripherals::buttonA->loop();
    Peripherals::buttonB->loop();
    ModeManager::currentDisplay->loop();

    if (Status::shouldAbortLoop())
    {
        Status::loopAborted();

        return;
    }

    delay(5000);
}
