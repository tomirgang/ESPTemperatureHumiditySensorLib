#ifndef _SERIAL_HELPER_H
#define _SERIAL_HELPER_H

#include "Arduino.h"

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
 */
void print_data_to_serial(void);

#endif
