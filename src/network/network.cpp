#include <WiFi.h>
#include <ArduinoJson.h>
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
    {
        Serial.printf("[%u] Received text: %s\n", num, payload);
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (error)
        {
            Serial.print(F("Failed to parse JSON: "));
            Serial.println(error.f_str());
            return;
        }
        const char *action = doc["action"];
        if (strcmp(action, "reset") == 0)
        {
            Serial.println("Reset game received from client.");
            // Perform the reset operation on the ESP32 side
            changeState(GameStateEnum::GAME_OVER, MoveStateEnum::IDLE, PieceStateEnum::IDLE);
        }
        break;
    }
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
    String json = "{\"pgn\":\"" + gameContext.pgn + "\","
                                                    "\"game\":\"" +
                  gameStateToString(currentGameState) + "\","
                                                        "\"move\":\"" +
                  moveStateToString(currentMoveState) + "\","
                                                        "\"piece\":\"" +
                  pieceStateToString(currentPieceState) + "\","
                                                          "\"board\":[";

    // Add the 8x8 msBoard array to the JSON
    for (int i = 0; i < 8; i++)
    {
        json += "[";
        for (int j = 0; j < 8; j++)
        {
            json += String(gameContext.msBoard[i][j]);
            if (j < 7)
                json += ","; // Add a comma between elements, but not after the last one
        }
        json += "]";
        if (i < 7)
            json += ","; // Add a comma between rows, but not after the last one
    }

    json += "],";

    json += "\"sensorsBoard\":[";
    // Add the 8x8 anotherBoard array to the JSON
    for (int i = 0; i < 8; i++)
    {
        json += "[";
        for (int j = 0; j < 8; j++)
        {
            json += String(sensorsBoard[i][j]); // Assuming anotherBoard is defined similarly to msBoard
            if (j < 7)
                json += ","; // Add a comma between elements, but not after the last one
        }
        json += "]";
        if (i < 7)
            json += ","; // Add a comma between rows, but not after the last one
    }

    json += "]}";

    // Broadcast the JSON string via WebSocket
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
    case PieceStateEnum::CASTLING:
        return "CASTLING";
    default:
        return "UNKNOWN";
    }
}