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
    const char* identifier;
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
Symbol_t* init_symbol(Value_t value, const char* identifier, bool constant);
Value_t init_value(TYPE type, int data);
Value_t string_to_value(TYPE type, const char* str);

#endif
