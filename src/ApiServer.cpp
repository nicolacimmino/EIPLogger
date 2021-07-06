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

    String startLine = client.readStringUntil('\r');

    startLine.toCharArray(Peripherals::buffer, TEXT_BUFFER_SIZE);

    strtok(Peripherals::buffer, " ");
    char *uri = strtok(NULL, " ");

    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");

    if (strcmp(uri, "/info") == 0)
    {
        File file;

        if (file = SPIFFS.open(DataLog::instance()->getLogFileNameForDay(Status::getDayOfYear()), "r"))
        {
            while (file.available())
            {
                memset(Peripherals::buffer, 0,TEXT_BUFFER_SIZE);

                file.readBytesUntil('\n', Peripherals::buffer, TEXT_BUFFER_SIZE);
                
                client.println(Peripherals::buffer);
            }
            file.close();
        }
    }

    client.print("\r\n\r\n");
    client.stop(); 
}