#include <state_machine/move_state_managing.h>

void handleMoveState()
{
    switch (currentMoveState)
    {
        /* when move state is IDLE piece state is also IDLE and since in both cases same code is executing we can do it like this */
        /* TODO move state IDLE may maybe be removed */
    case MoveStateEnum::IDLE:
    case MoveStateEnum::IN_PROGRESS:
        handlePieceState();
        break;
    case MoveStateEnum::MOVE_MADE:
        proccessMoveStateMoveMade();
        break;
    default:
        break;
    }
}

/* when move state IDLE. We look at how many pieces are lifted (pieceLiftedLocations). */
/* TODO should check that piece lifted is from player that is on move */
/*
void proccessMoveStateIdle()
{
    if (gameContext.pieceLiftedLocations.size() == 1)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::PIECE_LIFTED);
        gameContext.firstPieceLiftedLocation = gameContext.pieceLiftedLocations.at(0);
    }
    if (gameContext.pieceLiftedLocations.size() > 1)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
        gameContext.firstPieceLiftedLocation = {-1, -1};
        gameContext.secondPieceLiftedLocation = {-1, -1};
    }
}
*/

void proccessMoveStateMoveMade()
{
    /* if castling happened */
    if (gameContext.isCastling >= 0)
    {
        updatePgnCastling(gameContext.onMove, gameContext.moveNum, gameContext.pgn, gameContext.isCastling);
        if (gameContext.isCastling == 0)
        {
            updateBoard(gameContext.msBoard, 0, 3, 0, 1);
            updateBoard(gameContext.msBoard, 0, 0, 0, 2);
        }
        else if (gameContext.isCastling == 1)
        {
            updateBoard(gameContext.msBoard, 0, 3, 0, 5);
            updateBoard(gameContext.msBoard, 0, 7, 0, 4);
        }
        else if (gameContext.isCastling == 2)
        {
            updateBoard(gameContext.msBoard, 7, 3, 7, 1);
            updateBoard(gameContext.msBoard, 7, 0, 7, 2);
        }
        else if (gameContext.isCastling == 3)
        {
            updateBoard(gameContext.msBoard, 7, 3, 7, 5);
            updateBoard(gameContext.msBoard, 7, 7, 7, 4);
        }
        gameContext.isCastling = -1;
    }
    /* normal move */
    else if (!gameContext.isCapture)
    {
        std::pair<int, int> placedPosition = getPlacedLocations(sensorsBoard, gameContext.msBoard).at(0);

        updatePgn(gameContext.onMove, gameContext.moveNum, gameContext.pgn, gameContext.msBoard, gameContext.firstPieceLiftedLocation.first, gameContext.firstPieceLiftedLocation.second, placedPosition.first, placedPosition.second);
        updateBoard(gameContext.msBoard, gameContext.firstPieceLiftedLocation.first, gameContext.firstPieceLiftedLocation.second, placedPosition.first, placedPosition.second);
    }
    /* capture */
    else if (gameContext.isCapture)
    {
        gameContext.isCapture = false;
        updatePgn(gameContext.onMove, gameContext.moveNum, gameContext.pgn, gameContext.msBoard, gameContext.firstPieceLiftedLocation.first, gameContext.firstPieceLiftedLocation.second, gameContext.secondPieceLiftedLocation.first, gameContext.secondPieceLiftedLocation.second);
        updateBoard(gameContext.msBoard, gameContext.firstPieceLiftedLocation.first, gameContext.firstPieceLiftedLocation.second, gameContext.secondPieceLiftedLocation.first, gameContext.secondPieceLiftedLocation.second);
    }
    resetFirstAndSecondPieceLiftedLocations(gameContext);
    changeOnMove(gameContext.onMove, gameContext.moveNum);
    changeState(GameStateEnum::RUNNING, MoveStateEnum::IDLE, PieceStateEnum::IDLE);
}