#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

struct hashmap_item {
    struct hashmap_item *next;
    int *key;
};

struct hashmap {
    struct hashmap_item **buckets;
    size_t keySize;
    size_t size;
};

static int hash(const int *key, int keySize, int size) {
    int hash = 17;

    int i;
    for (i = 0; i < keySize; i++) {
        hash += key[i] * pow(10, i);
    }

    return hash % size;
}

struct hashmap *hashmap_create(size_t size, size_t keySize) {
    struct hashmap *hm = malloc(sizeof(struct hashmap));
    hm->buckets = calloc(size, sizeof(struct hashmap_item *));
    hm->keySize = keySize;
    hm->size = size;
    return hm;
}

int compare_key(const int *key1, const int *key2, int keySize) {
    int i;
    for (i = 0; i < keySize; i++) {
        if (key1[i] != key2[i]) {
            return 0;
        }
    }
    return 1;
}

int hashmap_set(struct hashmap *hm, int *key) {
    struct hashmap_item *item;
    int index = hash(key, hm->keySize, hm->size);

    for (item = hm->buckets[index]; item != NULL; item = item->next) {
        if (compare_key(item->key, key, hm->keySize) == 1) {/* key already exists */
            return 0;
        }
    }

    item = malloc(sizeof(struct hashmap_item));
    item->key = key;
    item->next = hm->buckets[index];
    hm->buckets[index] = item;

    return 1;
}

void hashmap_free(struct hashmap *hm) {
    struct hashmap_item *item;
    struct hashmap_item *next;

    int i;
    for (i = 0; i < hm->size; i++) {
        for (item = hm->buckets[i]; item != NULL;) {
            next = item->next;
            free(item);
            item = next;
        }
    }

    free(hm->buckets);
    free(hm);
}