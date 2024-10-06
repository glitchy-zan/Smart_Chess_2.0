#ifndef STATE_FRAMEWORK_H
#define STATE_FRAMEWORK_H

enum class GameStateEnum
{
    IDLE,
    RUNNING,
    GAME_OVER
};

enum class MoveStateEnum
{
    IDLE,
    IN_PROGRESS,
    MOVE_MADE
};

enum class PieceStateEnum
{
    IDLE,
    PIECE_LIFTED,
    SECOND_PIECE_LIFTED,
    MULTIPLE_PIECES_LIFTED,
    CASTLING,
    CHANGED
};

void changeState(GameStateEnum newGameState, MoveStateEnum newMoveState, PieceStateEnum newPieceState);

#endif // STATE_FRAMEWORK_H