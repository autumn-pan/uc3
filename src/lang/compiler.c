#include "lang/compiler.h"
#include "lang/reader.h"
#include "lang/parser/parser.h"
#include "lang/ast.h"
#include "lang/component/component.h"
#include "lang/util/hash.h"
#include <stdio.h>

void compile(char* file_name)
{
    char* src = read(file_name);
    Lexer* lexer = init_lexer(src);
    TokenStream* tokenstream = tokenize(lexer);
    ASTNode_t* root = parse(init_parser(tokenstream, lexer));

    HashTable_t* table = init_component_registry(root);
    append_component_dependencies(table);

    if(verify_components(table) == 1)
    {
        fprintf(stderr, "Error: Circular dependency detected!");
        exit(EXIT_FAILURE);
    }

    SymbolNode_t* symbol_table = symbolize_ast(root);
}