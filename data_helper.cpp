#include "data_helper.h"

#define INITIALIZED_KEY "is_initialized"

#define LEN_LAST 100
#define LEN_DAY 96

struct measurement
{
    time_t ts;
    float humidity;
    float temperature;
};

struct measurement latest;

bool use_spiffs = false;

uint8_t last_ptr = 0;
uint8_t day_ptr = 0;
time_t last_day_update = 0l;
struct measurement last[LEN_LAST];
struct measurement day[LEN_DAY];

void init_data(void)
{
    for (int i = 0; i < LEN_LAST; i++)
    {
        last[i].ts = 0;
    }

    for (int i = 0; i < LEN_DAY; i++)
    {
        day[i].ts = 0;
    }

    if (SAVE_DATA)
    {
        if (!SPIFFS.begin(true))
        {
            Serial.println("An Error has occurred while mounting SPIFFS");
            use_spiffs = false;
        }
        else
        {
            Serial.println("Using SPIFFS to save data.");
            use_spiffs = true;
        }
    }

    load_data();
}

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

    preferences.begin(SENSOR_PREFERENCES, false);

    int ok = serial_select_ssid(buffer, BUFFER_SIZE);
    if (ok == 0 && buffer[0] != '\0')
    {
        Serial.printf("Using SSID %s.\n", buffer);

        preferences.putString(WIFI_SSID, (const char *)buffer);

        int len = read_serial(buffer, BUFFER_SIZE, "wifi password");
        if (len > 0 && buffer[0] != '\0')
        {
            Serial.println("WiFi password OK.");
            preferences.putString(WIFI_PASSWORD, (const char *)buffer);
        }
        else
        {
            Serial.println("Invalid WiFi password!");
            return 1;
        }
    }
    else
    {
        Serial.println("Invalid SSID.");
        return 1;
    }

    preferences.end();

    return 0;
}

void reset_sensor(void)
{
    Preferences preferences;
    Serial.println("Press r to reset the sensor data.");
    for (int i = 0; i < 10; i++)
    {
        int c = Serial.read();
        if (c == 'r')
        {
            Serial.println("Resetting sensor data.");
            preferences.begin(SENSOR_PREFERENCES, false);
            preferences.clear();
            preferences.end();
            ESP.restart();
            return;
        }
        Serial.print(".");
        delay(1000);
    }
    Serial.println();
}

bool add_values(float humidity, float temperature)
{
    bool day_update = false;
    time(&latest.ts);
    latest.humidity = humidity;
    latest.temperature = temperature;

    last[last_ptr].ts = latest.ts;
    last[last_ptr].humidity = latest.humidity;
    last[last_ptr].temperature = latest.temperature;

    last_ptr = (last_ptr + 1) % LEN_LAST;

    time_t now;
    time(&now);
    if (now > (last_day_update + DAY_SLEEP_TIME))
    {
        time(&last_day_update);

        Serial.println("Record new day value.");

        day[day_ptr].ts = latest.ts;
        day[day_ptr].humidity = latest.humidity;
        day[day_ptr].temperature = latest.temperature;

        day_ptr = (day_ptr + 1) % LEN_DAY;

        save_data();

        day_update = true;
    }

    return day_update;
}

void save_data(void)
{
    if (!use_spiffs)
    {
        return;
    }

    File file = SPIFFS.open("/last.data", FILE_WRITE);

    if (file)
    {
        size_t len = file.write((byte *)last, sizeof(last));
        if (len != sizeof(last))
        {
            Serial.println("Saving last.data failed. Size is wrong.");
        }
        else
        {
            Serial.println("Saving last.data succeeded.");
        }
    }
    else
    {
        Serial.println("Failed to open file last.data for writing.");
    }

    file.close();

    file = SPIFFS.open("/day.data", FILE_WRITE);

    if (file)
    {
        size_t len = file.write((byte *)day, sizeof(day));
        if (len != sizeof(day))
        {
            Serial.println("Saving day.data failed. Size is wrong.");
        }
        else
        {
            Serial.println("Saving day.data succeeded.");
        }
    }
    else
    {
        Serial.println("Failed to open file day.data for writing.");
    }

    file.close();
}

