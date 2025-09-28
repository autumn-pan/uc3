#include "include/lang/parser/lexer.h"
#include "lang/parser/parser.h"
#include <stdio.h>
#include <string.h>
#include "lang/ast.h"
#include <stdlib.h>
#include "lang/reader.h"

int main()
{
    char* source = read("config.uc3");
    printf(source);
    Lexer* lexer = init_lexer(source);
    TokenStream* ts = tokenize(lexer);

    printf(source);

    Token* token = ts->head;
    while(token)
    {
        printf("\nToken, %i: %s", token->type, token->value),
        token = token->next;
    }

    Parser_t* parser = init_parser(ts);
    ProjectRoot_t* root = parse(parser);

    printf(root->nodes[0]->children[0]->data.str);
    printf(root->nodes[1]->identifier);
}