
#ifndef __DATALOG_H__
#define __DATALOG_H__

#include <FS.h>
#include <SPIFFS.h>
#include "config.h"
#include "Peripherals.h"
#include "Status.h"
#include "Utilities.h"

class DataLog
{
private:
    const char *getLogFileNameForDay(uint16_t day);
    File retrievalfile;
    static DataLog *dataLog;

protected:
public:
    void loop();
    void setup();
    void dump();
    bool startRetrieval();
    float getValue(uint32_t minutesBackSinceNow, uint8_t maxDelta, uint8_t valueIndex);
    void stopRetrieval();
    static DataLog *instance();
    void showContent();
};

#endif