#ifndef COMPILER_H
#define COMPILER_H

#include "lang/util/hash.h"

typedef struct
{
    char* file_name;
} Compiler;

void compile(char* file_name);
void gen_config(HashTable_t* component_registry);
#endif