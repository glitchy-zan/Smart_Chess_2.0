#include <Arduino.h>
#include <network/network.h>
#include <hardware/button.h>
#include <hardware/pin_config.h>

void setup()
{
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
}