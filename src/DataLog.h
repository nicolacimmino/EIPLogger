
#ifndef __DATALOG_H__
#define __DATALOG_H__

#include <FS.h>
#include <SPIFFS.h>
#include <time.h>
#include "config.h"
#include "Peripherals.h"
#include "Status.h"

class DataLog
{
private:
    const char *getLogFileName();
    File retrievalfile;

    time_t calculateUnixTime(uint8_t y, uint8_t mo, uint8_t d, uint8_t h, uint8_t m, uint8_t s, bool dst)
    {
        struct tm time;

        time.tm_sec = s;
        time.tm_min = m;
        time.tm_hour = h;
        time.tm_mday = d;
        time.tm_mon = mo - 1;
        time.tm_year = (100 + y); // We expect decade+year so we need +2000 and -1900 for the epoch => +100
        time.tm_isdst = dst ? 1 : 0;

        return mktime(&time);
    }

protected:
public:
    void loop();
    void dump();
    bool startRetrieval();
    float getValue(uint32_t minutesBackSinceNow, uint8_t valueIndex);
    void stopRetrieval();
};

#endif