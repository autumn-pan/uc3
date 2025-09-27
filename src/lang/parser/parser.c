#include "lang/parser/parser.h"
#include "lang/parser/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
Parser_t* init_parser(TokenStream* ts)
{
    Parser_t *parser = (Parser_t*)(malloc(sizeof(Parser_t)));
    if(!parser)
        return NULL;
    
    parser->ts = ts;

    parser->pos = 0;
    parser->ptr = ts->head;

    return parser;
}

Token *get_next_token(Parser_t *p) {
    if (p->pos < p->len - 1) {
        return p->ptr->next;
    }
    return NULL;
}

char* match(Parser_t *p, enum TOKEN_TYPE type) {
    if (p->pos < p->len && p->ptr->type == type) {
        char* tmp = p->ptr->value;
        p->pos++;
        p->ptr = p->ptr->next;
        return  tmp;
    }
    return false;
}

// Match value function
bool match_value(Parser_t *p, char *value) {
    if (p->pos < p->len && strcmp(p->ptr->value, value) == 0) {
        p->pos++;
        p->ptr = p->ptr->next;
        return true;
    }

    return false;
}

BinaryASTNode_t* parse_field(Parser_t* parser)
{
    if(!match_value(parser, "FIELD"))
        return NULL;
    
    char* node_identifier;
    if(!(node_identifier = match(parser, IDENTIFIER)))
        return NULL;

    if(!match_value(parser, "DEFAULT"))
        return NULL;

    char* val;
    if(!(val = match(parser, INT_LITERAL)))
        return NULL;
    
    BinaryASTNode_t* node = init_ast(FIELD, node_identifier);
    node->children[0] = init_ast(INT, val);

    return node;
}

BinaryASTNode_t* parse_statement(Parser_t* parser)
{
    BinaryASTNode_t* node = NULL;
    if((node = parse_field(parser)) != NULL)
        return node;

    return NULL;
}