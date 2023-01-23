#ifndef _SERIAL_HELPER_H
#define _SERIAL_HELPER_H

#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <time.h>
#include "data_helper.h"
#include "sensor_ntp.h"

/**
 * @brief Read data from serial console.
 *
 * @param buffer buffer for writing data
 * @param buffer_length length of the input buffer
 * @param name name to display as prompt
 * @return int number of read characters
 */
int read_serial(unsigned char *buffer, uint8_t buffer_length, const char *name);

/**
 * @brief Wait some seconds to give the PC a chance to connect.
 */
void wait_for_serial_connect(void);

/**
 * @brief Print sensor measurements to serial.
 *
 * @param short_format also print historic data?
 *
 */
void print_data_to_serial(bool short_format);

/**
 * @brief Print sensor information
 *
 */
void print_sensor_info_to_serial(void);

/**
 * @brief Print latest measurement.
 *
 */
void print_latest_data_to_serial(void);

/**
 * @brief Print last data record.
 *
 */
void print_last_data_to_serial(void);

/**
 * @brief Print day data record.
 *
 */
void print_day_data_to_serial(void);

#endif
