#ifndef _SENSOR_UDP_H
#define _SENSOR_UDP_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include <Preferences.h>

#include "sensor_constants.h"
#include "data_helper.h"

/**
 * @brief Send sensor data as UDP broadcast.
 * 
 * @param temperature temperature value as float
 * @param humidity humidity value as float
 */
void udp_send_data(float humidity, float temperature);

#endif
