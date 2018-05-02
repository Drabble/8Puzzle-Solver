/**
 * Author: Antoine Drabble & Loic Stankovic
 * Date: 29.04.2018
 * Description: 8-Puzzle solver
 *              The board contains the number of each cell. The cell 0 corresponds to the empty space.
 */

#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 3

int* board;

/*struct Node{
    int*
};*/


void shuffle(int *board, size_t n)
{
    srand(time(NULL));
    if (n > 1) 
    {
        size_t i;
        for (i = 1; i < n - 1; i++) 
        {
            size_t j = rand() % (i + 1);
            if (j == 0)
                j = 1;

            int tmp = board[j];
            board[j] = board[i];
            board[i] = tmp;
        }
    }
}

void print_board(int *board, size_t size){
    for(size_t i = 0; i < size; i++){
        for (size_t j = 0; j < size; j++)
            printf("%d ", board[i + j * size]);
        printf("\n");
    }
}

/*void dfs(){
    #pragma omp parallel{
        #pragma omp for shared(board)

    }
}*/

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

    shuffle(board, (size_t) BOARD_SIZE * BOARD_SIZE);

    print_board(board, (size_t) BOARD_SIZE);

    //dfs();

    return EXIT_SUCCESS;
}
