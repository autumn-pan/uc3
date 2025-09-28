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
BinaryASTNode_t* parse_statement(Parser_t* parser);
BlockASTNode_t* parse_block(Parser_t* parser);
ProjectRoot_t* parse(Parser_t* parser);

#endif
