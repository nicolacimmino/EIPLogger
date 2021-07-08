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
        return;
    }

    while (client.connected() && !client.available())
    {
        delay(1);
    }

    client.readStringUntil('\r').toCharArray(Peripherals::buffer, TEXT_BUFFER_SIZE);

    strtok(Peripherals::buffer, " ");
    
    char *uri = strtok(NULL, " ");
    char *firstPathToken = strtok(uri, "/");

    if (strcmp("log", firstPathToken) == 0)
    {
        uint8_t daysOffset = atoi(strtok(NULL, "/"));
        this->serveLog(&client, daysOffset);
    }
    else
    {
        client.print("HTTP/1.1 404 Not Found\r\n\r\n");
    }

    client.print("\r\n\r\n");
    client.stop();
}

void ApiServer::serveLog(WiFiClient *client, uint8_t daysOffset)
{
    File file;

    if (file = SPIFFS.open(DataLog::instance()->getLogFileNameForDay(Status::getDayOfYear() - daysOffset), "r"))
    {
        client->print("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");

        while (file.available())
        {
            memset(Peripherals::buffer, 0, TEXT_BUFFER_SIZE);

            file.readBytesUntil('\n', Peripherals::buffer, TEXT_BUFFER_SIZE);

            client->println(Peripherals::buffer);
        }
        file.close();

        return;
    }

    client->print("HTTP/1.1 404 Not Found\r\n\r\n");
}