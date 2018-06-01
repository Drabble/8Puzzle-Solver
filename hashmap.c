#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <limits.h>
#include "hashmap.h"

#define MAX_LOAD 2
#define KEY_SIZE 9

hashmap *hashmap_create()
{
    hashmap* new_hashmap = malloc(sizeof(hashmap));
    new_hashmap->size = 2;

    hashmap_initialize(new_hashmap);

    return new_hashmap;
}

void hashmap_free(hashmap *hm)
{
   
    free(hm->primary_buckets[0][0]);
    free(hm->primary_buckets[0]);
    free(hm->primary_buckets);
    free(hm);
}

int reverse(int value)
{
    int number = value;
    int res = number & 0x1;
    int s = sizeof(number) * CHAR_BIT - 1; 

    for (number >>= 1; number; number >>= 1)
    {   
      res <<= 1;
      res |= number & 0x1;
      s--;
    }
    return res <<= s; // shift when v's highest bits are zero
}

int hash(int *key) {
    int hash = 17;

    int i;
    for (i = 0; i < KEY_SIZE; i++) {
        hash += key[i] * pow(10, i);
    }

    return hash;
}

int hashmap_initialize(hashmap* hm)
{
    hm->primary_buckets = (node ***) calloc(48, sizeof(node **));
    if (hm->primary_buckets != NULL)
    {
        hm->primary_buckets[0] = (node **) calloc(hm->size, sizeof(node *));
        if (hm->primary_buckets[0] != NULL)
        {
            hm->primary_buckets[0][0] = (node *) calloc(hm->size, sizeof(node));
            if (hm->primary_buckets[0][0] != NULL)
            {
                hm->primary_buckets[0][0]->sentinel = 1;
                hm->primary_buckets[0][0]->next = NULL;

                return 1;
            }
            free(hm->primary_buckets[0]);
        }
        free(hm->primary_buckets);
    }
    return 0;
}

node *get_secondary_bucket(hashmap* hm, int key)
{
    int i, min, max;
    node **secondary, *sentinel;

    for (i = min = 0, max = 2; key >= max; i += 1, max <<= 1)
        min = max;

    if (hm->primary_buckets[i] == NULL)
    {
        secondary = (node **)calloc(hm->size >> 1, sizeof(node *));
        if(secondary == NULL) return NULL;

        if(!__sync_bool_compare_and_swap(&hm->primary_buckets[i], NULL, secondary))
            free(secondary);
    }

    if (hm->primary_buckets[i][key - min] == NULL)
        if ((sentinel = hashmap_initialize_bucket(hm, key)) != NULL)
            hm->primary_buckets[i][key - min] = sentinel;

    return hm->primary_buckets[i][key - min];
}

int hashmap_list_insert(node *head, node *n)
{
    node *prev, *crt;

    while(1)
    {
        if (hashmap_find(head, n->reversed_key, n->sentinel, &prev, &crt))
            // check if the current depth is lower than the one already stored
            if (n->value >= crt->value) 
               return 0; 
        
        n->next = crt;

        if(__sync_bool_compare_and_swap(&prev->next, crt, n))
            return 1;
    }
}

node *hashmap_initialize_bucket(hashmap* hm, int key)
{
    node *sentinel, *parent_bucket, *prev;
    int parent = hm->size;
    while((parent = parent >> 1) > key);

    parent_bucket = get_secondary_bucket(hm, key - parent);
    if ((sentinel = (node *)malloc(sizeof(node))) != NULL)
    {
        sentinel->sentinel = 1;
        sentinel->reversed_key = key = reverse(key);
        sentinel->next = NULL;
        if(!hashmap_list_insert(parent_bucket, sentinel))
        {
            free(sentinel);
            if (!hashmap_find(parent_bucket, key, 1, &prev, &sentinel))
            {
                return NULL;
            }
        }
    }
    return sentinel;
}



int hashmap_find(node *head, int reversed_key, int is_sentinel, node **prev, node **crt)
{
    conversion next;
    while(1)
    {
        *prev = head;
        *crt = (*prev)->next;
        while(1)
        {
            if (*crt == NULL)
                return 0;
            next.n = (*crt)->next;
            if (next.value & 0x1)
            {
                next.value ^= 0x1;
                if(__sync_bool_compare_and_swap(&(*prev)->next, crt, next.n))
                    *crt = next.n;
                else 
                    break;
            }
            else if((*crt)->reversed_key == reversed_key && is_sentinel == (*crt)->sentinel)
                return 1;
            else if((*crt)->reversed_key == reversed_key && is_sentinel)
                return 0;
            else if((*crt)->reversed_key > reversed_key)
                return 0;
            else{
                *prev = *crt;
                *crt  = next.n;
            }
        } 
    }
}

int hashmap_insert(hashmap* hm, int* item, int value)
{
    node *bucket, *new_node;
    int size, key;

    if ((new_node = (node *)malloc(sizeof(node))) != NULL)
    {
        new_node->sentinel = 0;
        key = hash(item);
        new_node->reversed_key = reverse(key);
        new_node->item = item;
        new_node->value = value;

        if ((bucket = get_secondary_bucket(hm, key % hm->size)) != NULL)
            if (hashmap_list_insert(bucket, new_node))
            {
                size = hm->size;
                if((__sync_fetch_and_add(&hm->item_count, 1) / size) >= MAX_LOAD)
                    __sync_bool_compare_and_swap(&hm->size, size, size * 2);

                return 1;
            }
        
        free(new_node);
    }

    return 0;
}