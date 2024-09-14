#ifndef SENSOR_H
#define SENSOR_H

void readSensors();
unsigned char readShiftRegister(int sens_row_x_pin);
extern int sensors_board[8][8];

#endif // SENSOR_H