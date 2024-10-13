#ifndef STATE_MANAGING_H
#define STATE_MANAGING_H

#include <state_machine/state_framework.h>
#include <util/util.h>

struct GameContext
{
    int onMove = 1;
    int moveNum = 1;
    String pgn = "";
    int msBoard[8][8] = {
        {4, 2, 3, 6, 5, 3, 2, 4},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {-4, -2, -3, -6, -5, -3, -2, -4}};
    std::vector<std::pair<int, int>> pieceLiftedLocations;
    std::vector<std::pair<int, int>> piecePlacedLocations;
    std::pair<int, int> firstPieceLiftedLocation = {-1, -1};
    std::pair<int, int> secondPieceLiftedLocation = {-1, -1};
    bool isCapture = false;
    bool opponentsPieceLifted = false;
    /* 0 -> wshort, 1 -> wlong, 2 -> bshort, 3 -> blong */
    int isCastling = -1;
};

void handleStates();
/* from game_state_managing.cpp */
void proccessGameStateIdle();
void proccessGameStateGameOver();
/* from move_state_managing.cpp */
void handleMoveState();
void proccessMoveStateIdle();
void proccessMoveStateMoveMade();
/* from piece_state_managing.cpp */
void handlePieceState();
void proccessPieceStateIdle();
void proccessPieceStatePieceLifted();
void proccessPieceStateSecondPieceLifted();
void proccessPieceStateCastling();
void proccessPieceStateMultiplePiecesLifted();
void proccessPieceStateChanged();
/* from state_machine_utils.cpp */
void monitorPieceLiftings(GameContext &gameContext);
void resetMsBoard();
void checkButtonPressed();
void resetGameVariables(GameContext &gameContext);
void resetFirstAndSecondPieceLiftedLocations(GameContext &gameContext);
bool areKingAndRookLifted(GameContext &gameContext);
int determineCastlingType(GameContext &gameContext);
bool isCastlingPlacementValid(GameContext &gameContext);
bool isPieceLiftedOnMove(GameContext &gameContext);
bool areKingAndRookLiftedAndSameColor(GameContext &gameContext);

/* from state_framework.cpp */
extern GameStateEnum currentGameState;
extern MoveStateEnum currentMoveState;
extern PieceStateEnum currentPieceState;
extern int sensorsBoard[8][8];
extern bool flagButtonPressed;

#endif // STATE_MANAGING_H