void load_data(void)
{
    if (!use_spiffs)
    {
        return;
    }

    File file = SPIFFS.open("/last.data");

    if (file)
    {
        size_t len = file.readBytes((char *)last, sizeof(last));
        if (len != sizeof(last))
        {
            Serial.println("File last.data has wrong length. Resetting data.");
            for (int i = 0; i < LEN_LAST; i++)
            {
                last[i].ts = 0;
            }
        }
        else
        {
            last_ptr = 0;
            while (last_ptr < (LEN_LAST - 1) && last[last_ptr + 1].ts > last[last_ptr].ts)
            {
                last_ptr++;
            }
            last_ptr = (last_ptr + 1) % LEN_LAST;

            print_last_data_to_serial();

            Serial.printf("last ptr: %d\n", last_ptr);

            if (last_ptr > 0)
            {
                char buffer[21];
                format_timestamp(last[last_ptr].ts, buffer);
                char buffer2[21];
                format_timestamp(last[last_ptr - 1].ts, buffer2);
                Serial.printf("last ptr: %s, last ptr - 1: %s\n", buffer, buffer2);
            }
            else
            {
                char buffer[21];
                format_timestamp(last[last_ptr].ts, buffer);
                Serial.printf("time of last ptr: %s\n", buffer);
            }
        }
    }
    else
    {
        Serial.println("Failed to open file last.data for reading.");
    }

    file.close();

    file = SPIFFS.open("/day.data");

    if (file)
    {
        size_t len = file.readBytes((char *)day, sizeof(day));
        if (len != sizeof(day))
        {
            Serial.println("File day.data has wrong length. Resetting data.");
            for (int i = 0; i < LEN_DAY; i++)
            {
                day[i].ts = 0;
            }
        }
        else
        {
            day_ptr = 0;
            while (day_ptr < (LEN_DAY - 1) && day[day_ptr + 1].ts > day[day_ptr].ts)
            {
                day_ptr++;
            }
            day_ptr = (day_ptr + 1) % LEN_DAY;

            print_day_data_to_serial();

            Serial.printf("day ptr: %d\n", day_ptr);

            if (day_ptr > 0)
            {
                char buffer[21];
                format_timestamp(day[day_ptr].ts, buffer);
                char buffer2[21];
                format_timestamp(day[day_ptr - 1].ts, buffer2);
                Serial.printf("day ptr: %s, day ptr - 1: %s\n", buffer, buffer2);
            }
            else
            {
                char buffer[21];
                format_timestamp(day[day_ptr].ts, buffer);
                Serial.printf("time of day ptr: %s\n", buffer);
            }
        }
    }
    else
    {
        Serial.println("Failed to open file day.data for reading.");
    }

    file.close();
}

void format_timestamp(time_t timestamp, char *buffer)
{
    strftime(buffer, 20, "%d.%m.%Y %H:%M:%S", localtime(&timestamp));
}

void print_measurement(struct measurement *data)
{
    char buffer[20];
    format_timestamp(data->ts, buffer);
    Serial.printf("%s: Temperatur: %.2f °C, Feuchtigkeit: %.2f %%\n",
                  buffer, data->temperature, data->humidity);
}

void print_latest_data_to_serial(void)
{
    char buffer[21];
    format_timestamp(last_day_update, buffer);
    Serial.printf("last day: %s, last_ptr: %d, day_ptr: %d\n", buffer, last_ptr, day_ptr);
    print_measurement(&latest);
}

void print_last_data_to_serial(void)
{
    Serial.println("\n\nLast measurements:");
    for (int i = LEN_LAST - 10; i < LEN_LAST; i++)
    {
        int idx = (last_ptr + i) % LEN_LAST;
        if (last[idx].ts > 0)
        {
            Serial.printf("%d: ", idx);
            print_measurement(&last[idx]);
        }
    }
    Serial.println("----\n");
}

