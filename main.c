#include "include/lang/parser/lexer.h"
#include "lang/parser/parser.h"
#include <stdio.h>
#include <string.h>
#include "lang/ast.h"
#include <stdlib.h>
#include "lang/reader.h"
#include "lang/symbol/symbol.h"
#include "lang/util/hash.h"
#include "lang/component/component.h"


int main()
{
    char* source = read("config.uc3");
    Lexer* lexer = init_lexer(source);
    TokenStream* ts = tokenize(lexer);

    printf(source);

    Token* token = ts->head;
    while(token)
    {
        printf("\nToken, %i: %s", token->type, token->value),
        fflush(stdout);
        token = token->next;
    }

    Parser_t* parser = init_parser(ts, lexer);
    ASTNode_t* root = parse(parser);
    
    SymbolNode_t* symbol = symbolize_ast(root);
    Symbol_t* sym = symbol->symbols[0];

    HashTable_t* registry = init_component_registry(root);
    append_component_dependencies(registry);

    Component_t* component = (Component_t*)registry->contents[get_hash_pos(registry, root->children[0]->data.str)]->value;


    printf("\nCyclic: %i", verify_components(registry));

    printf(component->fields[0]->variable->identifier);
}