#ifndef PARSER_H
#define PARSER_H
#include "lang/ast.h"

typedef struct 
{
    int pos;
    int len;
    TokenStream* ts;
    
    Token* ptr;
} Parser_t;


Parser_t* init_parser(TokenStream* ts);
ASTNode_t* parse_statement(Parser_t* parser);
ASTNode_t* parse_block(Parser_t* parser);
ASTNode_t* parse(Parser_t* parser);

#endif
