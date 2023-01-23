#include "sensor_constants.h"
#include "serial_helper.h"

int read_serial(unsigned char *buffer, uint8_t buffer_length, const char *name)
{
    Serial.printf("\n%s: ", name);

    buffer[buffer_length - 1] = '\0';

    int t = 0;
    int p = 0;
    while (p < buffer_length - 1)
    {
        int c = Serial.read();
        if (c == -1)
        {
            delay(100);
            t++;
            if (t > 200)
            {
                Serial.println("\nInput timeout!\n");
                return -1;
            }
        }
        else if (c == '\n' || c == '\r')
        {
            if(p == 0) {
                continue;
            }

            Serial.print('\n');
            buffer[p] = '\0';
            break;
        }
        else
        {
            Serial.printf("%c", c);
            buffer[p++] = c;
            t = 0;
        }
    }

    Serial.printf("%s is %s.\n", name, buffer);
    return p;
}

void wait_for_serial_connect(void)
{
    for (int i = 0; i < 5; i++)
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.println();
}

void print_data_to_serial(bool short_format)
{
    Serial.println("\n\n=====");
    print_sensor_info_to_serial();
    print_latest_data_to_serial();

    if (!short_format)
    {
        print_last_data_to_serial();
        print_day_data_to_serial();
    }
    Serial.println("\n=====\n");
}

void print_sensor_info_to_serial(void)
{
    Preferences preferences;
    preferences.begin(SENSOR_PREFERENCES, true);
    String ssid = preferences.getString(WIFI_SSID);
    String name = preferences.getString(SENSOR_NAME);
    preferences.end();

    Serial.printf("Sensor %s:\n", name);

    time_t now;
    time(&now);
    char buffer[21];
    format_timestamp(now, buffer);
    Serial.printf("Systemzeit: %s\n", buffer);

    if (WiFi.isConnected())
    {
        Serial.printf("Connected to WiFi %s with IP %s\n", ssid.c_str(), WiFi.localIP().toString().c_str());
    }
    else
    {
        Serial.println("Not connected to WiFi.");
    }
}
