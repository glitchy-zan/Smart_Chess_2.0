#ifndef STATE_MANAGING_H
#define STATE_MANAGING_H

#include <state_machine/state_framework.h>
#include <util/util.h>

void handleStates();
extern GameStateEnum currentGameState;
extern MoveStateEnum currentMoveState;
extern PieceStateEnum currentPieceState;
extern int sensorsBoard[8][8];
extern bool flagButtonPressed;

#endif // STATE_MANAGING_H