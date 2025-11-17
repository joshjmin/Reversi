#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "lab8part2.h"
#include "liblab8part2.h"
#include <sys/time.h>
#include <sys/resource.h>

#define UN 'U'
#define BLACK 'B'
#define WHITE 'W'
#define COMP 'C'
#define USER 'U'

//create basic board
void createBoard(char board[][26], int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            board[i][j] = UN;
        }
    }
    board[n/2-1][n/2-1] = WHITE;
    board[n/2][n/2] = WHITE;
    board[n/2-1][n/2] = BLACK;
    board[n/2][n/2-1] = BLACK; 
    }

//print board
void printBoard(char board[][26], int n){
    char start = 'a';
    printf("  ");
    for(int i = 0; i < n; i++){
        printf("%c", start);
        start++;
    }
    printf("\n");
    start = 'a';
    for(int i = 0; i < n; i++){
        printf("%c ", start);
        for(int j = 0; j < n; j++){
            printf("%c", board[i][j]);
        }
        printf("\n");
        start++;
    }
}


//check if the position is in bounds
bool positionInBounds(int n, int row, int col){
    if(row < n && row >= 0 && col < n && col >= 0){
        return true;
    } else {
        return false;
    }
}

//finds the opposite color of input
char findopposite(char color){
    if(color == BLACK){
        return WHITE;
    } else{
        return BLACK;
    }
}

//change turn to other player
void switchTurn(char* turn){
    if(*turn == COMP){
        *turn = USER;
    } else{
        *turn = COMP;
    }
}


//check if direction is in bounds and is not the same color for neighbouring tiles; then checks if same color is seen
bool checkLegalInDirection(char board[][26], int n, int row, int col, char color, int deltaRow, int deltaCol){

    if(!positionInBounds(n, row + deltaRow, col+deltaCol)|| board[row+deltaRow][col+deltaCol] != findopposite(color)){
        return false;
    } else{
        for(int i = 1; i < n; i++){
            if(row + i*deltaRow < 0 || col + i*deltaCol < 0 || row + i*deltaRow >= n || col+i*deltaCol >= n){
                return false;
            } else if(board[row+i*deltaRow][col+i*deltaCol] == UN){
                return false;
            } else if(board[row+i*deltaRow][col+i*deltaCol] == color){
                return true;
            }
        }
        return false;
    }
}

//returns true if move is valid; there is a legal direction and the position is not occupied
bool moveIsValid(char board[][26], int n, int row, int col, char color){
    bool check = false;

    if(board[row][col] != UN){
        return false;
    } else{
        for(int i = -1; i < 2; i++){
            for(int j = -1; j < 2; j++){
                if(!(i == 0 && j == 0)){
                   check = checkLegalInDirection(board, n, row, col, color, i, j);
                }
                if (check){
                    return true;
                }

            }
        }
        return check;
    }
}

//checks if the color has a valid move
bool hasValidMove(char board[][26], int n, char color) {
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (moveIsValid(board, n, i, j, color)){
            return true;
            }
        }
    }
    return false;
}

//flips the tiles if the color in direction is opposite
void flipTilesInDirection(char board[][26], int n, int row, int col, char color, int deltaRow, int deltaCol){  
    
    for(int i = 1; i < n; i++){
            if(board[row+i*deltaRow][col+i*deltaCol] == color || board[row+i*deltaRow][col+i*deltaCol] == UN){
                return;
            }
            if(board[row+i*deltaRow][col+i*deltaCol] == findopposite(color)){
                board[row+i*deltaRow][col+i*deltaCol] = color;
            }
        }
    }

//checks and flips the tile in every direction
void checkForFlipTiles(char board[][26], int n, int row, int col, char color){
    bool flip = false;

    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            if(!(i == 0 && j == 0)){
               flip = checkLegalInDirection(board, n, row, col, color, i, j);
               if(flip){
                    flipTilesInDirection(board, n, row, col, color, i, j);
               }
            }
        }
    }
}

//place move on board for trial
void applyMove(char board[][26], int n, int row, int col, char turn) {

    if(moveIsValid(board, n, row, col, turn)){
        board[row][col] = turn;
        checkForFlipTiles(board, n, row, col, turn);
    }
}

