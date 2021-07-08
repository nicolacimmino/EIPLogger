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
    else if (strcmp("info", firstPathToken) == 0)
    {
        this->serveInfo(&client);
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
        client->print("HTTP/1.1 200 OK\r\nContent-Type: text/csv\r\n\r\n");

        while (file.available())
        {
            memset(Peripherals::buffer, 0, TEXT_BUFFER_SIZE);

            size_t bytesRead = file.readBytes(Peripherals::buffer, TEXT_BUFFER_SIZE - 1);

            Peripherals::buffer[bytesRead] = 0;

            client->print(Peripherals::buffer);
        }
        file.close();

        return;
    }

    client->print("HTTP/1.1 404 Not Found\r\n\r\n");
}

void ApiServer::serveInfo(WiFiClient *client)
{
    client->print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");

    client->print("{");

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "\"ip\": \"%s\",", WiFi.localIP().toString().c_str());
    client->print(Peripherals::buffer);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "\"disk\" : { \"used\": \"%d\", \"total\": \"%d\" },",
             SPIFFS.usedBytes(),
             SPIFFS.totalBytes());
    client->print(Peripherals::buffer);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "\"battery\" : { \"level\": \"%d\", \"voltage\": \"%0.2f\" },",
             Status::getBatteryLevel(),
             Status::batteryVoltage / 1000.0);
    client->print(Peripherals::buffer);

    snprintf(Peripherals::buffer, TEXT_BUFFER_SIZE, "\"iaq\" : { \"temperature\": \"%0.1f\", \"humidity\": \"%d\",\"co2\": \"%d\", \"tvoc\": \"%d\",\"pm1\": \"%d\", \"pm2p5\": \"%d\",\"pm4\": \"%d\", \"pm10\": \"%d\" }",
             Status::temperature->get(),
             Status::humidity->get(),
             Status::co2->get(),
             Status::tvoc->get(),
             Status::pm1->get(),
             Status::pm2p5->get(),
             Status::pm4->get(),
             Status::pm10->get());
    client->print(Peripherals::buffer);

    client->print("}");
}