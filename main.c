#include "include/lang/parser/lexer/lexer.h"
#include <stdio.h>
int main()
{
    Lexer* lexer = init_lexer("hi hello 123 hi 123");

    TokenStream* tokenstream = tokenize(lexer);


    Token* token = tokenstream->head;
    while(token)
    {
        printf("\n Token: %s", token->value);
        token = token->next;
    }

    return 0;
}