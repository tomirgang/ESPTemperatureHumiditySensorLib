#include "sensor_ntp.h"

const char *ntpServer = "pool.ntp.org";

void init_ntp(void)
{
    configTime(0, 0, ntpServer);
    setenv("TZ", MY_TZ, 1); // Set environment variable with time zone
    tzset();
}
