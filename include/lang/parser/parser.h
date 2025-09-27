#ifndef PARSER_H
#define PARSER_H
#include "lang/ast.h"

typedef struct 
{
    TokenStream* ts;
    char* value;
} Parser_t;

#endif