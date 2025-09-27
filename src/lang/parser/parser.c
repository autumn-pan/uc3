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
        p->pos++;
        p->ptr = p->ptr->next;
        return p->ptr;
    }
    return NULL;
}

bool match(Parser_t *p, enum TOKEN_TYPE type) {
    if (p->pos < p->len && p->ptr->type == type) {
        p->pos++;
        p->ptr = p->ptr->next;
        return true;
    }
    return false;
}

// Match value function
bool match_value(Parser_t *p, char *value) {
    if (p->pos < p->len && strcmp(p->ptr->value, value) == 0) {
        p->pos++;
        return true;
    }

    return false;
}

BinaryASTNode_t* parse_field(Parser_t* parser)
{
    return NULL;
}

BinaryASTNode_t* parse_statement(Parser_t* parser)
{
    BinaryASTNode_t* node = NULL;
    if((node = parse_field(parser)) != NULL)
        return node;

    return NULL;
}