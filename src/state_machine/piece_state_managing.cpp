#include <state_machine/piece_state_managing.h>

void handlePieceState()
{
    switch (currentPieceState)
    {
    case PieceStateEnum::IDLE:
        resetFirstAndSecondPieceLiftedLocations(gameContext);
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

void proccessPieceStateIdle()
{
    /* if one piece is lifted */
    /* TODO should check that piece lifted is from player that is on move */
    if (gameContext.pieceLiftedLocations.size() == 1)
    {
        // če je piece lifted notOnMove -> potential capture, second piece
        if (!isPieceLiftedOnMove(gameContext))
            gameContext.opponentsPieceLifted = true;
        gameContext.firstPieceLiftedLocation = gameContext.pieceLiftedLocations.at(0);
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::PIECE_LIFTED);
    }
    /* if multiple pieces lifted at the same time it is not okay */
    else if (gameContext.pieceLiftedLocations.size() > 1)
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
}

/************************************************************************************************/

void proccessPieceStatePieceLifted()
{
    /* if piece that was lifted is placed back on board */
    if (gameContext.pieceLiftedLocations.size() == 0)
    {
        /* piece is placed on the same square */
        if (isSamePosition(gameContext.msBoard, sensorsBoard))
            changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::IDLE);
        /* piece is placed somewhere else (normal move) */
        else
            changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::CHANGED);
    }
    /* if second piece was lifted */
    /* TODO check that it is not same color piece lifted (unless castling) */
    else if (gameContext.pieceLiftedLocations.size() == 2)
    {
        /* store second location */
        /* pieceLiftedLocations is constantly updating and we can not be sure that firstaliftedPiece is at first place */
        gameContext.secondPieceLiftedLocation = (gameContext.pieceLiftedLocations.at(0) == gameContext.firstPieceLiftedLocation) ? gameContext.pieceLiftedLocations.at(1) : gameContext.pieceLiftedLocations.at(0);
        /* if rook and king are lifted we go in castling state */
        if (areKingAndRookLifted(gameContext))
            changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::CASTLING);
        else
            changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::SECOND_PIECE_LIFTED);
    }
    /* if too many pieces lifted */
    else if (gameContext.pieceLiftedLocations.size() > 2)
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
}

void proccessPieceStateSecondPieceLifted()
{
    /* capture happened */
    /* TODO should make sure piece was placed to where second piece was lifted */
    if (gameContext.pieceLiftedLocations.size() == 1)
    {
        gameContext.isCapture = true;
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::CHANGED);
    }
    else if (gameContext.pieceLiftedLocations.size() == 0 || gameContext.pieceLiftedLocations.size() > 2)
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
}

void proccessPieceStateMultiplePiecesLifted()
{
    /* put pieces back to last move position to continue */
    if (gameContext.pieceLiftedLocations.size() == 0 && isSamePosition(gameContext.msBoard, sensorsBoard))
    {
        gameContext.opponentsPieceLifted = false;
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::IDLE);
    }
}

void proccessPieceStateCastling()
{
    gameContext.isCastling = determineCastlingType(gameContext);
    if (gameContext.pieceLiftedLocations.size() > 2)
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
    else if (gameContext.pieceLiftedLocations.size() == 0)
    {
        gameContext.piecePlacedLocations = getPlacedLocations(sensorsBoard, gameContext.msBoard);
        if (isCastlingPlacementValid(gameContext))
            changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::CHANGED);
        else
            changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
    }
}

/* from here we can only go to multiple pieces lifted unless move is made by button press */
void proccessPieceStateChanged()
{
    if (gameContext.pieceLiftedLocations.size() != 0 && !gameContext.isCapture)
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
    else if (gameContext.pieceLiftedLocations.size() != 1 && gameContext.isCapture)
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
}