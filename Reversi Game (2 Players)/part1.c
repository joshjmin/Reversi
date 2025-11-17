#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "lab8part1.h"

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

//prints the available moves for input color if they are valid moves
void checkAvailableMoves(char board[][26], int n, char color){
    char start = 'a';

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if (moveIsValid(board, n, i, j, color)){
                printf("%c%c\n", start+i, start+j);
            }
        }
    }
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

//returns the number of flips in each direction
int flipScore(char board[][26], int n, int row, int col, char color, int deltaRow, int deltaCol){  
    int flipscore = 0;

    for(int i = 1; i < n; i++){
            if(board[row+i*deltaRow][col+i*deltaCol] == color || board[row+i*deltaRow][col+i*deltaCol] == UN){
                return flipscore;
            }
            if(board[row+i*deltaRow][col+i*deltaCol] == findopposite(color)){
                flipscore++;
            }
    }

    return flipscore;
}

//returns the total number of flips for each position
int countFlips(char board[][26], int n, int row, int col, char compColor) {
    bool flip = false;
    int score = 0;

    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            if(!(i == 0 && j == 0)){
                flip = checkLegalInDirection(board, n, row, col, compColor, i, j);
                if(flip){
                    score += flipScore(board, n, row, col, compColor, i, j);
                }
            }
        }
    }
    return score;
}

//compares total number of flips for each position
//chooses the best position and places on board
void getComputerMove(char board[][26], int n, char compColor){

    int bestRow, bestCol, score; 
    int bestScore = -1;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (moveIsValid(board, n, i, j, compColor)) {
                score = countFlips(board, n, i, j, compColor);
                if (score > bestScore) {
                    bestScore = score;
                    bestRow = i;
                    bestCol = j;
                }
            }
        }
    }
    printf("Computer places %c at %c%c.\n", compColor, 'a'+ bestRow, 'a'+ bestCol);
    board[bestRow][bestCol] = compColor;
    checkForFlipTiles(board, n, bestRow, bestCol, compColor);
}


int main(void){
    //initialize the variables
    int n;
    int userColNum, userRowNum = 0;
    char userRow, userCol, compColor, userColor;
    char turn = COMP;
    char board[26][26];

    //prints original board
    printf("Enter the board dimension: ");
    scanf("%d", &n);

    //prompts user to decide color
    printf("Computer plays (B/W) : ");
    scanf(" %c", &compColor);
    createBoard(board, n);
    printBoard(board, n);

    //determine player colors
    if(compColor == BLACK){
        turn = COMP;
        userColor = WHITE;
    } else{
        turn = USER;
        userColor = BLACK;
    }

    //gameplay
    while(hasValidMove(board, n, compColor) || hasValidMove(board, n, userColor)){

        //decides if each player has any moves
        if(turn == USER){
            if(!hasValidMove(board, n, userColor)){
                printf("%c player has no valid move.\n", userColor);
                switchTurn(&turn);
            }
        } else{
            if(!hasValidMove(board, n, compColor)){
                printf("%c player has no valid move.\n", compColor);
                switchTurn(&turn);
            }
        }

        //player move on their turn
        if(turn == COMP){
            getComputerMove(board, n, compColor);
        } else {
            printf("Enter move for colour %c (RowCol): ", userColor);
            scanf(" %c %c", &userRow, &userCol);
            userColNum = userCol - 'a';
            userRowNum = userRow - 'a';
            
            if(moveIsValid(board, n, userRowNum, userColNum, userColor)){
                board[userRowNum][userColNum] = userColor;
                checkForFlipTiles(board, n, userRowNum, userColNum, userColor);
            } else {
                printf("Invalid move.\n");
                printf("%c player wins.\n", compColor);
                return 0;
            }
        }
        //print board and switch turn
        printBoard(board, n);
        switchTurn(&turn);
    }

    //counts total number of each color pieces on board
    int blackCount = 0, whiteCount = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == BLACK) {
                blackCount++;
            }
            if (board[i][j] == WHITE){
                whiteCount++;
            }
        }
    }

    //prints winner depending on number of pieces on board
    if (blackCount > whiteCount){
        printf("B player wins.\n");
    } else if (whiteCount > blackCount){
        printf("W player wins.\n");
    }else {
        printf("Draw!\n");
    }
    return 0;
}