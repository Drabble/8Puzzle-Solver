#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

typedef struct hashmap_item {
    struct hashmap_item *next;
    int *key;
    int value;
} hashmap_item;

typedef struct hashmap {
    struct hashmap_item **buckets;
    size_t keySize;
    size_t size;
} hashmap;

static int hash(const int *key, int keySize, int size) {
    int hash = 17;

    int i;
    for (i = 0; i < keySize; i++) {
        hash += key[i] * pow(10, i);
    }

    return hash % size;
}

hashmap *hashmap_create(size_t size, size_t keySize) {
    hashmap *hm = malloc(sizeof(hashmap));
    hm->buckets = calloc(size, sizeof(hashmap_item *));
    hm->keySize = keySize;
    hm->size = size;
    return hm;
}

int compare_key(const int *key1, const int *key2, int keySize) {
    for (int i = 0; i < keySize; i++) {
        if (key1[i] != key2[i]) {
            return 0;
        }
    }
    return 1;
}

int hashmap_set_if_lower(hashmap *hm, int *key, int value) {
    hashmap_item *item;
    int index = hash(key, hm->keySize, hm->size);

    for (item = hm->buckets[index]; item != NULL; item = item->next) {
        if (compare_key(item->key, key, hm->keySize) == 1) {/* key already exists */
            if(item->value <= value){ // Lower value, don't replace
                return 0;
            } else{
                item->value = value;
                return 1;
            }
        }
    }

    item = malloc(sizeof(hashmap_item));
    item->key = malloc(hm->keySize * sizeof(int));
    memcpy(item->key, key, hm->keySize * sizeof(int));
    item->value = value;
    item->next = hm->buckets[index];
    hm->buckets[index] = item;

    return 1;
}

void hashmap_free(hashmap *hm) {
    hashmap_item *item;
    hashmap_item *next;

    int i;
    for (i = 0; i < hm->size; i++) {
        for (item = hm->buckets[i]; item != NULL;) {
            next = item->next;
            free(item->key);
            free(item);
            item = next;
        }
    }

    free(hm->buckets);
    free(hm);
}