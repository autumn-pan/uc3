#include "lang/compiler.h"
#include "lang/reader.h"
#include "lang/parser/parser.h"
#include "lang/ast.h"
#include "lang/component/component.h"
#include "lang/util/hash.h"
#include <stdio.h>
#include "lang/symbol/symbol.h"

void compile(char* file_name)
{
    char* src = read(file_name);
    Lexer* lexer = init_lexer(src);
    TokenStream* tokenstream = tokenize(lexer);

    Token* token = tokenstream->head;

    while(token)
    {
        printf("\nToken: %s%s%i%s%i", token->value, ", ", token->line, ", ", token->column);
        token = token->next;
    }

    ASTNode_t* root = parse(init_parser(tokenstream, lexer));
    HashTable_t* table = init_component_registry(root);

    // After configuration, active modules will have their dependencies checked and verfied.
    /*
    append_component_dependencies(table);

    if(verify_components(table) == 1)
    {
        fprintf(stderr, "Error: Circular dependency detected!");
        exit(EXIT_FAILURE);
    }
    */

    SymbolNode_t* symbol_table = symbolize_ast(root);
}

void gen_config(HashTable_t* component_registry)
{
    Macro_t** macro_list = calloc(0, sizeof(Macro_t));
    uint8_t num_macros = 0;

    for(int i = 0; i < component_registry->hash_max; i++)
    {
        Component_t* component = (Component_t*)component_registry->contents[i]->value;

        if(!component)
            continue;

        ASTNode_t* block = component->node;
        for(int j = 0; j < block->num_children; j++)
        {
            ASTNode_t* child = block->children[j];
            if(child->type != MACRO_AST)
                continue;

            Macro_t* macro = init_macro(child->data.str, child->children[0]);
            if(!macro)
                return;

            num_macros++;
            macro_list = realloc(macro_list, sizeof(Macro_t)*num_macros);
            if(!macro_list)
            {
                fprintf(stderr, "Error: Memory allocation failed!");
                exit(EXIT_FAILURE);
            }

            macro_list[num_macros - 1] = macro;
        }
    }

    // Create the file
    FILE* file = fopen("autoconfig.h", "w");
    if(!file)
    {
        fprintf(stderr, "Error: autoconfig.h failed to open!");
        exit(EXIT_FAILURE);
    }


}

