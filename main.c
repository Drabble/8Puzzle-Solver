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
#include <stddef.h>
#include <math.h>

#define BOARD_SIZE 3

struct queue_item {
    int* content;
    struct queue_item* next;
    struct queue_item* prec;
};
struct queue_root {
    struct queue_item* head;
    struct queue_item* tail;
};

void init_queue(struct queue_root* queue){
    queue->head = queue->tail = NULL;
}

void push_queue(struct queue_root* queue, int* content, struct queue_item* prec){
    struct queue_item *item = malloc(sizeof(struct queue_item));
    item->content = content;
    item->next = NULL;
    item->prec = prec;
    if (queue->head == NULL){
        queue->head = queue->tail = item;
    } else {
        queue->tail = queue->tail->next = item;
    }
}

int exists_queue(struct queue_root* queue, const int* content, int contentSize){
    struct queue_item* item = queue->head;
    while(item != NULL){
        int same = 1;
        int i;
        for(i = 0; i < contentSize; i++){
            if(item->content[i] != content[i]){
                same = 0;
                break;
            }
        }
        if(same == 1){
            return 1;
        }
        item = item->next;
    }
    return 0;
}

void shuffle(int *array, size_t n)
{
    srand(time(NULL));
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
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

int bfs(struct queue_root* q){
    struct queue_item* current = q->head;

    while(current != NULL){
        // Find 0
        int pos;
        for(pos = 0; pos < BOARD_SIZE * BOARD_SIZE; pos++) if(current->content[pos] == 0) break;
        // Get possible moves
        int left = pos - BOARD_SIZE;
        int right = pos + BOARD_SIZE;
        int up = pos % BOARD_SIZE == 0 ? -1 : pos - 1 ;
        int down = pos % BOARD_SIZE == BOARD_SIZE - 1 ? -1 : pos + 1;

        int directions[] = {left, right, up, down};

        int i;
        for(i = 0; i < 4; i++){
            if(directions[i] >= 0 && directions[i] < BOARD_SIZE * BOARD_SIZE){
                int* newBoard = malloc(BOARD_SIZE * BOARD_SIZE * sizeof(int));
                int j;
                for(j = 0; j < BOARD_SIZE * BOARD_SIZE; j++){
                    if(j == pos){
                        newBoard[j] = current->content[directions[i]];
                    } else if(j == directions[i]){
                        newBoard[j] = current->content[pos];
                    } else {
                        newBoard[j] = current->content[j];
                    }
                }
                int solved = 1;
                for(j = 0; j < BOARD_SIZE * BOARD_SIZE; j++){
                    if(j != newBoard[j]){
                        solved = 0;
                        break;
                    }
                }
                if(solved == 1){
                    printf("Solved\n");
                    printf("Path is (in reverse order): \n");
                    print_board(newBoard, BOARD_SIZE);
                    printf("\n");
                    struct queue_item* path = current;
                    while(path != NULL){
                        print_board(path->content, BOARD_SIZE);
                        printf("\n");
                        path = path->prec;
                    }
                    return 1;
                }
                if(exists_queue(q, newBoard, BOARD_SIZE * BOARD_SIZE) == 0){
                    push_queue(q, newBoard, current);
                } else{
                    free(newBoard);
                }
            }
        }
        current = current->next;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int board[] = {1,0,2,3,4,5,6,7,8};

    //shuffle(board, (size_t) BOARD_SIZE * BOARD_SIZE);

    print_board(board, (size_t) BOARD_SIZE);

    struct queue_root* q = malloc(sizeof(struct queue_root));
    init_queue(q);

    push_queue(q, board, NULL);

    if(bfs(q) == 1){
        printf("Found the shortest path");
    } else{
        printf("Impossible level");
    }

    // TODO: Free the QUEUE?

    return EXIT_SUCCESS;
}
