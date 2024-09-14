#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

void setUpAccessPoint();
void setUpWebSocket();
void setUpServer();
void handleClientRequests();
void handleWebSocketCommunication();

#endif // NETWORK_H