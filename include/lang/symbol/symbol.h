#ifndef SYMBOL_H
#define SYMBOL_H
#include <stdlib.h>
#include <stdbool.h>
#include "lang/ast.h"
#include "lang/util/hash.h"
enum DATATYPE
{
    BOOLEAN_T,
    INTEGER_T,
    STR_T,

    UNKNOWN_T
};

typedef enum
{
    FALSE = 0,
    NEUTRAL = -1,
    TRUE = 1
} TRISTATE;

typedef struct
{
    char* identifier;
    bool constant;
    enum DATATYPE type;
    uint32_t hash;

    int value;
} Symbol_t;

typedef struct symbolNode_t
{
    size_t num_children;
    struct symbolNode_t** children;
    HashTable_t* symbols;
} SymbolNode_t;

SymbolNode_t* symbolize_ast(ASTNode_t* node);
Symbol_t* init_symbol(enum DATATYPE type, char* identifier, char* value, bool constant);

#endif