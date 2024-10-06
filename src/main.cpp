#include <Arduino.h>
#include <network/network.h>
#include <hardware/button.h>
#include <hardware/pin_config.h>
#include <hardware/sensor.h>
#include <state_machine/state_managing.h>

void setup()
{
    for (int i = 0; i < 8; i++)
    {
        pinMode(sens_row_ALL_out_pin[i], INPUT);
    }
    pinMode(CLK_pin, OUTPUT);
    pinMode(SH_LD_pin, OUTPUT);
    pinMode(button_pin, INPUT_PULLUP);
    Serial.begin(9600);
    setUpAccessPoint();
    setUpWebSocket();
    setUpServer();
    setUpCallbackForButton();
}

void loop()
{
    handleClientRequests();
    handleWebSocketCommunication();
    readSensors();
    handleStates();
    sendUpdate();
    //delay(2000);
}