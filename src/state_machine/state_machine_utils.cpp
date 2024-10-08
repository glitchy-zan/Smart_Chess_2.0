#include <state_machine/state_machine_utils.h>

/* checks if button is pressed (move is made) and changes state correspondingly */
void checkButtonPressed()
{
    if (!flagButtonPressed || currentPieceState != PieceStateEnum::CHANGED)
        return;
    changeState(GameStateEnum::RUNNING, MoveStateEnum::MOVE_MADE, PieceStateEnum::IDLE);
    flagButtonPressed = false;
}

/* monitors if pieces are lifted and saves locations in pieceLiftedLocations */
void monitorPieceLiftings(GameContext &gameContext)
{
    gameContext.pieceLiftedLocations = getLiftedLocations(sensorsBoard, gameContext.msBoard);
}

/* resets game variables to theirs original values */
void resetGameVariables(GameContext &gameContext)
{
    gameContext.onMove = 1;
    gameContext.moveNum = 1;
    gameContext.pgn = "";
    resetMsBoard();
    gameContext.firstPieceLiftedLocation = {-1, -1};
    gameContext.secondPieceLiftedLocation = {-1, -1};
    gameContext.isCapture = false;
    gameContext.isCastling = -1;
    changeState(GameStateEnum::IDLE, MoveStateEnum::IDLE, PieceStateEnum::IDLE);
}

/* resets helper variables that represent locations of first and second piece being lifted  */
void resetFirstAndSecondPieceLiftedLocations(GameContext &gameContext)
{
    gameContext.firstPieceLiftedLocation = {-1, -1};
    gameContext.secondPieceLiftedLocation = {-1, -1};
}

/* checks if 2 lifted pieces are King and Rook. We need that if castling is happening */
bool areKingAndRookLifted(GameContext &gameContext)
{
    int firstPiece = gameContext.msBoard[gameContext.firstPieceLiftedLocation.first][gameContext.firstPieceLiftedLocation.second];
    int secondPiece = gameContext.msBoard[gameContext.secondPieceLiftedLocation.first][gameContext.secondPieceLiftedLocation.second];
    if ((abs(firstPiece) == 4 && abs(secondPiece) == 6) || (abs(firstPiece) == 6 && abs(secondPiece) == 4))
        return true;
    return false;
}

void resetMsBoard()
{
    int initialBoard[8][8] = {
        {4, 2, 3, 6, 5, 3, 2, 4},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {-4, -2, -3, -6, -5, -3, -2, -4}};
    // Loop to copy initialBoard back to msBoard
    memcpy(gameContext.msBoard, initialBoard, sizeof(initialBoard));
}

int determineCastlingType(GameContext &gameContext)
{
    if ((gameContext.firstPieceLiftedLocation.first == 0 && gameContext.firstPieceLiftedLocation.second == 0) || (gameContext.secondPieceLiftedLocation.first == 0 && gameContext.secondPieceLiftedLocation.second == 0))
        return 0;
    else if ((gameContext.firstPieceLiftedLocation.first == 0 && gameContext.firstPieceLiftedLocation.second == 7) || (gameContext.secondPieceLiftedLocation.first == 0 && gameContext.secondPieceLiftedLocation.second == 7))
        return 1;
    else if ((gameContext.firstPieceLiftedLocation.first == 7 && gameContext.firstPieceLiftedLocation.second == 0) || (gameContext.secondPieceLiftedLocation.first == 7 && gameContext.secondPieceLiftedLocation.second == 0))
        return 2;
    return 3;
}

/* Function to check if castling positions are correctly placed */
bool isCastlingPlacementValid(GameContext &gameContext)
{
    if (gameContext.isCastling == 0)
        return (gameContext.piecePlacedLocations.at(0).first == 0 && gameContext.piecePlacedLocations.at(0).second == 1 && gameContext.piecePlacedLocations.at(1).first == 0 && gameContext.piecePlacedLocations.at(1).second == 2) || (gameContext.piecePlacedLocations.at(1).first == 0 && gameContext.piecePlacedLocations.at(1).second == 1 && gameContext.piecePlacedLocations.at(0).first == 0 && gameContext.piecePlacedLocations.at(0).second == 2);
    else if (gameContext.isCastling == 1)
        return (gameContext.piecePlacedLocations.at(0).first == 0 && gameContext.piecePlacedLocations.at(0).second == 5 && gameContext.piecePlacedLocations.at(1).first == 0 && gameContext.piecePlacedLocations.at(1).second == 4) || (gameContext.piecePlacedLocations.at(1).first == 0 && gameContext.piecePlacedLocations.at(1).second == 5 && gameContext.piecePlacedLocations.at(0).first == 0 && gameContext.piecePlacedLocations.at(0).second == 4);
    else if (gameContext.isCastling == 2)
        return (gameContext.piecePlacedLocations.at(0).first == 7 && gameContext.piecePlacedLocations.at(0).second == 2 && gameContext.piecePlacedLocations.at(1).first == 7 && gameContext.piecePlacedLocations.at(1).second == 1) || (gameContext.piecePlacedLocations.at(1).first == 7 && gameContext.piecePlacedLocations.at(1).second == 2 && gameContext.piecePlacedLocations.at(0).first == 7 && gameContext.piecePlacedLocations.at(0).second == 1);
    else if (gameContext.isCastling == 3)
        return (gameContext.piecePlacedLocations.at(0).first == 7 && gameContext.piecePlacedLocations.at(0).second == 5 && gameContext.piecePlacedLocations.at(1).first == 7 && gameContext.piecePlacedLocations.at(1).second == 4) || (gameContext.piecePlacedLocations.at(1).first == 7 && gameContext.piecePlacedLocations.at(1).second == 5 && gameContext.piecePlacedLocations.at(0).first == 7 && gameContext.piecePlacedLocations.at(0).second == 4);
    return false;
}

bool isPieceLiftedOnMove(GameContext &gameContext)
{
    int pieceLifted = gameContext.msBoard[gameContext.pieceLiftedLocations.at(0).first][gameContext.pieceLiftedLocations.at(0).second];
    return ((pieceLifted > 0 && gameContext.onMove == 1) || (pieceLifted < 0 && gameContext.onMove == 0));
}