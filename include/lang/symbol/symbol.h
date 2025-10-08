#ifndef SYMBOL_H
#define SYMBOL_H
#include <stdlib.h>
#include <stdbool.h>
#include "lang/ast.h"
#include "lang/util/hash.h"

typedef enum 
{
    INT_T,
    BOOL_T,
    UNKNOWN_T
} TYPE;

typedef enum
{
    FALSE = 0,
    NEUTRAL = -1,
    TRUE = 1
} TRISTATE;

typedef struct 
{
    int value;
    TYPE type;
} Value_t;

typedef struct
{
    char* identifier;
    bool constant;
    uint32_t hash;

    Value_t value;
} Symbol_t;

typedef struct symbolNode_t
{
    size_t num_children;
    struct symbolNode_t** children;
    HashTable_t* symbols;
} SymbolNode_t;

SymbolNode_t* symbolize_ast(ASTNode_t* node);
Symbol_t* init_symbol(TYPE type, char* identifier, char* value, bool constant);
#endif
