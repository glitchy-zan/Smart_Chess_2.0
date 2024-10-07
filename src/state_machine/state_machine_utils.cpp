#include <state_machine/state_machine_utils.h>

/* monitors if pieces are lifted and saves locations in pieceLiftedLocations */
void monitorPieceLiftings()
{
    gameContext.pieceLiftedLocations.clear();
    int num_pieces_last_position = numberOfPiecesOnBoard(gameContext.msBoard);
    int num_pieces_now = numberOfPiecesOnBoard(sensorsBoard);
    /* if pieces lifted save them */
    if (num_pieces_last_position > num_pieces_now)
        gameContext.pieceLiftedLocations = getLiftedLocations(sensorsBoard, gameContext.msBoard);
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
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            gameContext.msBoard[i][j] = initialBoard[i][j];
        }
    }
}

void checkButtonPressed()
{
    if (flagButtonPressed)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::MOVE_MADE, PieceStateEnum::IDLE);
        flagButtonPressed = false;
    }
}

void resetGameVariables(int &onMove, int &moveNum, String &pgn, std::pair<int, int> &firstPieceLiftedLocation, std::pair<int, int> &secondPieceLiftedLocation, bool &isCapture, int &isCastling)
{
    onMove = 1;
    moveNum = 1;
    pgn = "";
    resetMsBoard();
    firstPieceLiftedLocation = {-1, -1};
    secondPieceLiftedLocation = {-1, -1};
    isCapture = false;
    isCastling = -1;
    changeState(GameStateEnum::IDLE, MoveStateEnum::IDLE, PieceStateEnum::IDLE);
}