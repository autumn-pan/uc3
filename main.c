#include "include/lang/parser/lexer.h"
#include <stdio.h>
#include <string.h>

int main()
{
    Lexer* lexer = init_lexer("DEFINE juice{}");
    TokenStream* ts = tokenize(lexer);

    Token* token = ts->head;
    while(token)
    {
        printf("\nToken, %i: %s", token->type, token->value),
        token = token->next;
    }
    return 0;
}