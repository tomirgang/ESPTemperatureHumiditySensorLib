#include "data_helper.h"

#define INITIALIZED_KEY "is_initialized"
#define SENSOR_NAME "sensor_name"

Preferences preferences;

bool is_setup(void) {
    bool initialized = false;
    preferences.begin(SENSOR_PREFERENCES, true);
    initialized = preferences.getBool(INITIALIZED_KEY, false);
    preferences.end();
    return initialized;
}

int serial_setup(void) {
    unsigned char buffer[BUFFER_SIZE];
    int len;
    int error = 0;

    preferences.begin(SENSOR_PREFERENCES, false);

    len = read_serial(buffer, BUFFER_SIZE, "sensor name");
    if(len > 0 && buffer[0] != '\0') {
        preferences.putString(SENSOR_NAME, (const char *)buffer);
        preferences.putBool(INITIALIZED_KEY, true);
    } else {
        error = 1;
    }

    preferences.end();

    return serial_wifi_setup() + error;
}

int serial_wifi_setup(void) {
    unsigned char buffer[BUFFER_SIZE];
    int len;

    preferences.begin(SENSOR_PREFERENCES, false);

    len = serial_select_ssid(buffer, BUFFER_SIZE);
    if(len > 0 && buffer[0] != '\0') {
        preferences.putString(WIFI_SSID, (const char *)buffer);

        len = read_serial(buffer, BUFFER_SIZE, "wifi password");
        if(len > 0 && buffer[0] != '\0') {
            preferences.putString(WIFI_PASSWORD, (const char *)buffer);
        } else {
            return 1;
        }
    } else {
        return 1;
    }

    preferences.end();

    return 0;
}