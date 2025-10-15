#ifndef COMPILER_H
#define COMPILER_H

#include "lang/util/hash.h"
#include "lang/symbol/symbol.h"

typedef struct
{
    char* file_name;
} Compiler;

void config(HashTable_t* components);
void compile(char* file_name);
void gen_config(HashTable_t* component_registry, SymbolNode_t* global_symbols);
#endif