void print_day_data_to_serial(void)
{
    Serial.println("\n\nLast day:");
    for (int i = LEN_DAY - 10; i < LEN_DAY; i++)
    {
        int idx = (day_ptr + i) % LEN_DAY;
        if (day[idx].ts > 0)
        {
            Serial.printf("%d: ", idx);
            print_measurement(&day[idx]);
        }
    }
    Serial.println("----\n");
}

bool is_day(void)
{
    time_t now;
    time(&now);
    if (day_ptr == 0)
    {
        return now > (day[LEN_DAY - 1].ts + DAY_SLEEP_TIME);
    }
    else
    {
        return now > (day[day_ptr - 1].ts + DAY_SLEEP_TIME);
    }
}

size_t json_latest(char *buffer, size_t len)
{
    Preferences preferences;
    preferences.begin(SENSOR_PREFERENCES, true);

    char buf[21];
    format_timestamp(latest.ts, buf);

    size_t p = snprintf(buffer, len, "{\"Sensor\":\"%s\",\"Time\":\"%s\",\"TS\":\"%ld\",\"Temperature\":\"%.2f °C\",\"Humidity\":\"%.2f %%\"}",
                        preferences.getString(SENSOR_NAME).c_str(), buf, latest.ts, latest.temperature, latest.humidity);

    preferences.end();

    return p;
}

size_t json_last(char *buffer, size_t len)
{
    Preferences preferences;
    preferences.begin(SENSOR_PREFERENCES, true);

    char buf[21];
    format_timestamp(latest.ts, buf);

    size_t p = snprintf(buffer, len, "{\"Sensor\":\"%s\",\"Time\":\"%s\",\"TS\":\"%ld\",\"Temperature\":\"%.2f °C\",\"Humidity\":\"%.2f %%\",\"last\":[",
                        preferences.getString(SENSOR_NAME).c_str(), buf, latest.ts, latest.temperature, latest.humidity);
    preferences.end();

    for (int i = 0; i < LEN_LAST; i++)
    {
        int idx = (last_ptr + i) % LEN_LAST;
        if (last[idx].ts > 0)
        {
            format_timestamp(last[idx].ts, buf);
            p = p + snprintf(&buffer[p], len - p, "{\"Time\":\"%s\",\"TS\":\"%ld\",\"Temperature\":\"%.2f °C\",\"Humidity\":\"%.2f %%\"},",
                             buf, last[idx].ts, last[idx].temperature, last[idx].humidity);
        }
    }
    p--; // remove last ,
    p = p + snprintf(&buffer[p], len - p, "]}");

    return p;
}

size_t json_day(char *buffer, size_t len)
{
    Preferences preferences;
    preferences.begin(SENSOR_PREFERENCES, true);

    char buf[21];
    format_timestamp(latest.ts, buf);

    size_t p = snprintf(buffer, len, "{\"Sensor\":\"%s\",\"Time\":\"%s\",\"TS\":\"%ld\",\"Temperature\":\"%.2f °C\",\"Humidity\":\"%.2f %%\",\"day\":[",
                        preferences.getString(SENSOR_NAME).c_str(), buf, latest.ts, latest.temperature, latest.humidity);
    preferences.end();

    for (int i = 0; i < LEN_DAY; i++)
    {
        int idx = (day_ptr + i) % LEN_DAY;
        if (day[idx].ts > 0)
        {
            format_timestamp(day[idx].ts, buf);
            p = p + snprintf(&buffer[p], len - p, "{\"Time\":\"%s\",\"TS\":\"%ld\",\"Temperature\":\"%.2f °C\",\"Humidity\":\"%.2f %%\"},",
                             buf, day[idx].ts, day[idx].temperature, day[idx].humidity);
        }
    }
    p--; // remove last ,
    p = p + snprintf(&buffer[p], len - p, "]}");

    return p;
}