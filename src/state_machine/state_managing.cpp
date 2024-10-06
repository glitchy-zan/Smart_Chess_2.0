#include <vector>
#include <state_machine/state_managing.h>

void monitorPieceLiftings();
void proccessGameStateIdle();
void proccessGameStateGameOver();
void proccessMoveStateIdle();
void proccessPieceStateIdle();
void proccessPieceStatePieceLifted();
void proccessPieceStateSecondPieceLifted();
void proccessPieceStateCastling();
void proccessPieceStateMultiplePiecesLifted();
void proccessPieceStateChanged();
void proccessMoveStateMoveMade();

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
static std::pair<int, int> firstPieceLiftedLocation = {-1, -1};
static std::pair<int, int> secondPieceLiftedLocation = {-1, -1};
bool isCapture = false;
// 0 -> wshort, 1 -> wlong, 2 -> bshort, 3 -> blong
int isCastling = -1;

void handleStates()
{
    if (flagButtonPressed)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::MOVE_MADE, PieceStateEnum::IDLE);
        flagButtonPressed = false;
    }
    switch (currentGameState)
    {
    case GameStateEnum::IDLE:
        proccessGameStateIdle();
        break;
    case GameStateEnum::RUNNING:
        monitorPieceLiftings();
        switch (currentMoveState)
        {
        case MoveStateEnum::IDLE:
            proccessMoveStateIdle();
            break;
        case MoveStateEnum::IN_PROGRESS:
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
            break;
        case MoveStateEnum::MOVE_MADE:
            proccessMoveStateMoveMade();
            break;
        default:
            break;
        }
        break;
    case GameStateEnum::GAME_OVER:
        proccessGameStateGameOver();
        break;

    default:
        break;
    }
}

/* monitors if pieces are lifted and saves locations in pieceLiftedLocations */
void monitorPieceLiftings()
{
    pieceLiftedLocations.clear();
    int num_pieces_last_position = numberOfPiecesOnBoard(msBoard);
    int num_pieces_now = numberOfPiecesOnBoard(sensorsBoard);
    /* if pieces lifted save them */
    if (num_pieces_last_position > num_pieces_now)
        pieceLiftedLocations = getLiftedLocations(sensorsBoard, msBoard);
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
            msBoard[i][j] = initialBoard[i][j];
        }
    }
}

/* when game state is IDLE we are checking for starting position. Once reached we change game state to RUNNING */
void proccessGameStateIdle()
{
    if (isStartingPosition(sensorsBoard))
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IDLE, PieceStateEnum::IDLE);
}

/* when websocket receives action for reset */
void proccessGameStateGameOver()
{
    onMove = 1;
    moveNum = 1;
    pgn = "";
    resetMsBoard();
    firstPieceLiftedLocation = {-1, -1};
    secondPieceLiftedLocation = {-1, -1};
    isCapture = false;
    changeState(GameStateEnum::IDLE, MoveStateEnum::IDLE, PieceStateEnum::IDLE);
}

/* when move state IDLE. We look at how many pieces are lifted (pieceLiftedLocations). */
/* TODO should check that piece lifted is from player that is on move */
void proccessMoveStateIdle()
{
    if (pieceLiftedLocations.size() == 1)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::PIECE_LIFTED);
        firstPieceLiftedLocation = pieceLiftedLocations.at(0);
    }
    if (pieceLiftedLocations.size() > 1)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
        firstPieceLiftedLocation = {-1, -1};
        secondPieceLiftedLocation = {-1, -1};
    }
}

void proccessPieceStateIdle()
{
    /* if one piece is lifted */
    /* TODO should check that piece lifted is from player that is on move */
    if (pieceLiftedLocations.size() == 1)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::PIECE_LIFTED);
        firstPieceLiftedLocation = pieceLiftedLocations.at(0);
    }
    /* if multiple pieces lifted at the same time it is not okay */
    if (pieceLiftedLocations.size() > 1)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
    }
}