//calculate total pieces on board
int calculateScore(char board[][26], int n, int row, int col, char turn) {
    char opp;
    int total = 0;

    if (turn == BLACK){
        opp = WHITE;
    } else {
        opp = BLACK;
    }

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (!(i == 0 && j == 0)){
                int currentRow = row + i;
                int currentCol = col + j;
                int count = 0;

                while (positionInBounds(currentRow, currentCol, n) && board[currentRow][currentCol] == opp) {
                    count++;
                    currentRow += i;
                    currentCol += j;
                }

                if (positionInBounds(n, currentRow, currentCol) && board[currentRow][currentCol] == turn){
                    total += count;
                }
            }   
        }
    }
    return total;
}

//count the number of valid moves
int countValidMoves(char board[][26], int n, char turn) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 'U' && calculateScore(board, n, i, j, turn) > 0){
                count++;
            }
        }
    }
    return count;
}

//evluate the board and give total score on number of corners, adjacent pieces of corner, difference between colors, number of valid moves
int evaluateBoard(char board[][26], int n, char turn) {
    char opp;
    int cornerBonus = 0, adjacencyLose = 0, colorDiff = 0, mobility = 0;
    
    if(turn == BLACK){
        opp = WHITE;
    } else {
        opp = BLACK;
    }

    if (board[0][0] == turn || board[n-1][n-1] == turn || board[0][n-1] == turn || board[n-1][0] == turn){
        cornerBonus += 100;
    }else if(board[0][0] == opp || board[n-1][n-1] == opp || board[0][n-1] == opp || board[n-1][0] == opp){
        cornerBonus -= 100;
    }else {
        if (board[0][1] == turn || board [1][0] == turn || board[1][1] == turn || board[0][n-2] == turn || board[1][n-1] == turn || board[1][n-2] == turn || board[n-2][0] == turn || board[n-1][1] == turn || board[n-2][1] == turn || board[n-2][n-1] == turn || board[n-1][n-2] == turn || board[n-2][n-2] == turn){
            adjacencyLose -= 50;
            }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == turn){
                colorDiff++;
            } else if (board[i][j] == opp){
                colorDiff--;
            }
        }
    }

    mobility = countValidMoves(board, n, turn) - countValidMoves(board, n, opp);
    return (cornerBonus + adjacencyLose + colorDiff * 10 + mobility * 5);
}

//try to maximize user value and minimize comp value
int minimax(char board[][26], int n, char turn, bool maxPlayer, int score1, int score2, int moves) {
    int bestScore, score;
    char opp;
    char current;

    if (moves == 0) {
        return evaluateBoard(board, n, turn);
    }

    if (turn == BLACK){
        opp = WHITE;
    } else{
        opp = BLACK;
    }

    if (maxPlayer){
        current = turn;
    } else {
        current = opp;
    }

    if (maxPlayer) {
        bestScore = -10000;
    } else {
        bestScore = 10000;
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (moveIsValid(board, n, i, j, current)) {
                char newBoard[26][26];
                memcpy(newBoard, board, 26*26*sizeof(char));
                applyMove(newBoard, n, i, j, current);

                if (maxPlayer){
                    score = minimax(newBoard, n, turn, false, score1, score2, moves - 1);
                    if (score > bestScore){
                        bestScore = score;
                    }
                    if (bestScore > score1){
                        score1 = bestScore;
                    }
                } else {
                    score = minimax(newBoard, n, turn, true, score1, score2, moves - 1);
                    if (score < bestScore){
                        bestScore = score;
                    }
                    if (bestScore < score2){
                        score2 = bestScore;
                    }
                }

                if (score1 >= score2){
                    break;
                }
            }
        }
    }
    return bestScore;
}

//make move
int makeMove(const char board[][26], int n, char turn, int *row, int *col) {
    int bestScore = -10000;

    char firstBoard[26][26];
    memcpy(firstBoard, board, 26*26*sizeof(char));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (moveIsValid(firstBoard, n, i, j, turn)) {
                char newBoard[26][26];
                memcpy(newBoard, board, 26*26*sizeof(char));
                applyMove(newBoard, n, i, j, turn);

                int score = minimax(newBoard, n, turn, false, -10000, 10000, 5);
                
                if (score > bestScore) {
                    bestScore = score;
                    *row = i;
                    *col = j;
                }
            }
        }
    }
    return 0;
}

// int main(void){

