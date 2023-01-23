#ifndef _DATA_HELPER_H
#define _DATA_HELPER_H

#include <Arduino.h>
#include <Preferences.h>
#include <time.h>
#include <SPIFFS.h>
#include "sensor_constants.h"
#include "serial_helper.h"
#include "wifi_helper.h"

/**
 * @brief Is sensor configured?
 *
 * @return true if sensor is configured
 * @return false if sensor is not configured
 */
bool is_setup(void);

/**
 * @brief Setup sensor data using serial interaction.
 *
 * @return int 0 if setup was successful.
 */
int serial_setup(void);

/**
 * @brief Setup wifi data using serial interaction.
 *
 * @return int 0 if setup was successful.
 */
int serial_wifi_setup(void);

/**
 * @brief Reset all sensor data.
 */
void reset_sensor(void);

/**
 * @brief Add values from a new sensor measurement.
 *
 * @param humidity measured humidity
 * @param temperature measured temperature
 * @return bool true if day data was updated
 */
bool add_values(float humidity, float temperature);

/**
 * @brief Initialize data structures.
 */
void init_data(void);

/**
 * @brief Save data to flash.
 */
void save_data(void);

/**
 * @brief Load data from flash.
 */
void load_data(void);

/**
 * @brief Format a timestamp as 23.01.1987 13:45:42
 *
 * @param timestamp unix time to format
 * @param buffer buffer of length 20
 */
void format_timestamp(time_t timestamp, char *buffer);

/**
 * @brief Is next day value reached?
 *
 * @return true if measurement was added to day data
 * @return false else
 */
bool is_day(void);

#endif
