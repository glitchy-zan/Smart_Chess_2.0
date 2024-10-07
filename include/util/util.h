 #ifndef UTIL_H
#define UTIL_H

#include <Arduino.h>
#include <vector>

void unsignedCharToIntArray(unsigned char value, int *array);
bool isStartingPosition(int sensors_board[8][8]);
int numberOfPiecesOnBoard(int board[8][8]);
std::vector<std::pair<int, int>> getLiftedLocations(int board_lifted[8][8], int board_last_state[8][8]);
std::vector<std::pair<int, int>> getPlacedLocations(int board_placed[8][8], int board_last_state[8][8]);
bool isSamePosition(int board_1[8][8], int board_2[8][8]);
void changeOnMove(int &onMove, int &moveNum);
void updateBoard(int board[8][8], int posStartRow, int posStartCol, int posEndRow, int posEndCol);
char rank(int row);
char file(int col);
void updatePgn(int &onMove, int &moveNum, String &pgn, int board[8][8], int startRow, int startCol, int endRow, int endCol);
void updatePgnCastling(int &onMove, int &moveNum, String &pgn, int isCastling);

#endif // UTIL_H