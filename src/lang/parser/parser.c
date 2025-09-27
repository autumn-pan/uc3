#include "lang/parser/parser.h"
#include "lang/parser/lexer.h"
#include <stdlib.h>

Parser_t* init_parser(TokenStream* ts)
{
    Parser_t *parser = (Parser_t*)(malloc(sizeof(Parser_t)));
    if(!parser)
        return NULL;
    
    parser->ts = ts;
    return parser;
}

BinaryASTNode_t parse_field(Parser_t parser)
{
    
}

BinaryASTNode_t* parse_statement(Parser_t parser)
{

}