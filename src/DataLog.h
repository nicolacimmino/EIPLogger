
#ifndef __DATALOG_H__
#define __DATALOG_H__

#include <FS.h>
#include <SPIFFS.h>
#include "config.h"
#include "Peripherals.h"
#include "Status.h"

class DataLog
{
private:
    const char *getLogFileName();

protected:
public:
    void loop();
    void dump();
};

#endif