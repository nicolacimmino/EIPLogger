#include "MainDisplay.h"

void MainDisplay::onBClick()
{
    this->forceFullDraw();
}

void MainDisplay::onBLongPress()
{
}

void MainDisplay::refreshDisplay()
{        
    this->printLabelledValue("Temperature",
                             this->getAreaX(0),
                             this->getAreaY(0),
                             DIS_NONE,
                             Status::temperature->get(), "C",
                             "L", Status::temperature->getMin(),
                             "H", Status::temperature->getMax());

    this->printLabelledValue("Humidity",
                             this->getAreaX(2), this->getAreaY(2),
                             DIS_NO_DECIMAL,
                             Status::humidity->get(), "%RH",
                             "L", Status::humidity->getMin(),
                             "H", Status::humidity->getMax());

    this->printLabelledValue("CO2",
                             this->getAreaX(1), this->getAreaY(1),
                             DIS_NO_DECIMAL | DIS_LARGE_VALUE,
                             Status::co2->get(), "ppm",
                             "L", Status::co2->getMin(),
                             "H", Status::co2->getMax());

    this->printLabelledValue("TVOC",
                             this->getAreaX(3), this->getAreaY(3),
                             DIS_NO_DECIMAL | DIS_LARGE_VALUE,
                             Status::tvoc->get(), "ppb",
                             "L", Status::tvoc->getMin(),
                             "H", Status::tvoc->getMax());

    this->printLabelledValue("IAQI",
                             this->getAreaX(4), this->getAreaY(4),
                             DIS_NO_DECIMAL,
                             Status::getIAQI(), "",
                             "CO2", Status::getCO2QI(),
                             "TVOC", Status::getTVOCQI(),
                             "CLI", Status::getClimateQI());       
}

uint16_t MainDisplay::getAreaX(uint8_t areaNumber)
{
    return 10 + (areaNumber % 2) * 200;
}

uint16_t MainDisplay::getAreaY(uint8_t areaNumber)
{
    return 50 + ((areaNumber / 2) * 80);
}