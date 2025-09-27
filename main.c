#include "include/lang/parser/lexer.h"
#include "lang/parser/parser.h"
#include <stdio.h>
#include <string.h>
#include "lang/ast.h"

int main()
{
    Lexer* lexer = init_lexer("FIELD hi DEFAULT 128");
    TokenStream* ts = tokenize(lexer);

    Token* token = ts->head;
    while(token)
    {
        printf("\nToken, %i: %s", token->type, token->value),
        token = token->next;
    }

    Parser_t* parser = init_parser(ts);
    BinaryASTNode_t* node = parse_statement(parser);
    if(!node)
    {
        printf("\nNode is NULL");
        return 0;
    }
    fflush(stdout);
    printf(node->data.str);
    return 0;
}