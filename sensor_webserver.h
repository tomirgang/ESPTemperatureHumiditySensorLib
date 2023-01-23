#ifndef _SENSOR_WEBSERVER_H
#define _SENSOR_WEBSERVER_H

#include <WebServer.h>
#include "sensor_constants.h"
#include "data_helper.h"

/**
 * @brief Init webserver.
 */
void init_webserver(void);

/**
 * @brief Process client request.
 * 
 */
void handle_client(void);

#endif