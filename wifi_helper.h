#ifndef _WIFI_HELPER_H
#define _WIFI_HELPER_H

#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include "serial_helper.h"
#include "sensor_constants.h"

/**
 * @brief Select a WiFi SSID using serial.
 * 
 * @param buffer buffer for SSID.
 * @param buffer_length buffer length
 * @return int ssid length, -1 in case of error
 */
int serial_select_ssid(unsigned char *buffer, uint8_t buffer_length);

/**
 * @brief Is WiFi configured?
 * 
 * @return true if WiFi is configured
 * @return false if WiFi is not configured.
 */
bool is_wifi_setup(void);

/**
 * @brief Connect to WiFi network.
 */
void connect(void);

/**
 * @brief Interactive reset of WiFi configuration.
 * 
 */
void serial_reset_wifi_data(void);

#endif