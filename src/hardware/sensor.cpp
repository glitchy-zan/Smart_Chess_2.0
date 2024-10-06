#include <Arduino.h>
#include <hardware/sensor.h>
#include <hardware/pin_config.h>
#include <util/util.h>

/* Showing 0 || 1 depending on sensors detecting magnets */
int sensorsBoard[8][8];

/* Reads whole sensor board */
void readSensors()
{
    // Serial.println("*****************************************************");
    for (int row = 0; row < 8; row++)
    {
        unsigned char value = readShiftRegister(sens_row_ALL_out_pin[row]);
        unsignedCharToIntArray(value, sensorsBoard[row]);

        /*
            // development -> TODO delete
            // [row][0] -> H, [row][7] -> A
            Serial.print(sensorsBoard[row][0]);
            Serial.print(sensorsBoard[row][1]);
            Serial.print(sensorsBoard[row][2]);
            Serial.print(sensorsBoard[row][3]);
            Serial.print(sensorsBoard[row][4]);
            Serial.print(sensorsBoard[row][5]);
            Serial.print(sensorsBoard[row][6]);
            Serial.println(sensorsBoard[row][7]);
          */
    }
}

/* load and read 1 row of sensors via shift register */
unsigned char readShiftRegister(int sens_row_x_pin)
{
    /* Load the values into the shift register */
    digitalWrite(SH_LD_pin, LOW);
    delayMicroseconds(5); // Wait a moment for the values to settle
    digitalWrite(SH_LD_pin, HIGH);

    unsigned char value = 0;

    /* Read each bit from the shift register */
    for (int i = 7; i >= 0; i--)
    {
        /* Drop the clock to prepare for reading */
        digitalWrite(CLK_pin, LOW);
        delayMicroseconds(1);

        /* Read the bit from the Q7 pin */
        int out = digitalRead(sens_row_x_pin);
        if (out == 0)
        {
            value |= (1 << i);
        }
        /* Pulse the clock to shift the next bit */
        digitalWrite(CLK_pin, HIGH);
        delayMicroseconds(1);
    }
    return value;
}