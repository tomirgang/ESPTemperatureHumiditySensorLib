#include "wifi_helper.h"

int serial_select_ssid(unsigned char *buffer, uint8_t buffer_length)
{
    if (WiFi.isConnected())
    {
        WiFi.disconnect();
        delay(1000);
    }

    WiFi.mode(WIFI_STA);
    delay(1000);

    int scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);

    if (scanResult == 0)
    {
        Serial.println(F("No networks found"));
    }
    else if (scanResult > 0)
    {
        Serial.printf("%d networks found:\n", scanResult);

        for (int i = 0; i < scanResult; i++)
        {
            Serial.printf("%d: %s\n", i, WiFi.SSID(i).c_str());
        }

        int len = read_serial(buffer, buffer_length, "network nr");
        if (len > 0)
        {
            int idx = 0;
            for (int i = 0; i < len; i++)
            {
                if (buffer[i] >= '0' && buffer[i] <= '9')
                {
                    idx = (idx * 10) + (buffer[0] - '0');
                }
            }
            if (idx < 0 || idx >= scanResult)
            {
                Serial.println("Invalid SSID number.");
                return -1;
            }
            String ssid = WiFi.SSID(idx);
            Serial.printf("SSID %d (%s) selected.", idx, ssid.c_str());

            ssid.getBytes(buffer, buffer_length);
            buffer[buffer_length - 1] = '\0';
        }
        else
        {
            Serial.println("Invalid SSID number.");
            return -1;
        }
    }
    else
    {
        Serial.printf("WiFi scan error %d", scanResult);
        return -1;
    }
    return 0;
}

bool is_wifi_setup(void)
{
    Preferences preferences;
    bool initialized = false;
    preferences.begin(SENSOR_PREFERENCES, true);
    initialized = preferences.isKey(WIFI_SSID) && preferences.isKey(WIFI_PASSWORD);
    preferences.end();
    return initialized;
}

void wifi_connect(void)
{
    Preferences preferences;

    if (!is_wifi_setup())
    {
        Serial.println("WiFi not configured.");
        return;
    }

    if (WiFi.isConnected())
    {
        Serial.println("WiFi already connected.");
        return;
    }

    preferences.begin(SENSOR_PREFERENCES, true);
    String ssid = preferences.getString(WIFI_SSID);
    String password = preferences.getString(WIFI_PASSWORD);
    preferences.end();

    WiFi.begin(ssid.c_str(), password.c_str());

    Serial.println("\nConnecting");
    int t = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(200);
        t++;
        if (t > 50)
        {
            Serial.println("WiFi failed.");
            serial_reset_wifi_data();
            return;
        }
    }

    Serial.print("\nConnected to the WiFi network. IP: ");
    Serial.println(WiFi.localIP());
}

void serial_reset_wifi_data(void)
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
            preferences.remove(WIFI_SSID);
            preferences.remove(WIFI_PASSWORD);
            preferences.end();
            return;
        }
        delay(1000);
    }
}