//     struct rusage usage; // a structure to hold "resource usage" (including time)
//     struct timeval start, end; // will hold the start and end times
    
//     getrusage(RUSAGE_SELF, &usage);
//     start = usage.ru_utime;
//     double timeStart = start.tv_sec + 
//                        start.tv_usec / 1000000.0; // in seconds
    
//                        int makeMove(const char board[][26], int n, char turn, int *row, int *col) {
//                         int bestScore = -10000;
                    
//                         char firstBoard[26][26];
//                         memcpy(firstBoard, board, 26*26*sizeof(char));
                    
//                         for (int i = 0; i < n; i++) {
//                             for (int j = 0; j < n; j++) {
//                                 if (moveIsValid(firstBoard, n, i, j, turn)) {
//                                     char newBoard[26][26];
//                                     memcpy(newBoard, board, 26*26*sizeof(char));
//                                     applyMove(newBoard, n, i, j, turn);
                    
//                                     int score = minimax(newBoard, n, turn, false, -10000, 10000, 10);
                                    
//                                     if (score > bestScore) {
//                                         bestScore = score;
//                                         *row = i;
//                                         *col = j;
//                                     }
//                                 }
//                             }
//                         }
//                         return 0;
//                     }
    
//     getrusage(RUSAGE_SELF, &usage);
//     end = usage.ru_utime;
//     double timeEnd = end.tv_sec + 
//                      end.tv_usec / 1000000.0; // in seconds
    
//     double totalTime = timeEnd - timeStart;
//     printf("%lf", totalTime);
    
//     //totalTime now holds the time (in seconds) it takes to run your code
// }

// int main(void){
//     //initialize the variables
//     int n;
//     char compColor, userColor;
//     char turn = COMP;
//     char board[26][26];
//     int row, col = 0;

//     //prints original board
//     printf("Enter the board dimension: ");
//     scanf("%d", &n);

//     //prompts user to decide color
//     printf("Computer plays (B/W) : ");
//     scanf(" %c", &compColor);
//     createBoard(board, n);
//     printBoard(board, n);

//     //determine player colors
//     if(compColor == BLACK){
//         turn = COMP;
//         userColor = WHITE;
//     } else{
//         turn = USER;
//         userColor = BLACK;
//     }

//     //gameplay
//     while(hasValidMove(board, n, compColor) || hasValidMove(board, n, userColor)){

//         //decides if each player has any moves
//         if(turn == USER){
//             if(!hasValidMove(board, n, userColor)){
//                 printf("%c player has no valid move.\n", userColor);
//                 switchTurn(&turn);
//             }
//         } else{
//             if(!hasValidMove(board, n, compColor)){
//                 printf("%c player has no valid move.\n", compColor);
//                 switchTurn(&turn);
//             }
//         }

//         //player move on their turn
//         if(turn == COMP){
//             char firstBoard[26][26];
//             makeMove(firstBoard, n, turn, &row, &col);
//             printf("Computer places %c at %c%c.\n", compColor, 'a'+ row, 'a'+ col);
//             board[row][col] = compColor;
//             checkForFlipTiles(board, n, row, col, compColor);

//         } else {
//             findSmarterMove(board, n, userColor, &row, &col);
//             printf("Testing AI move (row, col): %c%c\n", row + 'a', col + 'a');
            
//             if(moveIsValid(board, n, row, col, userColor)){
//                 board[row][col] = userColor;
//                 checkForFlipTiles(board, n, row, col, userColor);
//             } else {
//                 printf("Invalid move.\n");
//                 printf("%c player wins.\n", compColor);
//                 return 0;
//             }
//         }
//         //print board and switch turn
//         printBoard(board, n);
//         switchTurn(&turn);
//     }

//     //counts total number of each color pieces on board
//     int blackCount = 0, whiteCount = 0;

//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < n; j++) {
//             if (board[i][j] == BLACK) {
//                 blackCount++;
//             }
//             if (board[i][j] == WHITE){
//                 whiteCount++;
//             }
//         }
//     }

//     //prints winner depending on number of pieces on board
//     if (blackCount > whiteCount){
//         printf("B player wins.\n");
//     } else if (whiteCount > blackCount){
//         printf("W player wins.\n");
//     }else {
//         printf("Draw!\n");
//     }
//     return 0;
// }