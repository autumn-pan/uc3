#include "lang/symbol/symbol.h"
#include <ctype.h>
#include "lang/ast.h"
#include <string.h>
#include <stdio.h>
#include <limits.h>

// Translate a string to a boolean
bool str_to_bool(char* str)
{
    if(strcmp(str, "true") == 0 || atoi(str) == 1)
        return true;
    return false;
}

// Translate a string into a tristate
TRISTATE str_to_tristate(char* str)
{
    if(strcmp(str, "neutral") == 0 || atoi(str) == -1)
        return NEUTRAL;
    else    
        return str_to_bool(str);
}

Value_t* init_value(char* str, enum DATATYPE type)
{
    Value_t* value = malloc(sizeof(value));

    // TODO: cover all datatypes
    switch(type)
    {
        case INTEGER_T:
            value->data.integer = atoi(str);
            break;
        case STR_T:
            value->data.str = str;
            break;
        case BOOLEAN_T:
            value->data.boolean = str_to_bool(str);
            break;
        default:
            value->data.str = str;
            break;
        }
}

// Constructor for a symbol
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
    node->symbols = init_hash_table(32);
    return node;
}

// Append a node as a child of another node; constructs a symbol tree
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
                continue;
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

                symbol = init_symbol(
                    child->children[0]->type,  
                    child->data.str,
                    child->children[0]->data.str,
                    false
                );
                
            }

            if(!symbol)
                return NULL;

            // Insert the symbol into the scope if it's not null
            insert_hash(symbol_node->symbols, symbol, symbol->identifier);
        }

        child_index++;
        if(child_index >= node->num_children)
            break;

        child = node->children[child_index];
    }
    printf("flag");
    fflush(stdout);
    return symbol_node;
}
