#include <state_machine/piece_state_managing.h>

void proccessPieceStateIdle()
{
    /* if one piece is lifted */
    /* TODO should check that piece lifted is from player that is on move */
    if (gameContext.pieceLiftedLocations.size() == 1)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::PIECE_LIFTED);
        gameContext.firstPieceLiftedLocation = gameContext.pieceLiftedLocations.at(0);
    }
    /* if multiple pieces lifted at the same time it is not okay */
    if (gameContext.pieceLiftedLocations.size() > 1)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
    }
}

void proccessPieceStatePieceLifted()
{
    /* if piece that was lifted is placed back on board */
    if (gameContext.pieceLiftedLocations.size() == 0)
    {
        /* piece is placed on the same square */
        if (isSamePosition(gameContext.msBoard, sensorsBoard))
        {
            changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::IDLE);
            gameContext.firstPieceLiftedLocation = {-1, -1};
            gameContext.secondPieceLiftedLocation = {-1, -1};
        }
        /* piece is placed somewhere else (normal move) */
        else
        {
            gameContext.isCapture = false;
            changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::CHANGED);
        }
    }
    /* if second piece was lifted */
    /* TODO check that it is not same color piece lifted (unless castling) */
    if (gameContext.pieceLiftedLocations.size() == 2)
    {
        /* store second location */
        /* pieceLiftedLocations is constantly updating and we can not be sure that firstaliftedPiece is at first place */
        gameContext.secondPieceLiftedLocation = (gameContext.pieceLiftedLocations.at(0) == gameContext.firstPieceLiftedLocation) ? gameContext.pieceLiftedLocations.at(1) : gameContext.pieceLiftedLocations.at(0);
        /* if rook and king are lifted we go in castling state */
        int firstPiece = gameContext.msBoard[gameContext.firstPieceLiftedLocation.first][gameContext.firstPieceLiftedLocation.second];
        int secondPiece = gameContext.msBoard[gameContext.secondPieceLiftedLocation.first][gameContext.secondPieceLiftedLocation.second];
        if ((abs(firstPiece) == 4 && abs(secondPiece) == 6) || (abs(firstPiece) == 6 && abs(secondPiece) == 4))
            changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::CASTLING);
        else
            changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::SECOND_PIECE_LIFTED);
    }
    /* if too many pieces lifted */
    if (gameContext.pieceLiftedLocations.size() > 2)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
        gameContext.firstPieceLiftedLocation = {-1, -1};
        gameContext.secondPieceLiftedLocation = {-1, -1};
    }
}

void proccessPieceStateSecondPieceLifted()
{
    /* both pieces cant be placed at the same time */
    if (gameContext.pieceLiftedLocations.size() == 0)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
        gameContext.firstPieceLiftedLocation = {-1, -1};
        gameContext.secondPieceLiftedLocation = {-1, -1};
    }
    /* capture happened */
    /* TODO should make sure piece was placed to where second piece was lifted */
    if (gameContext.pieceLiftedLocations.size() == 1)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::CHANGED);
        gameContext.isCapture = true;
    }
    if (gameContext.pieceLiftedLocations.size() > 2)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
        gameContext.firstPieceLiftedLocation = {-1, -1};
        gameContext.secondPieceLiftedLocation = {-1, -1};
    }
}

/* put pieces back to last move position to continue */
void proccessPieceStateMultiplePiecesLifted()
{
    if (gameContext.pieceLiftedLocations.size() == 0 && isSamePosition(gameContext.msBoard, sensorsBoard))
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::IDLE);
}

