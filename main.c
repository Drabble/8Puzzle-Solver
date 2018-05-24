#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashmap.c"

#define BOARD_SIDE 3
#define BOARD_LENGTH BOARD_SIDE * BOARD_SIDE
#define MAX_DEPTH 50
#define COLOR 0

/**
 * Print the square board
 * 
 * @param board the board array
 * @param size  the size of the board's side
 */
void print_board(const int *board, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int b = board[j + i * size];
            if (b == 0 && COLOR)
                printf("\033[1;31m");
            printf("%d ", b);
            if (b == 0 && COLOR)
                printf("\033[0m");
        }
        printf("\n");
    }
    printf("---\n");
}


/**
 * Check if the board is a solution. The correct solution is:
 *
 *      1 2 3
 *      4 5 6
 *      7 8 0
 * 
 * @param  board the board array to check
 * @return       1 if the solution is correct, 0 otherwise
 */
int check_solved(const int *board, int size) {
    if (board[size - 1] != 0)
        return 0;

    for (int i = 0; i < size - 2; ++i)
        if (board[i] != i + 1)
            return 0;

    return 1;
}

/**
 * Create a copy of an array and swap two of its elements 
 * @param origin pointer to the original array
 * @param dest   pointer to the destination array
 * @param size   size of the array
 * @param from   first element to swap
 * @param to     second element to swap
 */
void swap(const int *origin, int *dest, int size, int from, int to) {
    memcpy(dest, origin, size * sizeof(int));    
    int temp = dest[to];
    dest[to] = dest[from];
    dest[from] = temp;
}

/**
 * Sum the Manhattan distances for each cell of the board while swapping the cell 0 with the cell direction
 * @param board     pointer to the original array
 * @param length    size of the array
 * @param side      size of one side of the 2d array
 * @param direction element to swap with the cell 0
 * @return          Manhattan score
 */
size_t manhattanDistanceWithSwap(const int board[], const size_t length, const size_t side, int direction){
    size_t total = 0;
    size_t i;
    for(i = 0; i < length; i++){
        int xi = i % side;
        int yi = i / side;
        if(board[i] == 0){
            total += abs(xi - board[direction] % side) + abs(yi - board[direction] / side);
        } else if(i == direction){
            total += abs(xi - 0) + abs(yi - 0);
        } else{
            total += abs(xi - board[i] % side) + abs(yi - board[i] / side);
        }
    }
    return total;
}

typedef struct{
    int pos;
    int manhattanDistance;
} move;

/**
 * Compare the Manhattan scores of two moves
 * @param a_    move 1
 * @param b_    move 2
 * @return to   comparison between the two scores
 */
int cmpManhattanDistances(const void *a_, const void *b_)
{
    const move *a = a_, *b = b_;

    if(a->manhattanDistance == b->manhattanDistance)
        return 0;
    else if (a->manhattanDistance > b->manhattanDistance)
        return -1;
    else
        return 1;

}

/**
 * Solve the 8-puzzle using the recursive depth-first traversal
 * 
 * @param  board the board array
 * @param  depth the current depth of the recursion
 * @return       a boolean indicating if a solution is found (1 for found, 0 otherwise)
 */
int solve_dfs(const int board[], struct hashmap *explored, int depth) {
    if (check_solved(board, BOARD_LENGTH)){
        printf("solved!\n");
        return 1;
    }
    
    if (depth > MAX_DEPTH)
        return 0;

    // calculate position of the 0 (empty cell)
    int pos;
    for (pos = 0; pos < BOARD_LENGTH; pos++) if (board[pos] == 0) break;

    // compute the different moves, -1 if not possible
    move directions[4];
    directions[0].pos = pos % BOARD_SIDE == 0 ? -1 : pos - 1;
    directions[1].pos = pos % BOARD_SIDE == BOARD_SIDE - 1 ? -1 : pos + 1;
    directions[2].pos = pos - BOARD_SIDE;
    directions[3].pos = pos + BOARD_SIDE;

    // compute Manhattan distances
    for(int i = 0; i < 4; i++){
        directions[i].manhattanDistance = manhattanDistanceWithSwap(board, BOARD_LENGTH, BOARD_SIDE, directions[i].pos);
    }

    // sort by manhattan distance
    qsort(directions, 4, sizeof(move), cmpManhattanDistances);


    for (int i = 0; i < 4; i++) {
        int direction = directions[i].pos;
        if (direction >= 0 && direction < BOARD_LENGTH) {
            int* new_board = malloc(BOARD_LENGTH * sizeof(int));

            // swap the 0 with a possible index
            swap(board, new_board, BOARD_LENGTH, pos, direction);

            // if this configuration is not yet explored
            if (hashmap_set(explored, new_board))
            {
                // if a possible solution is found
                if (solve_dfs(new_board, explored, depth + 1))
                {
                    print_board(new_board, BOARD_SIDE);
                    free(new_board);
                    return 1;
                }
            }
            free(new_board);
        }
    }

    return 0;
}


int main(int argc, char const *argv[])
{
    int board[] = { 1, 2, 0, 3, 4, 5, 6, 7, 8};
    //int board[] = { 0, 5, 3, 4, 2, 7, 6, 8, 1};
    //int board[] = { 5,6,7,4,0,8,3,2,1};
    //int board[] = { 1,3,4,8,6,2,7,0,5};
    //int board[] = {1,2,3,8,0,4,7,6,5};

    struct hashmap *explored;
    explored = hashmap_create(100000000, BOARD_LENGTH); // TODO: Trouver une bonne taille pour la hashmap


    if (solve_dfs(board, explored, 0))
    {
        printf("solution found\n");
    } else {
        printf("no solution found\n");
    }

    hashmap_free(explored);

    return 0;
}