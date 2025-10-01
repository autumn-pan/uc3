#ifndef SYMBOL_H
#define SYMBOL_H
#include <stdlib.h>
#include <stdbool.h>
enum DATATYPE
{
    BOOLEAN,
    INTEGER,
    FLOAT,
    STRING,
    CHAR,
    TRISTATE_TYPE
};

enum TRISTATE
{
    FALSE = 0,
    NEUTRAL = -1,
    TRUE = 1
};

typedef struct
{
    char* identifier;
    bool constant;
    enum DATATYPE type;

    union
    {
        char* str;
        int integer;
        bool boolean;
        char character;
        float floating_point;
        enum TRISTATE tristate;
    } value;
} Symbol_t;

typedef struct symbolNode_t
{
    size_t num_children;
    struct symbolNode_t** children;
    Symbol_t** symbols;
} SymbolNode_t;

#endif