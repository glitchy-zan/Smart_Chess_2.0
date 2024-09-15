#include <state_machine/state_framework.h>

GameStateEnum currentGameState = GameStateEnum::IDLE;
MoveStateEnum currentMoveState = MoveStateEnum::IDLE;
PieceStateEnum currentPieceState = PieceStateEnum::IDLE;

void changeState(GameStateEnum newGameState, MoveStateEnum newMoveState, PieceStateEnum newPieceState)
{
    currentGameState = newGameState;
    currentMoveState = newMoveState;
    currentPieceState = newPieceState;
}
