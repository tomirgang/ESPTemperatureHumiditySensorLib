#include "data_helper.h"

#define INITIALIZED_KEY "is_initialized"
#define SENSOR_NAME "sensor_name"

bool is_setup(void)
{
    Preferences preferences;
    bool initialized = false;
    preferences.begin(SENSOR_PREFERENCES, true);
    initialized = preferences.getBool(INITIALIZED_KEY, false);
    preferences.end();
    return initialized;
}

int serial_setup(void)
{
    Preferences preferences;
    unsigned char buffer[BUFFER_SIZE];
    int len;
    int error = 0;

    preferences.begin(SENSOR_PREFERENCES, false);

    len = read_serial(buffer, BUFFER_SIZE, "sensor name");
    if (len > 0 && buffer[0] != '\0')
    {
        preferences.putString(SENSOR_NAME, (const char *)buffer);
        preferences.putBool(INITIALIZED_KEY, true);
    }
    else
    {
        error = 1;
    }

    preferences.end();

    return serial_wifi_setup() + error;
}

int serial_wifi_setup(void)
{
    Preferences preferences;
    unsigned char buffer[BUFFER_SIZE];
    int len;

    preferences.begin(SENSOR_PREFERENCES, false);

    len = serial_select_ssid(buffer, BUFFER_SIZE);
    if (len > 0 && buffer[0] != '\0')
    {
        preferences.putString(WIFI_SSID, (const char *)buffer);

        len = read_serial(buffer, BUFFER_SIZE, "wifi password");
        if (len > 0 && buffer[0] != '\0')
        {
            preferences.putString(WIFI_PASSWORD, (const char *)buffer);
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }

    preferences.end();

    return 0;
}

void reset_sensor(void)
{
    Preferences preferences;
    Serial.println("Press r to reset the WiFi data.");
    for (int i = 0; i < 10; i++)
    {
        int c = Serial.read();
        if (c == 'r')
        {
            Serial.println("Resetting WiFi data.");
            preferences.begin(SENSOR_PREFERENCES, false);
            preferences.clear();
            preferences.end();
            ESP.restart();
            return;
        }
        delay(1000);
    }
}

void add_values(float humidity, float temperature) {
    //TODO: implement
}
