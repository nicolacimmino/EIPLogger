#include "DataLog.h"

DataLog *DataLog::dataLog = NULL;

DataLog *DataLog::instance()
{
    if (!DataLog::dataLog)
    {
        DataLog::dataLog = new DataLog();
    }

    return DataLog::dataLog;
}

const char *DataLog::getLogFileNameForDay(uint16_t day)
{
    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "/data_%02d.csv", (day % MAX_LOG_FILES));

    return Peripherals::buffer;
}

void DataLog::setup()
{
    if (!SPIFFS.begin(true))
    {
        DIAGNOSTIC("DATALOG,Mount Failed")
    }        
}

void DataLog::loop()
{
    static unsigned long lastRecordTime = 0;

    DIAGNOSTIC("DATALOG,loop")

    if (lastRecordTime != 0 && millis() - lastRecordTime < 60000)
    {
        DIAGNOSTIC("DATALOG,toosoon")
        return;
    }

    File file;

    if (!SPIFFS.exists(this->getLogFileNameForDay(Status::getDayOfYear())))
    {
        DIAGNOSTIC("DATALOG,log not found")
        file = SPIFFS.open(this->getLogFileNameForDay(Status::getDayOfYear()), "w");

        if (!file)
        {
            DIAGNOSTIC("DATALOG,failed to create")
            return;
        }

        file.print("TIME,T,H,CO2,TVOC,PM1,PM2.5,PM4,PM10\n");
        file.close();
    }

    if (!(file = SPIFFS.open(this->getLogFileNameForDay(Status::getDayOfYear()), "a")))
    {
        DIAGNOSTIC("DATALOG,fail")
        return;
    }

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "20%02i-%02i-%02iT%02i:%02i,%0.1f,%d,%d,%d,%d,%d,%d,%d,%d\n",
             Status::getYear(),
             Status::getMonth(),
             Status::getDay(),
             Status::getHour(),
             Status::getMinute(),
             Status::temperature->get(),
             Status::humidity->get(),
             Status::co2->get(),
             Status::tvoc->get(),
             Status::pm1->get(),
             Status::pm2p5->get(),
             Status::pm4->get(),
             Status::pm10->get(),
             Status::batteryVoltage);

    file.print(Peripherals::buffer);
    file.close();

    if (SPIFFS.exists(this->getLogFileNameForDay(Status::getDayOfYear() + 1)))
    {
        SPIFFS.remove(this->getLogFileNameForDay(Status::getDayOfYear() + 1));
    }

    DIAGNOSTIC("DATALOG,end")
}

void DataLog::dump()
{
    DIAGNOSTIC("DATALOG,DUMP,start")

    File file;

    if (!(file = SPIFFS.open(this->getLogFileNameForDay(Status::getDayOfYear()), "r")))
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

bool DataLog::startRetrieval()
{
    if (!(this->retrievalfile = SPIFFS.open(getLogFileNameForDay(Status::getDayOfYear()), "r")))
    {
        DIAGNOSTIC("DATALOG,RETRIEVAL,fail")
        return false;
    }

    return true;
}

float DataLog::getValue(uint32_t minutesBackSinceNow, uint8_t valueIndex)
{
    uint8_t ix;
    time_t wantedTime = Status::getUnixTime() - (minutesBackSinceNow * 60);

    while (true)
    {
        ix = 0;
        memset(Peripherals::buffer, 0, TEXT_BUFFER_SIZE);

        if (!this->retrievalfile.available())
        {
            return NO_VALUE;
        }

        while (this->retrievalfile.available())
        {

            if (this->retrievalfile.peek() == '\n')
            {
                this->retrievalfile.read();
                break;
            }

            Peripherals::buffer[ix++] = this->retrievalfile.read();

            if (ix == TEXT_BUFFER_SIZE)
            {
                return NO_VALUE;
            }
        }

        char line[TEXT_BUFFER_SIZE];
        memcpy(line, Peripherals::buffer, TEXT_BUFFER_SIZE);

        char *y = strtok(line, "-");
        char *mo = strtok(NULL, "-");
        char *d = strtok(NULL, "T");
        char *h = strtok(NULL, ":");
        char *m = strtok(NULL, ",");

        if (y == NULL || mo == NULL || d == NULL || h == NULL || m == NULL)
        {
            continue;
        }

        // Innacurate on the first day after entering/exiting DST as we assume DST on current time. Good way around would be to log UTC.
        time_t recordTime = Utilities::calculateUnixTime(atoi(y) - 2000, atoi(mo), atoi(d), atoi(h), atoi(m), 0, Status::isDST());

        if (recordTime > wantedTime)
        {
            char *v = strtok(Peripherals::buffer, ",");

            while (valueIndex-- > 0)
            {
                v = strtok(NULL, ",");
            }

            return atof(v);
        }
    }

    return NO_VALUE;
}

void DataLog::stopRetrieval()
{
    if (this->retrievalfile)
    {
        this->retrievalfile.close();
    }
}

void DataLog::showContent()
{
    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "DATALOG,space,%d,%d", SPIFFS.usedBytes(), SPIFFS.totalBytes());
    DIAGNOSTIC(Peripherals::buffer)

    File root = SPIFFS.open("/");

    File file = root.openNextFile();
    while (file)
    {
        snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "DATALOG,ls,%s\t%d bytes", file.name(), file.size());
        DIAGNOSTIC(Peripherals::buffer);

        file.close();
        file = root.openNextFile();
    }

    root.close();
}