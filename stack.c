#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <string.h>

typedef struct stack
{
    int next;
    int size;
    int *values;
} stack;

stack* stack_create(int size)
{
    stack *s = malloc(sizeof(stack));
    s->size = size;
    s->next = 0;
    s->values = malloc(size * sizeof(int));

    return s;
}

int stack_is_full(stack *s)
{
    return s->next == s->size;
}

int stack_is_empty(stack *s)
{
    return s->next == 0;
}

void stack_push(stack* s, int v){
    if(stack_is_full(s))
        return;
    s->values[s->next] = v;
    s->next++;
}


int stack_pop(stack* s){
    if(stack_is_empty(s))
        return -1;

    s->next--;
    return s->next;
}

void stack_dump(stack *s, int *dest)
{
    memcpy(dest, s->values, s->size * sizeof(int));    
}

void stack_free(stack *s)
{
    free(s->values);
    free(s);
}