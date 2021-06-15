#include "DataLog.h"

const char *DataLog::getLogFileName()
{
    return "/data_02.csv";
}

void DataLog::loop()
{
    DIAGNOSTIC("DATALOG,loop")

    File file;

    if (!SPIFFS.exists(this->getLogFileName()))
    {
        DIAGNOSTIC("DATALOG,log not found")
        file = SPIFFS.open(this->getLogFileName(), "w");

        if (!file)
        {
            DIAGNOSTIC("DATALOG,failed to create")
            return;
        }

        file.print("TIME,T,H,CO2,TVOC,CLIQI,CO2QI,TVOCQI,IAQI\n");
        file.close();
    }

    if (!(file = SPIFFS.open(this->getLogFileName(), "a")))
    {
        DIAGNOSTIC("DATALOG,fail")
        return;
    }

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "20%02i-%02i-%02iT%02i:%02i,%0.1f,%d,%d,%d,%d,%d,%d,%d\n",
             Status::getYear(),
             Status::getMonth(),
             Status::getDay(),
             Status::getHour(),
             Status::getMinute(),
             Status::temperature->get(),
             Status::humidity->get(),
             Status::co2->get(),
             Status::tvoc->get(),
             Status::getClimateQI(),
             Status::getCO2QI(),
             Status::getTVOCQI(),
             Status::getIAQI());

    file.print(Peripherals::buffer);
    file.close();

    DIAGNOSTIC("DATALOG,end")
}

void DataLog::dump()
{
    DIAGNOSTIC("DATALOG,DUMP,start")

    File file;

    if (!(file = SPIFFS.open(this->getLogFileName(), "r")))
    {
        DIAGNOSTIC("DATALOG,DUMP,fail")
        return;
    }

    while (file.available())
    {
        Serial.write(file.read());
    }

    file.close();

    DIAGNOSTIC("DATALOG,DUMP,done")
}