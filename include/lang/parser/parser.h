#ifndef PARSER_H
#define PARSER_H
#include "lang/ast.h"
#include <stdlib.h>

typedef struct 
{
    int pos;
    int len;
    TokenStream* ts;

    size_t line;
    size_t column;
    
    Token* ptr;
} Parser_t;


Parser_t* init_parser(TokenStream* ts, Lexer* lexer);
ASTNode_t* parse(Parser_t* parser);

#endif
