#include "lang/symbol/symbol.h"
#include <ctype.h>
#include "lang/ast.h"
#include <string.h>
#include <stdio.h>
#include <limits.h>

bool str_to_bool(char* str)
{
    if(strcmp(str, "true") == 0 || atoi(str) == 1)
        return true;
    return false;
}

Symbol_t* init_symbol(enum DATATYPE type, char* identifier, char* value, bool constant)
{
    Symbol_t* symbol = malloc(sizeof(Symbol_t));
    if(!symbol)
    {
        return NULL;
    }

    symbol->type = type;
    symbol->identifier = identifier;
    symbol->constant = constant;
    
    // TODO: cover all datatypes
    switch(type)
    {
        case INTEGER_T:
            symbol->value.integer = atoi(value);
            break;
        case STR_T:
            symbol->value.str = value;
            break;
        case BOOLEAN_T:
            symbol->value.boolean = str_to_bool(value);
            break;
        default:
            symbol->value.str = value;
            break;

    }

    return symbol;
}

SymbolNode_t* init_symbol_node()
{
    SymbolNode_t* node = malloc(sizeof(SymbolNode_t));
    if(!node)
    {
        return NULL;
    }

    node->num_children = 0;
    node->symbols = calloc(0, sizeof(Symbol_t));
    node->num_symbols = 0;
    return node;
}

void symbol_append(SymbolNode_t* node, Symbol_t* child)
{
    node->num_symbols++;

    node->symbols = (realloc(node->symbols, node->num_symbols * sizeof(Symbol_t*)));
    node->symbols[node->num_symbols - 1] = child;
}

void symbol_node_append(SymbolNode_t* node, SymbolNode_t* child)
{
    node->num_children++;

    node->children = (realloc(node->children, node->num_children * sizeof(SymbolNode_t*)));
    node->children[node->num_children - 1] = child;
}

// Create a Symbol Tree that corresponds to the variables of an Abstract Syntax Tree.
SymbolNode_t* symbolize_ast(ASTNode_t* node)
{
    // Initialize the root symbol
    SymbolNode_t* symbol_node = init_symbol_node();
    if(!symbol_node)
        return NULL;

    int child_index = 0;

    // Ensure that the child index is not out of bounds
    if(child_index >= node->num_children)
        return NULL;


    ASTNode_t* child = node->children[child_index];
    while(child)
    {
        // If the child node contains a block, the block will be its first and only child
        if(child->num_children > 0 && child->children[0]->type == BLOCK_AST)
        {
            SymbolNode_t* new_symbol = symbolize_ast(child->children[0]);
            if(!new_symbol)
                return NULL;
            symbol_node_append(symbol_node, new_symbol);

        }
        else if(child->type == VARIABLE_DECL_AST)
        {
            Symbol_t* symbol;
            if(!child->children[0]) // Register undefined symbols
            {
                symbol = init_symbol(
                    UNKNOWN_T,  
                    child->data.str,
                    NULL,
                    false
                );    
            }
            else // Register defined symbols
            {
                Symbol_t* symbol = init_symbol(
                    child->children[0]->type,  
                    child->data.str,
                    child->children[0]->data.str,
                    false
                );
            }

            if(!symbol)
                return NULL;

            symbol_append(symbol_node, symbol);

        }


        child_index++;

        if(child_index >= node->num_children)
            break;

        child = node->children[child_index];
    }

    return symbol_node;
}
