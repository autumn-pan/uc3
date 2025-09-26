#include "include/lang/parser/lexer.h"
#include <stdio.h>
#include <string.h>

int main()
{
    Lexer* lexer = init_lexer("hiii hello 123 bye");
    TokenStream* ts = tokenize(lexer);

    Token* token = ts->head;
    while(token)
    {
        printf("\nToken: %s", token->value),
        token = token->next;
    }
    return 0;
}