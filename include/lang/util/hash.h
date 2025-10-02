#ifndef HASH_H
#define HASH_H
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    void* value;
    char* key;
    int index;
} HashElement_t;

typedef struct 
{
    uint8_t hash_max;

    uint8_t num_elements;
    HashElement_t** contents;
} HashTable_t;


HashTable_t* init_hash_table(int hash_max);
HashElement_t* init_hash_element(void* value, char* key);
bool insert_hash(HashTable_t *table, void * symbol, char* key);
unsigned long get_hash_pos(HashTable_t * table, char * key);


#endif