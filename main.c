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

    Parser_t* parser = init_parser(ts, lexer);
    ASTNode_t* root = parse(parser);
    
    SymbolNode_t* symbol = symbolize_ast(root);

    Symbol_t* val = symbol->symbols->contents[get_hash_pos(symbol->symbols, "my_int")]->value;

    HashTable_t* registry = init_component_registry(root);
    append_component_dependencies(registry);

    for(int i = 0; i < registry->hash_max; i++)
    {
        if(registry->contents[i])
        {
            printf("\n%s",registry->contents[i]->key);
        }
    }

    fflush(stdout);

    Component_t* component = (Component_t*)registry->contents[get_hash_pos(registry, root->children[2]->data.str)]->value;


    printf("\nCyclic: %i", verify_components(registry));

    printf(component->identifier);
    fflush(stdout);

    printf(component->fields[1]->variable->identifier);
}