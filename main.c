#include "include/lang/parser/lexer.h"
#include "lang/parser/parser.h"
#include <stdio.h>
#include <string.h>
#include "lang/ast.h"
#include <stdlib.h>

int main()
{
    Lexer* lexer = init_lexer("CONFIG block {SUBSYSTEM block_system FIELD field DEFAULT 16}");
    TokenStream* ts = tokenize(lexer);

    Token* token = ts->head;
    while(token)
    {
        printf("\nToken, %i: %s", token->type, token->value),
        token = token->next;
    }

    Parser_t* parser = init_parser(ts);
    BlockASTNode_t* node = parse_block(parser);
    if(!node)
    {
        printf("\nNode is NULL");
        return 0;
    }
    
    fflush(stdout);
    printf("\n");
    printf(node->children[0]->data.str);
    printf("\ntype: %i", node->type);
}