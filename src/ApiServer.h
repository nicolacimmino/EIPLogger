#ifndef __API_SERVER_H__
#define __API_SERVER_H__

#include <WiFiServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include "config.h"
#include "Status.h"
#include "DataLog.h"

class ApiServer
{
    private:
        WiFiServer* server;
    public:
        ApiServer();
        void loop();
};

#endif