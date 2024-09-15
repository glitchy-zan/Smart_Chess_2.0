#ifndef SENSOR_H
#define SENSOR_H

void readSensors();
unsigned char readShiftRegister(int sens_row_x_pin);

#endif // SENSOR_H