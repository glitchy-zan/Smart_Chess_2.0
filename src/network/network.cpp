#include <network/network.h>
#include <network/web_handler.h>

/* function prototypes */
void setUpAccessPoint();
void setUpWebSocket();
void setUpServer();
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void handleClientRequests();
void handleWebsocketCommunication();

/* WiFi credentials for the ESP32 access point */
const char *ssid = "ESP32Net";
const char *password = "esp32net";
/* Create web server on esp32 */
WebServer server(80);
/* Create web socket for real time communication between server and client */
WebSocketsServer webSocket = WebSocketsServer(81);

void setUpAccessPoint()
{
    WiFi.softAP(ssid, password);
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
}

void setUpWebSocket()
{
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void setUpServer()
{
    server.on("/", handleRoot);
    server.begin();
    Serial.println("HTTP server started");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
    {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %s\n", num, ip.toString().c_str());
        break;
    }
    case WStype_TEXT:
        Serial.printf("[%u] Received text: %s\n", num, payload);
        break;
    case WStype_BIN:
        Serial.printf("[%u] Received binary data\n", num);
        break;
    }
}

void handleClientRequests()
{
    server.handleClient();
}

void handleWebSocketCommunication()
{
    webSocket.loop();
}