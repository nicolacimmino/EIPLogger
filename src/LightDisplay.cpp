// #include "LightDisplay.h"

// void LightDisplay::onBClick()
// {
//     this->forceFullDraw();
// }

// void LightDisplay::onBLongPress()
// {
// }

// void LightDisplay::refreshDisplay()
// {    
//     uint16_t r, g, b, a;

//     Peripherals::apds->getColorData(&r, &g, &b, &a);

//     this->printSimpleValue("Temperature", this->getAreaX(0), this->getAreaY(0), this->calculateColorTemperature(r, g, b), "K");

//     this->printSimpleValue("Luminance", this->getAreaX(1), this->getAreaY(1), this->calculateLux(r, g, b), "LUX");

//     this->printTimeValue("Sunrise", this->getAreaX(2), this->getAreaY(2), Status::getSunrise());

//     this->printTimeValue("Sunset", this->getAreaX(3), this->getAreaY(3), Status::getSunset()); 
// }

// uint16_t LightDisplay::getAreaX(uint8_t areaNumber)
// {
//     return 105 + (areaNumber % 2) * 350;
// }

// uint16_t LightDisplay::getAreaY(uint8_t areaNumber)
// {
//     return 50 + ((areaNumber / 2) * 150);
// }

// float LightDisplay::calculateColorTemperature(uint16_t r, uint16_t g, uint16_t b)
// {
//     float x = (-0.14282 * r) + (1.54924 * g) + (-0.95641 * b);
//     float y = (-0.32466 * r) + (1.57837 * g) + (-0.73191 * b);
//     float z = (-0.68202 * r) + (0.77073 * g) + (0.56332 * b);
//     float xchrome = x / (x + y + z);
//     float ychrome = y / (x + y + z);
//     float n = (xchrome - 0.3320) / (0.1858 - ychrome);
//     return (449.0 * pow(n, 3)) + (3525.0 * pow(n, 2)) + (6823.3 * n) + 5520.33;
// }

// float LightDisplay::calculateLux(uint16_t r, uint16_t g, uint16_t b)
// {
//     return (-0.32466 * r) + (1.57837 * g) + (-0.73191 * b);
// }