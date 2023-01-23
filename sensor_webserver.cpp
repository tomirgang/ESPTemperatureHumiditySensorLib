#include "sensor_webserver.h"

#define RESPONSE_SIZE 15000

WebServer server(80);
char buffer[RESPONSE_SIZE];

void handle_latest();
void handle_last();
void handle_day();
void handle_NotFound();

void init_webserver(void)
{
    server.on("/", handle_latest);
    server.on("/last", handle_last);
    server.on("/day", handle_day);
    server.onNotFound(handle_NotFound);

    server.begin();
    Serial.println("HTTP server started");
}

void handle_client(void)
{
    server.handleClient();
}

void handle_latest()
{
    json_latest(buffer, RESPONSE_SIZE);
    buffer[RESPONSE_SIZE - 1] = '\0';
    server.send(200, "application/json", buffer);
}

void handle_last()
{
    json_last(buffer, RESPONSE_SIZE);
    buffer[RESPONSE_SIZE - 1] = '\0';
    server.send(200, "application/json", buffer);
}

void handle_day()
{
    json_day(buffer, RESPONSE_SIZE);
    buffer[RESPONSE_SIZE - 1] = '\0';
    server.send(200, "application/json", buffer);
}

void handle_NotFound()
{
    server.send(404, "text/plain", "Not found");
}
