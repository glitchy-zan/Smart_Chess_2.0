#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <network/network.h>
#include <network/web_handler.h>

/* function prototypes */
void setUpAccessPoint();
void setUpWebSocket();
void setUpServer();
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void handleClientRequests();
void handleWebSocketCommunication();

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

/* Sends update of states and moves to all clients. handle_root function then displays it to web page */
void sendUpdate()
{
    String json = "{\"pgn\":\"" + pgn + "\","
                                        "\"game\":\"" +
                  gameStateToString(currentGameState) + "\","
                                                  "\"move\":\"" +
                  moveStateToString(currentMoveState) + "\","
                                                  "\"piece\":\"" +
                  pieceStateToString(currentPieceState) + "\"}";
    webSocket.broadcastTXT(json);
}

String gameStateToString(GameStateEnum state)
{
    switch (state)
    {
    case GameStateEnum::IDLE:
        return "Game has not started.";
    case GameStateEnum::RUNNING:
        return "Game is running.";
    case GameStateEnum::GAME_OVER:
        return "Game has ended.";
    default:
        return "UNKNOWN";
    }
}

String moveStateToString(MoveStateEnum state)
{
    switch (state)
    {
    case MoveStateEnum::IDLE:
        return "IDLE";
    case MoveStateEnum::IN_PROGRESS:
        return "Move is in progress.";
    case MoveStateEnum::MOVE_MADE:
        return "Move has been made.";
    default:
        return "UNKNOWN";
    }
}

String pieceStateToString(PieceStateEnum state)
{
    switch (state)
    {
    case PieceStateEnum::IDLE:
        return "IDLE";
    case PieceStateEnum::PIECE_LIFTED:
        return "One Piece is lifted.";
    case PieceStateEnum::SECOND_PIECE_LIFTED:
        return "Two pieces are lifted.";
    case PieceStateEnum::MULTIPLE_PIECES_LIFTED:
        return "Multiple pieces are lifted.";
    case PieceStateEnum::CHANGED:
        return "CHANGED";
    default:
        return "UNKNOWN";
    }
}