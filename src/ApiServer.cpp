#include "ApiServer.h"

ApiServer::ApiServer()
{
    this->server = new WiFiServer(80);
    this->server->begin();
    MDNS.addService("http", "tcp", 80);
}

void ApiServer::loop()
{
    WiFiClient client = this->server->available();
    if (!client)
    {
        DIAGNOSTIC("APICLI,no")
        return;
    }

    while (client.connected() && !client.available())
    {
        delay(1);
        DIAGNOSTIC("APICLI,waiting")
    }

    client.readStringUntil('\r');
    
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nOK\r\n\r\n");
 
    DIAGNOSTIC("APICLI,served")

    client.stop();
}