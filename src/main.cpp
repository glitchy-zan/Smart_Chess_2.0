#include <Arduino.h>
#include <network/network.h>

void setup()
{
    Serial.begin(9600);
    setUpAccessPoint();
    setUpWebSocket();
    setUpServer();
}

void loop()
{
    handleClientRequests();
    handleWebSocketCommunication();
}