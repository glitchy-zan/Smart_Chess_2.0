#include <state_machine/game_state_managing.h>

/* when game state is IDLE we are checking for starting position. Once reached we change game state to RUNNING */
void proccessGameStateIdle()
{
    if (isStartingPosition(sensorsBoard))
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IDLE, PieceStateEnum::IDLE);
}

/* when websocket receives action for reset */
void proccessGameStateGameOver()
{
    resetGameVariables(gameContext);
}