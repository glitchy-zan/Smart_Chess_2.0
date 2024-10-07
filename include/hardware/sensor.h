#ifndef SENSOR_H
#define SENSOR_H

#include <hardware/pin_config.h>
#include <util/util.h>

void readSensors();
unsigned char readShiftRegister(int sens_row_x_pin);

#endif // SENSOR_H