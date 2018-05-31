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
    hashmap_item **buckets;
    omp_lock_t **locks;
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
    hm->locks = malloc(size * sizeof(omp_lock_t*));
    for(int i = 0; i < size; i++){
        omp_init_lock(&hm->locks[i]);
    }
    hm->keySize = keySize;
    hm->size = size;
    return hm;
}

int compare_key(const int *key1, const int *key2, int keySize) {
    int res = 1;
    for (int i = 0; i < keySize; i++) {
        if (key1[i] != key2[i]) {
            res = 0;
        }
    }
    return res;
}

int hashmap_set_if_lower(hashmap *hm, int *key, int value) {
    hashmap_item *item;
    int index = hash(key, hm->keySize, hm->size);

    for (item = hm->buckets[index]; item != NULL; item = item->next) {
        if (compare_key(item->key, key, hm->keySize) == 1) {/* key already exists */
            if(item->value <= value){ // Lower value, don't replace
                return 0;
            } else{
                omp_set_lock(&hm->locks[index]);
                item->value = value;
                omp_unset_lock(&hm->locks[index]);
                return 1;
            }
        }
    }

    item = malloc(sizeof(hashmap_item));
    item->key = malloc(hm->keySize * sizeof(int));
    memcpy(item->key, key, hm->keySize * sizeof(int));
    omp_set_lock(&hm->locks[index]);
        item->value = value;
        item->next = hm->buckets[index];
        hm->buckets[index] = item;
    omp_unset_lock(&hm->locks[index]);

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