void proccessPieceStatePieceLifted()
{
    /* if piece that was lifted is placed back on board */
    if (pieceLiftedLocations.size() == 0)
    {
        /* piece is placed on the same square */
        if (isSamePosition(msBoard, sensorsBoard))
        {
            changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::IDLE);
            firstPieceLiftedLocation = {-1, -1};
            secondPieceLiftedLocation = {-1, -1};
        }
        /* piece is placed somewhere else (normal move) */
        else
        {
            isCapture = false;
            changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::CHANGED);
        }
    }
    /* if second piece was lifted */
    /* TODO check that it is not same color piece lifted (unless castling) */
    if (pieceLiftedLocations.size() == 2)
    {
        /* store second location */
        /* pieceLiftedLocations is constantly updating and we can not be sure that firstaliftedPiece is at first place */
        secondPieceLiftedLocation = (pieceLiftedLocations.at(0) == firstPieceLiftedLocation) ? pieceLiftedLocations.at(1) : pieceLiftedLocations.at(0);
        /* if rook and king are lifted we go in castling state */
        int firstPiece = msBoard[firstPieceLiftedLocation.first][firstPieceLiftedLocation.second];
        int secondPiece = msBoard[secondPieceLiftedLocation.first][secondPieceLiftedLocation.second];
        if ((abs(firstPiece) == 4 && abs(secondPiece) == 6) || (abs(firstPiece) == 6 && abs(secondPiece) == 4))
            changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::CASTLING);
        else
            changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::SECOND_PIECE_LIFTED);
    }
    /* if too many pieces lifted */
    if (pieceLiftedLocations.size() > 2)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
        firstPieceLiftedLocation = {-1, -1};
        secondPieceLiftedLocation = {-1, -1};
    }
}

void proccessPieceStateSecondPieceLifted()
{
    /* both pieces cant be placed at the same time */
    if (pieceLiftedLocations.size() == 0)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
        firstPieceLiftedLocation = {-1, -1};
        secondPieceLiftedLocation = {-1, -1};
    }
    /* capture happened */
    /* TODO should make sure piece was placed to where second piece was lifted */
    if (pieceLiftedLocations.size() == 1)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::CHANGED);
        isCapture = true;
    }
    if (pieceLiftedLocations.size() > 2)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
        firstPieceLiftedLocation = {-1, -1};
        secondPieceLiftedLocation = {-1, -1};
    }
}

/* put pieces back to last move position to continue */
void proccessPieceStateMultiplePiecesLifted()
{
    if (pieceLiftedLocations.size() == 0 && isSamePosition(msBoard, sensorsBoard))
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::IDLE);
}