void proccessPieceStateCastling()
{
    /* we look which castling it is */
    if ((gameContext.firstPieceLiftedLocation.first == 0 && gameContext.firstPieceLiftedLocation.second == 0) || (gameContext.secondPieceLiftedLocation.first == 0 && gameContext.secondPieceLiftedLocation.second == 0))
        gameContext.isCastling = 0;
    else if ((gameContext.firstPieceLiftedLocation.first == 0 && gameContext.firstPieceLiftedLocation.second == 7) || (gameContext.secondPieceLiftedLocation.first == 0 && gameContext.secondPieceLiftedLocation.second == 7))
        gameContext.isCastling = 1;
    else if ((gameContext.firstPieceLiftedLocation.first == 7 && gameContext.firstPieceLiftedLocation.second == 0) || (gameContext.secondPieceLiftedLocation.first == 7 && gameContext.secondPieceLiftedLocation.second == 0))
        gameContext.isCastling = 2;
    else
        gameContext.isCastling = 3;

    if (gameContext.pieceLiftedLocations.size() > 2)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
    }
    else if (gameContext.pieceLiftedLocations.size() == 0)
    {
        gameContext.piecePlacedLocations = getPlacedLocations(sensorsBoard, gameContext.msBoard);
        switch (gameContext.isCastling)
        {
            /* white short */
        case 0:
            if ((gameContext.piecePlacedLocations.at(0).first == 0 && gameContext.piecePlacedLocations.at(0).second == 1 && gameContext.piecePlacedLocations.at(1).first == 0 && gameContext.piecePlacedLocations.at(1).second == 2) || (gameContext.piecePlacedLocations.at(1).first == 0 && gameContext.piecePlacedLocations.at(1).second == 1 && gameContext.piecePlacedLocations.at(0).first == 0 && gameContext.piecePlacedLocations.at(0).second == 2))
            {
                changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::CHANGED);
            }
            else
            {
                changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
            }
            break;
            /* white long */
        case 1:
            if ((gameContext.piecePlacedLocations.at(0).first == 0 && gameContext.piecePlacedLocations.at(0).second == 5 && gameContext.piecePlacedLocations.at(1).first == 0 && gameContext.piecePlacedLocations.at(1).second == 4) || (gameContext.piecePlacedLocations.at(1).first == 0 && gameContext.piecePlacedLocations.at(1).second == 5 && gameContext.piecePlacedLocations.at(0).first == 0 && gameContext.piecePlacedLocations.at(0).second == 4))
            {
                changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::CHANGED);
            }
            else
            {
                changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
            }
            break;
            /* black short */
        case 2:
            if ((gameContext.piecePlacedLocations.at(0).first == 7 && gameContext.piecePlacedLocations.at(0).second == 2 && gameContext.piecePlacedLocations.at(1).first == 7 && gameContext.piecePlacedLocations.at(1).second == 1) || (gameContext.piecePlacedLocations.at(1).first == 7 && gameContext.piecePlacedLocations.at(1).second == 2 && gameContext.piecePlacedLocations.at(0).first == 7 && gameContext.piecePlacedLocations.at(0).second == 1))
            {
                changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::CHANGED);
            }
            else
            {
                changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
            }
            break;
            /* black long */
        case 3:
            if ((gameContext.piecePlacedLocations.at(0).first == 7 && gameContext.piecePlacedLocations.at(0).second == 5 && gameContext.piecePlacedLocations.at(1).first == 7 && gameContext.piecePlacedLocations.at(1).second == 4) || (gameContext.piecePlacedLocations.at(1).first == 7 && gameContext.piecePlacedLocations.at(1).second == 5 && gameContext.piecePlacedLocations.at(0).first == 7 && gameContext.piecePlacedLocations.at(0).second == 4))
            {
                changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::CHANGED);
            }
            else
            {
                changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
            }
        default:
            break;
        }
    }
}

/* from here we can only go to multiple pieces lifted unless move is made by button press */
void proccessPieceStateChanged()
{
    if (gameContext.pieceLiftedLocations.size() != 0 && !gameContext.isCapture)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
        gameContext.firstPieceLiftedLocation = {-1, -1};
        gameContext.secondPieceLiftedLocation = {-1, -1};
    }
    else if (gameContext.pieceLiftedLocations.size() != 1 && gameContext.isCapture)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
        gameContext.firstPieceLiftedLocation = {-1, -1};
        gameContext.secondPieceLiftedLocation = {-1, -1};
    }
}

void handlePieceState()
{
    switch (currentPieceState)
    {
    case PieceStateEnum::IDLE:
        proccessPieceStateIdle();
        break;
    case PieceStateEnum::PIECE_LIFTED:
        proccessPieceStatePieceLifted();
        break;
    case PieceStateEnum::SECOND_PIECE_LIFTED:
        proccessPieceStateSecondPieceLifted();
        break;
    case PieceStateEnum::CASTLING:
        proccessPieceStateCastling();
        break;
    case PieceStateEnum::MULTIPLE_PIECES_LIFTED:
        proccessPieceStateMultiplePiecesLifted();
        break;
    case PieceStateEnum::CHANGED:
        proccessPieceStateChanged();
        break;
    default:
        break;
    }
}