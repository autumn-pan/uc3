#include "lang/parser/parser.h"
#include "lang/parser/lexer.h"
#include <stdlib.h>
#include <string.h>

Parser_t* init_parser(TokenStream* ts)
{
    Parser_t *parser = (Parser_t*)(malloc(sizeof(Parser_t)));
    if(!parser)
        return NULL;
    
    parser->ts = ts;

    
    parser->ts->ptr = parser->ts->head;
    parser->ts->ptr_position = 0;

    parser->value = parser->ts->ptr->value;
    return parser;
}

void advance_parser(Parser_t* parser)
{
    parser->ts->ptr = parser->ts->ptr->next;

    parser->ts->ptr_position++;
    parser->value = parser->ts->ptr->value;
}

BinaryASTNode_t* parse_field(Parser_t* parser)
{
    if(strcmp(parser->value, "FIELD") != 0)
        return NULL;

    BinaryASTNode_t* node = (BinaryASTNode_t*)(malloc(sizeof(BinaryASTNode_t)));
    advance_parser(parser);
    if(parser->ts->ptr->type != IDENTIFIER)
    {
        // Error: compiler should crash at this point
        return NULL;
    }

    advance_parser(parser);
    if(strcmp(parser->value, "DEFAULT") != 0)
    {
        // Error: defaults are required for UC3 definitions
        return NULL;
    }

    if(parser->ts->ptr->type != INT_LITERAL)
    {
        // Error: compiler should crash at this point
        return NULL;
    }

    node->data.str = parser->value;
    node->type = FIELD;
    node->children[0] = init_ast(DEFAULT, parser->ts->ptr->value);

    return node;
}

BinaryASTNode_t* parse_statement(Parser_t* parser)
{
    BinaryASTNode_t* node = NULL;
    if(node = parse_field(parser) != NULL)
        return node;
}