void proccessPieceStateCastling()
{
    /* we look which castling it is */
    if ((firstPieceLiftedLocation.first == 0 && firstPieceLiftedLocation.second == 0) || (secondPieceLiftedLocation.first == 0 && secondPieceLiftedLocation.second == 0))
        isCastling = 0;
    else if ((firstPieceLiftedLocation.first == 0 && firstPieceLiftedLocation.second == 7) || (secondPieceLiftedLocation.first == 0 && secondPieceLiftedLocation.second == 7))
        isCastling = 1;
    else if ((firstPieceLiftedLocation.first == 7 && firstPieceLiftedLocation.second == 0) || (secondPieceLiftedLocation.first == 7 && secondPieceLiftedLocation.second == 0))
        isCastling = 2;
    else
        isCastling = 3;

    if (pieceLiftedLocations.size() > 2)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
    }
    else if (pieceLiftedLocations.size() == 0)
    {
        piecePlacedLocations = getPlacedLocations(sensorsBoard, msBoard);
        switch (isCastling)
        {
            /* white short */
        case 0:
            if ((piecePlacedLocations.at(0).first == 0 && piecePlacedLocations.at(0).second == 1 && piecePlacedLocations.at(1).first == 0 && piecePlacedLocations.at(1).second == 2) || (piecePlacedLocations.at(1).first == 0 && piecePlacedLocations.at(1).second == 1 && piecePlacedLocations.at(0).first == 0 && piecePlacedLocations.at(0).second == 2))
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
            if ((piecePlacedLocations.at(0).first == 0 && piecePlacedLocations.at(0).second == 5 && piecePlacedLocations.at(1).first == 0 && piecePlacedLocations.at(1).second == 4) || (piecePlacedLocations.at(1).first == 0 && piecePlacedLocations.at(1).second == 5 && piecePlacedLocations.at(0).first == 0 && piecePlacedLocations.at(0).second == 4))
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
            if ((piecePlacedLocations.at(0).first == 7 && piecePlacedLocations.at(0).second == 2 && piecePlacedLocations.at(1).first == 7 && piecePlacedLocations.at(1).second == 1) || (piecePlacedLocations.at(1).first == 7 && piecePlacedLocations.at(1).second == 2 && piecePlacedLocations.at(0).first == 7 && piecePlacedLocations.at(0).second == 1))
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
            if ((piecePlacedLocations.at(0).first == 7 && piecePlacedLocations.at(0).second == 5 && piecePlacedLocations.at(1).first == 7 && piecePlacedLocations.at(1).second == 4) || (piecePlacedLocations.at(1).first == 7 && piecePlacedLocations.at(1).second == 5 && piecePlacedLocations.at(0).first == 7 && piecePlacedLocations.at(0).second == 4))
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
    if (pieceLiftedLocations.size() != 0 && !isCapture)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
        firstPieceLiftedLocation = {-1, -1};
        secondPieceLiftedLocation = {-1, -1};
    }
    else if (pieceLiftedLocations.size() != 1 && isCapture)
    {
        changeState(GameStateEnum::RUNNING, MoveStateEnum::IN_PROGRESS, PieceStateEnum::MULTIPLE_PIECES_LIFTED);
        firstPieceLiftedLocation = {-1, -1};
        secondPieceLiftedLocation = {-1, -1};
    }
}

void proccessMoveStateMoveMade()
{
    /* if castling happened */
    if (isCastling >= 0)
    {
        updatePgnCastling(onMove, moveNum, pgn, isCastling);
        if (isCastling == 0)
        {
            updateBoard(msBoard, 0, 3, 0, 1);
            updateBoard(msBoard, 0, 0, 0, 2);
        }
        else if (isCastling == 1)
        {
            updateBoard(msBoard, 0, 3, 0, 5);
            updateBoard(msBoard, 0, 7, 0, 4);
        }
        else if (isCastling == 2)
        {
            updateBoard(msBoard, 7, 3, 7, 1);
            updateBoard(msBoard, 7, 0, 7, 2);
        }
        else if (isCastling == 3)
        {
            updateBoard(msBoard, 7, 3, 7, 5);
            updateBoard(msBoard, 7, 7, 7, 4);
        }
        isCastling = -1;
    }
    /* normal move */
    else if (!isCapture)
    {
        std::pair<int, int> placedPosition;
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (msBoard[i][j] == 0 && sensorsBoard[i][j] != 0)
                {
                    placedPosition = {i, j};
                }
            }
        }

        /* if move not possible */
        if (false)
        {
        }

        updatePgn(onMove, moveNum, pgn, msBoard, firstPieceLiftedLocation.first, firstPieceLiftedLocation.second, placedPosition.first, placedPosition.second);
        updateBoard(msBoard, firstPieceLiftedLocation.first, firstPieceLiftedLocation.second, placedPosition.first, placedPosition.second);
    }
    /* capture */
    else if (isCapture)
    {
        /* if move not possible */
        if (false)
        {
        }
        updatePgn(onMove, moveNum, pgn, msBoard, firstPieceLiftedLocation.first, firstPieceLiftedLocation.second, secondPieceLiftedLocation.first, secondPieceLiftedLocation.second);
        updateBoard(msBoard, firstPieceLiftedLocation.first, firstPieceLiftedLocation.second, secondPieceLiftedLocation.first, secondPieceLiftedLocation.second);
    }
    changeOnMove(onMove, moveNum);
    changeState(GameStateEnum::RUNNING, MoveStateEnum::IDLE, PieceStateEnum::IDLE);
    firstPieceLiftedLocation = {-1, -1};
    secondPieceLiftedLocation = {-1, -1};
}