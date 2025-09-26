#include "include/lang/parser/lexer/lexer.h"
#include <stdio.h>
#include <string.h>

int main()
{
    Lexer* lexer = init_lexer("hiii hello 123 bye");
    TokenStream* ts = tokenize(lexer);

    return 0;
}