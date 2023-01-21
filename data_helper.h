#ifndef _DATA_HELPER_H
#define _DATA_HELPER_H

#include <Preferences.h>
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

#endif
