#ifndef HASHMAP_H
#define HASHMAP_H

/**
 * The structure representing a hashmap list node
 */
typedef struct node
{
    int sentinel;
    int reversed_key;
    int value;
    int *item;
    struct node *next;
} node;

/**
 * The structure representing an hashmap
 */
typedef struct hashmap
{
    node ***primary_buckets;
    int item_count;
    int size;
} hashmap;

/**
 * An union type used to convert a node into an address, used for ABA problem
 */
typedef union
{
    node *n;
    int value;
} conversion;

hashmap *hashmap_create();
void hashmap_free(hashmap *hm);
int reverse(int value);
int hash(int *item);
int hashmap_initialize();
node *get_secondary_bucket(hashmap *hm, int key);
int hashmap_list_insert(node *head, node *n);
node *hashmap_initialize_bucket(hashmap *hm, int key);
int hashmap_find(node *head, int reversed_key, int is_sentinel, node **prev, node **crt);
int hashmap_insert(hashmap *hm,int* key, int value);
int hashmap_remove(int* key);
int hashmap_contains(int* key);

#endif