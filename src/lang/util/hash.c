#include "lang/util/hash.h"
#include "lang/symbol/symbol.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////
// HASHING
////////////////////////////////////////////////////

// DBJ2 Hashing Algorithm
unsigned long hash(const char *key, unsigned long hash_limit) {
  uint32_t hash = 5381;
  uint32_t c;

  while (c = *key++)
    hash = ((hash << 5) + hash) + c;

  return hash % hash_limit;
}

// Constructor for hash tables
HashTable_t *init_hash_table(int hash_max) {
  HashTable_t *table = malloc(sizeof(HashTable_t));
  if (!table) {
    fprintf(stderr, "Error: Failed to allocate enough memory!");
    exit(EXIT_FAILURE);
  }

  table->hash_max = hash_max;
  table->contents = calloc(table->hash_max, sizeof(HashElement_t *));
  if (!table->contents) {
    fprintf(stderr, "Error: Failed to allocate memory!");
    return NULL;
  }

  table->num_elements = 0;
  return table;
}

HashElement_t *init_hash_element(void *value, const char *key) {
  HashElement_t *element = malloc(sizeof(HashElement_t));
  if (!element) {
    fprintf(stderr, "Error: Failed to allocate memory!");
    return NULL;
  }

  element->value = value;
  element->key = key;
  return element;
}

// Returns true and quits early if there is a duplicate
bool insert_hash(HashTable_t *table, void *value, const char *key) {
  table->num_elements++;

  uint32_t index = hash(key, table->hash_max);
  HashElement_t *symbol = init_hash_element(value, key);
  while (table->contents[index] != NULL) {
    if (strcmp(table->contents[index]->key, key) == 0)
      return true;

    index = (index + 1) % table->hash_max;
  }

  table->contents[index] = symbol;

  // Double hash limit if table runs out of space
  while (table->num_elements >= table->hash_max / 2) {
    table->hash_max *= 2;
    HashElement_t **tmp = malloc(sizeof(HashElement_t *) * table->hash_max);
    if (!tmp) {
      fprintf(stderr, "Error: Failed to allocate memory!\n");
      return false;
    }

    // Rehash all symbols
    for (int i = 0; i < table->hash_max / 2; i++) {
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

  return false;
}

// Get the index of a key in a hash table, if present. Returns ULONG_MAX is
// absent.
unsigned long get_hash_pos(HashTable_t *table, const char *key) {
  if (!table || !key)
    return ULONG_MAX;

  int index = hash(key, table->hash_max);

  while (table->contents[index] != NULL) {
    if (strcmp(table->contents[index]->key, key) == 0) {
      return index;
    }

    index = (index + 1) % table->hash_max;
  }

  return ULONG_MAX;
}

// Removes and frees a symbol from a hash table
void delete_symbol(HashTable_t *table, char *key) {
  if (get_hash_pos(table, key) == ULONG_MAX)
    return;

  unsigned long index = get_hash_pos(table, key);
  free(table->contents[index - 1]);
  table->contents[index - 1] = NULL;
}