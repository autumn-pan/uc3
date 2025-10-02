#ifndef SYMBOL_H
#define SYMBOL_H
#include <stdlib.h>
#include <stdbool.h>
#include "lang/ast.h"
enum DATATYPE
{
    BOOLEAN_T,
    INTEGER_T,
    FLOAT_T,
    STR_T,
    CHAR_T,
    TRISTATE_T,
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
    union
    {
        char* str;
        int integer;
        bool boolean;
        char character;
        float floating_point;
        TRISTATE tristate;
    } data;

    enum DATATYPE type;
} Value_t;

typedef struct
{
    char* identifier;
    bool constant;
    enum DATATYPE type;
    uint32_t hash;

    Value_t value;
} Symbol_t;

typedef struct symbolNode_t
{
    size_t num_children;
    size_t num_symbols;
    struct symbolNode_t** children;
    Symbol_t** symbols;
    uint8_t hash_limit;

} SymbolNode_t;


SymbolNode_t* symbolize_ast(ASTNode_t* node);

#endif