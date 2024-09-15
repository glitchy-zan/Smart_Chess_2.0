#include <util/util.h>

/* helper function turning sensor outputs to array */
void unsignedCharToIntArray(unsigned char value, int *array)
{
    for (int i = 0; i < 8; i++)
    {
        array[i] = (value & (1 << (7 - i))) ? 1 : 0;
    }
}

/* looks at the sensor board and returns true if all pieces are on its starting squares. It can not look if pieces are placed
correctly since sensors only output 0 or 1 */
bool isStartingPosition(int sensors_board[8][8])
{
    for (int i = 0; i < 8; i++)
    {
        if (sensors_board[0][i] == 0 || sensors_board[1][i] == 0 || sensors_board[6][i] == 0 || sensors_board[7][i] == 0)
        {
            return false;
        }
    }
    return true;
}

/* given board having 0s and 1s it counts number of 1s (pieces) present */
int numberOfPiecesOnBoard(int board[8][8])
{
    int count = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] != 0)
                count++;
        }
    }
    return count;
}

/* get locations of squares where pieces were lifted; board lifted should be sensors board; board last state should be ms_board */
std::vector<std::pair<int, int>> getLiftedLocations(int board_lifted[8][8], int board_last_state[8][8])
{
    std::vector<std::pair<int, int>> lifted_locations;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board_lifted[i][j] == 0 && board_last_state[i][j] != 0)
            {
                lifted_locations.push_back(std::make_pair(i, j));
            }
        }
    }
    return lifted_locations;
}

/* compares 2 boards and checks if pieces are placed on same squares */
bool isSamePosition(int board_1[8][8], int board_2[8][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            /* can compare boards with 0|1 and boards -9|9 */
            if ((board_1[i][j] == 0 && board_2[i][j] != 0) || (board_1[i][j] != 0 && board_2[i][j] == 0))
            {
                return false;
            }
        }
    }
    return true;
}

/* happens when player makes a move */
void changeOnMove(int &onMove, int &moveNum)
{
    if (onMove == 0)
        onMove = 1;
    else
    {
        onMove = 0;
        moveNum = moveNum + 1;
    }
}

/* Updates board, normally msBoard */
void updateBoard(int board[8][8], int posStartRow, int posStartCol, int posEndRow, int posEndCol)
{
    int piece = board[posStartRow][posStartCol];
    board[posStartRow][posStartCol] = 0;
    board[posEndRow][posEndCol] = piece;
}

// Transforms column to file (A-H)
char file(int col)
{
    return 'a' + (7 - col);
}
// Transforms row to rank (1-8)
char rank(int row)
{
    return '1' + row;
}
// Creates String representing square
String squareToString(int row, int col)
{
    return String(file(col)) + String(rank(row));
}
// Transform piece number to character
char pieceChar(int piece)
{
    switch (abs(piece))
    {
    case 1:
        return 'P';
    case 2:
        return 'N';
    case 3:
        return 'B';
    case 4:
        return 'R';
    case 5:
        return 'Q';
    case 6:
        return 'K';
    default:
        return ' ';
    }
}
// Generates pgn for a move
String generatePgnMove(int board[8][8], int startRow, int startCol, int endRow, int endCol)
{
    int piece = board[startRow][startCol];
    int captured_piece = board[endRow][endCol];

    char piece_char_move = pieceChar(piece);
    String move = "";

    // Determine if it is a pawn move
    if (piece_char_move == 'P')
    {
        if (captured_piece != 0)
        {
            move += file(startCol); // For pawn captures, include the file of the pawn
            move += "x";
        }
        move += squareToString(endRow, endCol);
    }
    else
    {
        // Handle other pieces
        move += piece_char_move;

        // uncertainty :)
        if (startCol != endCol)
        {
            move += file(startCol);
        }
        else
        {
            move += rank(startRow);
        }

        if (captured_piece != 0)
        {
            move += "x";
        }

        move += squareToString(endRow, endCol);
    }
    // Handle promotions (not included in this basic example)
    // if (promotion) {
    //     move += "=";
    //     move += piece_char(promotion);
    // }

    return move;
}
// Updates pgn
void updatePgn(int &onMove, int &moveNum, String &pgn, int board[8][8], int startRow, int startCol, int endRow, int endCol)
{
    if (onMove == 1)
    {
        pgn += String(moveNum);
        pgn += ". ";
    }
    pgn += generatePgnMove(board, startRow, startCol, endRow, endCol);
    pgn += " ";
}