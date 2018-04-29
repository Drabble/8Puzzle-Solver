/**
 * Author: Antoine Drabble & Loic Stankovic
 * Date: 29.04.2018
 * Description: 8-Puzzle solver
 *              The board contains the number of each cell. The cell 0 corresponds to the empty space.
 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 3

int* board;

struct Node{
    int*
};

int main(int argc, char *argv[]) {
    // Create 2d array
    /*int** board = (int**)malloc(BOARD_SIZE * sizeof(int*));
    int i, j;
    for(i = 0; i < BOARD_SIZE; i++){
        board[i] = (int*)malloc(BOARD_SIZE * sizeof(int));
        for(j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = i + j * BOARD_SIZE;
        }
    }*/
    board = (int*)malloc(BOARD_SIZE * BOARD_SIZE * sizeof(int));
    int i;
    for(i = 0; i < BOARD_SIZE * BOARD_SIZE; i++){
        board[i] = i;
    }

    dfs();

    return EXIT_SUCCESS;
}

void dfs(){
    #pragma omp parallel{
        #pragma omp for shared(board)

    }
}