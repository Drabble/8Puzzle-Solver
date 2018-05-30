#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

typedef struct stack
{
    int size;
    int next;
    int *values;
} stack;

stack* stack_create(int size)
{
    stack *s = malloc(sizeof(stack));
    s->next = 0;
    s->values = malloc(size * sizeof(int));

    return s;
}

void stack_push(stack* s, int v){
    s->values[s->next] = v;
    s->next++;
}

void stack_pull(stack* s){
    s->next--;
    return s->next;
}