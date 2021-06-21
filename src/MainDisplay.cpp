#include "MainDisplay.h"

void MainDisplay::onBClick()
{    
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
                             "L:", Status::temperature->getMin(),
                             "H:", Status::temperature->getMax());

    this->printLabelledValue("Humidity",
                             this->getAreaX(2), this->getAreaY(2),
                             DIS_NO_DECIMAL,
                             Status::humidity->get(), "%RH",                             
                             "L:", Status::humidity->getMin(),
                             "H:", Status::humidity->getMax());

    this->printLabelledValue("CO2",
                             this->getAreaX(4), this->getAreaY(4),
                             DIS_NO_DECIMAL | DIS_LARGE_VALUE,
                             Status::co2->get(), "ppm",
                             NULL, 0,
                             "L:", Status::co2->getMin(),
                             "H:", Status::co2->getMax());

    this->printLabelledValue("TVOC",
                             this->getAreaX(6), this->getAreaY(6),
                             DIS_NO_DECIMAL | DIS_LARGE_VALUE,
                             Status::tvoc->get(), "ppb",
                             NULL, 0,
                             "L:", Status::tvoc->getMin(),
                             "H:", Status::tvoc->getMax());

    this->printLabelledValue("IAQI",
                             this->getAreaX(8), this->getAreaY(8),
                             DIS_NO_DECIMAL,
                             Status::getIAQI(), "",
                             "CO2:  ", Status::getCO2QI(),
                             "TVOC: ", Status::getTVOCQI(),
                             "CLI:  ", Status::getClimateQI(),
                             "PM:   ", Status::getPMAQI(),
                             NULL, 0, true);

    this->printLabelledValue("PM",
                             this->getAreaX(9), this->getAreaY(9),
                             DIS_NO_DECIMAL,
                             Status::getPMAQI(), "",                             
                             "PM1:   ", Status::pm1->get(),
                             "PM2.5: ", Status::pm2p5->get(),
                             "PM4:   ", Status::pm4->get(),
                             "PM10:  ", Status::pm10->get(),
                             NULL, 0,
                             true);

    this->plotGraph("", this->getAreaX(1), this->getAreaY(1), 24 * 60, 1, 40);
    this->plotGraph("", this->getAreaX(3), this->getAreaY(3), 24 * 60, 2, 100);
    this->plotGraph("", this->getAreaX(5), this->getAreaY(5), 24 * 60, 3, 4000);
    this->plotGraph("", this->getAreaX(7), this->getAreaY(7), 24 * 60, 4, 2000);
}

uint16_t MainDisplay::getAreaX(uint8_t areaNumber)
{
    return (areaNumber % 2) * 150;
}

uint16_t MainDisplay::getAreaY(uint8_t areaNumber)
{
    return 15 + ((areaNumber / 2) * 76);
}