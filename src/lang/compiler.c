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

    ASTNode_t* root = parse(init_parser(tokenstream, lexer));
    HashTable_t* table = init_component_registry(root);

    ///////////////////////////////////////////
    // User Configuration should happen here //
    ///////////////////////////////////////////

    append_component_dependencies(table);

    if(verify_components(table) == 1)
    {
        fprintf(stderr, "Error: Circular dependency detected!");
        exit(EXIT_FAILURE);
    }

    SymbolNode_t* symbol_table = symbolize_ast(root);

    gen_config(table, symbol_table);
}

void gen_config(HashTable_t* component_registry, SymbolNode_t* global_symbols)
{
    // Create the file
    FILE* file = fopen("autoconfig.h", "w");
    if(!file)
    {
        fprintf(stderr, "Error: autoconfig.h failed to open!");
        exit(EXIT_FAILURE);
    }

    uint8_t num_macros = 0;
    for(int i = 0; i < component_registry->hash_max; i++)
    {
        // Get the next component
        HashElement_t* element = component_registry->contents[i];

        if(!element)
            continue;
        
        Component_t* component = (Component_t*)element->value;
        if(!component)
            continue;

        // Set up the component's macro registry
        parse_component_macros(component);

        // Write macro values to the config file
        for(int j = 0; j < component->num_macros; j++)
        {
            // Ensure that the macro exists
            Macro_t* macro = component->macros[j];
            if(!macro)
            {
                fprintf(stderr, "Error: Macro was improperly appended!");
                exit(EXIT_FAILURE);
            }

            // Evaluate the macro's value
            SymbolNode_t* local_scope = global_symbols->children[i];
            macro->value = eval(macro->expr, global_symbols, local_scope);
            
            // Print the macro to the file
            fprintf(file, "#define %s%s%i", macro->identifier, " ", macro->value);
            fprintf(file, "\n");
        }
    }
}