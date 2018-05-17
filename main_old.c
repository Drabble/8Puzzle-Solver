/**
 * Author: Antoine Drabble & Loic Stankovic
 * Date: 29.04.2018
 * Description: 8-Puzzle solver
 *              The board contains the number of each cell. The cell 0 corresponds to the empty space.
 */

#include <omp.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include "hashmap.c"

#define BOARD_SIZE 3

void swap(const int origin[], int *dest, int size, int from, int to) {
    memcpy(dest, origin, (int)size * sizeof(int));
    int tmp = dest[to];
    dest[to] = dest[from];
    dest[from] = tmp;
}

void shuffle(int *array, size_t n) {
    srand(time(NULL));
    if (n > 1) {
        size_t i;
        for (i = 1; i < n - 1; i++) {
            size_t j = rand() % (i + 1);
            if (j == 0)
                j = 1;

            swap(array, i, j);
        }
    }

}

void print_board(const int *board, size_t size) {
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            printf("%d ", board[j + i * size]);
        }
        printf("\n");
    }
}

int check_finished(const int *board, size_t size) {
    int solved = 1;
    for (int j = 0; j < BOARD_SIZE * BOARD_SIZE; j++) {
        if (j != board[j]) {
            solved = 0;
            break;
        }
    }

    return solved;
}

int dfs(struct hashmap *hm, const int board[], int depth) {

    // Find 0
    if (check_finished(board, BOARD_SIZE * BOARD_SIZE))
    {
        printf("Solved\n");
        print_board(board, BOARD_SIZE);
        printf("\n");
        return 1;
    }

    int pos;
    for (pos = 0; pos < BOARD_SIZE * BOARD_SIZE; pos++) if (board[pos] == 0) break;

    // Get possible moves
    int left = pos - BOARD_SIZE;
    int right = pos + BOARD_SIZE;
    int up = pos % BOARD_SIZE == 0 ? -1 : pos - 1;
    int down = pos % BOARD_SIZE == BOARD_SIZE - 1 ? -1 : pos + 1;
    int directions[] = {left, right, up, down};

    int i;
    for (i = 0; i < 4; i++) {
        if (directions[i] >= 0 && directions[i] < BOARD_SIZE * BOARD_SIZE) {
            int *newBoard = malloc(BOARD_SIZE * BOARD_SIZE * sizeof(int));
            newBoard = swap(&board, newBoard, BOARD_SIZE * BOARD_SIZE, pos, directions[i]);

            if (hashmap_set(hm, newBoard)) {

                if(dfs(hm, newBoard, depth + 1)){
                    free(newBoard);
                    return 1;
                }
            }
            free(newBoard);
            
        }
    }
    return 0;
}

// Hashmap
// Trouver fonction/heurisitique pour avoir un bon choix initial
// Trouver une première solution rapidement de façon à éliminer les autres
// On fait un dfs mais qui s'arrête quand il dépasse une longueur de parcours plus grande que la meilleur solution trouvée jusqu'à maintenant
int main(int argc, char *argv[]) {
    int board[] = { 1, 2, 0, 3, 4, 5, 6, 7, 8};
    //int board[] = {0,1,2,3,4,5,6,7,8};
    shuffle(board, (size_t) BOARD_SIZE * BOARD_SIZE);

    print_board(board, (size_t) BOARD_SIZE);

    struct hashmap *hm;
    hm = hashmap_create(64, BOARD_SIZE * BOARD_SIZE); // TODO: Trouver une bonne taille pour la hashmap

    if (dfs(hm, board, 0) == 1) {
        printf("Found the shortest path");
    } else {
        printf("Impossible level");
    }

    hashmap_free(hm);

    return EXIT_SUCCESS;
}
