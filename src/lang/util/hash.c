#include "lang/util/hash.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>

////////////////////////////////////////////////////
// HASHING
////////////////////////////////////////////////////


// DBJ2 Algorithm
unsigned long hash(char * key, unsigned long hash_limit)
{
    uint32_t hash = 5381;
    uint32_t c;

    while (c = *key++)
        hash = ((hash << 5) + hash) + c;

    return hash % hash_limit;
}

HashTable_t* init_hash_table(int hash_max)
{
    HashTable_t* table = malloc(sizeof(HashTable_t));
    table->hash_max = hash_max;
    table->contents = malloc(sizeof(HashElement_t*)*table->hash_max);
    table->num_elements = 0;

    return table;
}

HashElement_t* init_hash_element(void* value, char* key)
{
    HashElement_t* element = malloc(sizeof(HashElement_t));

    element->value = value;
    element->key = key;

    return element;
}

void insert_hash(HashTable_t *table, void * value, char* key) 
{
    table->num_elements++;

    uint32_t index = hash(key, table->hash_max);
    HashElement_t* symbol = init_hash_element(value, key);

    while (table->contents[index] != NULL) {
        index = (index + 1) % table->hash_max;
    }
    

    table->contents[index] = symbol;

    // Double hash limit if table runs out of space
    while (table->num_elements >= table->hash_max / 2) 
    {
        table->hash_max *= 2;
        HashElement_t ** tmp = malloc(sizeof(HashElement_t*) * table->hash_max);

        // Rehash all symbols
        for (int i = 0; i < table->hash_max / 2; i++) 
        {
            if (table->contents[i] != NULL) {
                int new_index = hash(table->contents[i]->key, table->hash_max);

                // Handle collisions by linear probing
                while (tmp[new_index] != NULL) {
                    new_index = (new_index + 1) % table->hash_max;
                }

                tmp[new_index] = table->contents[i];
            }
        }

        free(table->contents);
        table->contents = tmp;
    }
}


unsigned long get_hash_pos(HashTable_t * table, char * key)
{
    int index = hash(key, table->hash_max);

    while (table->contents[index] != NULL) 
    {
        if (strcmp(table->contents[index]->key, key) == 0) 
        {
            return index;
        }

        index = (index + 1) % table->hash_max;
    }

    return ULONG_MAX;
}

void delete_symbol(HashTable_t * table, char * key)
{
    if(get_hash_pos(table, key) == ULONG_MAX)
        exit(EXIT_FAILURE);

    
    unsigned long index = get_hash_pos(table, key);

    //Free the symbol
    free(table->contents[index - 1]);

    //Clear the symbol
    table->contents[index - 1] = NULL;
}