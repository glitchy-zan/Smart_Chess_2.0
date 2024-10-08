#include <vector>
#include <state_machine/state_managing.h>

GameContext gameContext;

void handleStates()
{
    checkButtonPressed();
    switch (currentGameState)
    {
    case GameStateEnum::IDLE:
        proccessGameStateIdle();
        break;
    case GameStateEnum::RUNNING:
        monitorPieceLiftings(gameContext);
        handleMoveState();
        break;
    case GameStateEnum::GAME_OVER:
        proccessGameStateGameOver();
        break;
    default:
        break;
    }
}