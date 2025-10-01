#include "lang/symbol/symbol.h"
#include <ctype.h>
#include "lang/ast.h"
#include <string.h>



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
        case INT:
            symbol->value.integer = atoi(value);
            break;
        case STRING:
            symbol->value.str = value;
            break;
        case BOOLEAN:
            symbol->value.boolean = str_to_bool(value);
            break;
        default:
            return NULL;
            break;

    }
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

SymbolNode_t* symbolize_ast(ASTNode_t* node)
{
    SymbolNode_t* symbol_node = init_symbol_node();

    ASTNode_t* child = node->children[0];
    while(child)
    {
        if(child->type == BLOCK)
        {
            symbol_node_append(symbol_node, symbolize_ast(child));
        }
        
        else if(child->type == VARIABLE_DECL)
        {
            Symbol_t* symbol = init_symbol(
                child->children[0]->type,  
                child->data.str,
                child->children[0]->data.str,
                false
            );

            symbol_append(symbol_node, symbol);
        }
    }
}