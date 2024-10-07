#ifndef NETWORK_H
#define NETWORK_H

#include <state_machine/state_framework.h>
#include <state_machine/state_managing.h>

void setUpAccessPoint();
void setUpWebSocket();
void setUpServer();
void handleClientRequests();
void handleWebSocketCommunication();
void sendUpdate();
String gameStateToString(GameStateEnum state);
String moveStateToString(MoveStateEnum state);
String pieceStateToString(PieceStateEnum state);
extern GameStateEnum currentGameState;
extern MoveStateEnum currentMoveState;
extern PieceStateEnum currentPieceState;
extern int sensorsBoard[8][8];
extern GameContext gameContext;

#endif // NETWORK_H