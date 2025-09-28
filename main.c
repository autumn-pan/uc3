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
    BlockASTNode_t* node = parse_block(parser);
    if(!node)
    {
        printf("\nNode is NULL");
        return 0;
    }
    
    fflush(stdout);
    printf("\n");
    if(!node->children[0])
    {
        printf("\nNode has no children!");
        return 0;
    }

    printf("\n\nRoot: ");
    ProjectRoot_t* root = init_root();
    root_append_block(root, node);

    printf(root->nodes[0]->identifier);


}