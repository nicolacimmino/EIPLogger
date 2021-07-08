#include "StatusDisplay.h"

void StatusDisplay::refreshDisplay()
{
    Paint_ClearWindows(0, 15, 300, 400, WHITE);

    uint8_t line = 0;

    this->printValue("Status", 0, this->getLineY(line++), STATUS_DISPLAY_MID_FONT);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "IP:      %s", Status::serverMode ? WiFi.localIP().toString().c_str() : "---");
    this->printValue(Peripherals::buffer, 0, this->getLineY(line++), STATUS_DISPLAY_MID_FONT);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "Battery: %d%%", Status::getBatteryLevel());
    this->printValue(Peripherals::buffer, 0, this->getLineY(line++), STATUS_DISPLAY_MID_FONT);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "Uptime:  %d min", (int)millis() / 60000);
    this->printValue(Peripherals::buffer, 0, this->getLineY(line++), STATUS_DISPLAY_MID_FONT);
}

uint16_t StatusDisplay::getLineY(uint8_t line)
{
    return 25 + line * 15;
}
