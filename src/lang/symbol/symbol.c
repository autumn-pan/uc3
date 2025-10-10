#include "lang/symbol/symbol.h"
#include <ctype.h>
#include "lang/ast.h"
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////
// Value_t
////////////////////////////////////////////////////////////////////

Value_t init_value(TYPE type, int data)
{
    Value_t value;
    value.type = type;
    value.value = data;

    return value;
}

////////////////////////////////////////////////////////////////////
// Util
////////////////////////////////////////////////////////////////////

// Translate a string to a boolean
bool str_to_bool(const char* str)
{
    if(strcmp(str, "true") == 0 || atoi(str) == 1)
        return true;
    return false;
}

// Translate a string into a Value_t
Value_t string_to_value(TYPE type, const char* str)
{
    if(type == INT_T)
        return init_value(INT_T, atoi(str));
    else if(type == BOOL_T)
        return init_value(BOOL_T, str_to_bool(str));
    else
        return init_value(UNKNOWN_T, 0);
}

// Constructor for a symbol
Symbol_t* init_symbol(Value_t value, const char* identifier, bool constant)
{
    Symbol_t* symbol = malloc(sizeof(Symbol_t));
    if(!symbol)
    {
        return NULL;
    }

    symbol->identifier = identifier;
    symbol->constant = constant;
    symbol->value = value;

    return symbol;
}

SymbolNode_t* init_symbol_node()
{
    SymbolNode_t* node = malloc(sizeof(SymbolNode_t));
    if(!node)
    {
        fprintf(stderr, "Error: Failed to allocate enough memory!");
        exit(EXIT_FAILURE);
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
                    init_value(UNKNOWN_T, 0),  
                    child->data.str,
                    false
                );    
            }
            else // Register defined symbols
            {
                symbol = init_symbol(
                    init_value(child->children[0]->type, atoi(child->children[0]->data.str)),
                    child->data.str,
                    false
                );
            }

            // Insert the symbol into the scope if it's not null
            if(insert_hash(symbol_node->symbols, symbol, symbol->identifier))
            {
                fprintf(stderr, "Error: Variable declaration error!");
                exit(EXIT_FAILURE);
            }
        }

        child_index++;
        if(child_index >= node->num_children)
            break;

        child = node->children[child_index];
    }
    return symbol_node;
}

// Return the int value of a variable
Value_t get_identifier_value(ASTNode_t* node, SymbolNode_t* symbol_table, SymbolNode_t* scope)
{
    char* identifier = node->data.str;

    uint8_t var_index;

    Symbol_t* symbol;
    /*  
    Currently, acope is two-layered. It's only possible to have the global scope,
    as well as one layer of scope for each component. Blocks and conditional trees
    will not be included in the first release.
    */

    // Check global index
    if((var_index = get_hash_pos(symbol_table->symbols, identifier)) == ULONG_MAX)
    {
        fprintf(stderr, "Error: Identifier %s%s", identifier, " is not defined in scope!");
        exit(EXIT_FAILURE);
    }
    else
    {
        symbol = (Symbol_t*)symbol_table->symbols->contents[var_index];
        return symbol->value;
    }

    // Check local scope
    if((var_index = get_hash_pos(scope->symbols, identifier)) == ULONG_MAX)
    {
        fprintf(stderr, "Error: Identifier %s%s", identifier, " is not defined in scope!");
        exit(EXIT_FAILURE);
    }
    else
    {
        symbol = (Symbol_t*)scope->symbols->contents[var_index];
        return symbol->value;
    }

    // Find the symbol associated with the identifier
    return init_value(UNKNOWN_T, 0);
}

// Evaluate an expression AST and return its integer value
int eval(ASTNode_t* node, SymbolNode_t* table, SymbolNode_t* scope)
{
    if(!node)
    {
        fprintf(stderr, "Error: expression can't be null!");
        exit(EXIT_FAILURE);
    }

    AST_TYPE type = node->type;

    printf("\nType: %i", type);
    fflush(stdout);
    
    if(type == INT_AST)
        return string_to_value(INT_T, node->data.str).value;
    else if(type == BOOL_AST)
        return string_to_value(BOOL_T, node->data.str).value;
    else if(type == IDEN_AST)
        return get_identifier_value(node, table, scope).value;

    ASTNode_t* left;
    ASTNode_t* right;

    // Error: Unrecognized expression node
    if(!node->children[0] || !node->children[1])
        return -1;

    left = node->children[0];
    right = node->children[1];

    // Binary operators (unary ones don't exist yet)
    switch(node->type)
    {
        case PLUS_AST:
            return eval(left, table, scope) + eval(right, table, scope);
        case MINUS_AST:
            return eval(left, table, scope) - eval(right, table, scope);
        case MULT_AST:
            return eval(left, table, scope) * eval(right, table, scope);
        case DIV_AST:
            if(right != 0)
                return eval(left, table, scope) / eval(right, table, scope);
            else
            {
                fprintf(stderr, "Error: Division by zero is undefined!");
                exit(EXIT_FAILURE);
            }
            break;
    }

    // Error, unrecognized expression
    fprintf(stderr, "Error: Unrecognized expression structure!");
    exit(EXIT_FAILURE);
}