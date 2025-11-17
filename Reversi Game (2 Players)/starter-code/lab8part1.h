#ifndef SKELETON_H
#define SKELETON_H

#include <stdbool.h>

// Function Declarations for Part 1
void printBoard(char board[][26], int n);
bool positionInBounds(int n, int row, int col);
bool checkLegalInDirection(char board[][26],
                           int n,
                           int row,
                           int col,
                           char colour,
                           int deltaRow,
                           int deltaCol);

#endif
