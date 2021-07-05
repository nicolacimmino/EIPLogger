#ifndef __API_SERVER_H__
#define __API_SERVER_H__

#include <WiFiServer.h>
#include <ESPmDNS.h>
#include "config.h"

class ApiServer
{
    private:
        WiFiServer* server;
    public:
        ApiServer();
        void loop();
};

#endif