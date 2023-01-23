#include "sensor_udp.h"

WiFiUDP Udp;

void udp_send_data(float humidity, float temperature)
{
    time_t now;
    time(&now);

    char buffer[21];
    format_timestamp(now, buffer);

    Preferences preferences;
    preferences.begin(SENSOR_PREFERENCES, true);

    Udp.beginPacket(UDP_BROADCAST_IP, UDP_BROADCAST_PORT);
    Udp.printf("Sensor %s, Time: %s, TS: %ld, Temperature: %.2f °C, Humidity: %.2f %%",
               preferences.getString(SENSOR_NAME).c_str(), buffer, now, temperature, humidity);
    Udp.endPacket();

    preferences.end();

    Serial.println("UDP packet sent.");
}
