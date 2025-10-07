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
    union
    {
        // Str type should not be used, and is a debug value
        char* str; 
        int integer;
        bool boolean;
    } data;

    enum DATATYPE type;
} Value_t;

typedef struct
{
    char* identifier;
    bool constant;
    enum DATATYPE type;
    uint32_t hash;

    char* value;
} Symbol_t;

typedef struct symbolNode_t
{
    size_t num_children;
    struct symbolNode_t** children;
    HashTable_t* symbols;
} SymbolNode_t;

SymbolNode_t* symbolize_ast(ASTNode_t* node);
Symbol_t* init_symbol(enum DATATYPE type, char* identifier, char* value, bool constant);
Value_t* init_value(char* str, enum DATATYPE type);


#endif