#include "sensor_constants.h"
#include "serial_helper.h"

int read_serial(unsigned char *buffer, uint8_t buffer_length, const char *name) {
  Serial.printf("\n%s: ", name);

  buffer[buffer_length - 1] = '\0';

  int t = 0;
  int p = 0;
  while(p < buffer_length - 1) {
    int c = Serial.read();
    if(c == -1) {
      delay(100);
      t++;
      if(t > 200) {
        Serial.println("\nInput timeout!\n");
        return -1;
      }
    } else if(c == '\n' || c == '\r') {
      Serial.print('\n');
      buffer[p] = '\0';
      break;
    } else {
      Serial.printf("%c", c);
      buffer[p++] = c;
      t = 0;
    }
  }

  Serial.printf("%s is %s.\n", name, buffer);
  return p;
}