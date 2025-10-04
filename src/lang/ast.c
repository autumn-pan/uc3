#include "lang/ast.h"
#include <stdlib.h>
#include <stdio.h>
#include "lang/ast.h"

ASTNode_t* init_ast(AST_TYPE type, char* value)
{
    ASTNode_t* ast = (malloc(sizeof(ASTNode_t)));

    if(!ast)
    {
        fprintf(stderr, "Error: Failed to allocate enough memory!");
        exit(EXIT_FAILURE);
    }

    ast->children = (calloc(0, sizeof(ASTNode_t*)));
    ast->num_children = 0;
    
    // Set AST data based on the AST type
    ast->data.str = value;
    ast->type = type;
}

void ast_append(ASTNode_t* node, ASTNode_t* child)
{
    node->num_children++;

    node->children = (realloc(node->children, node->num_children * sizeof(ASTNode_t*)));
    if(!node->children)
    {
        fprintf(stderr, "Error: Failed to reallocate enough memory!");
        exit(EXIT_FAILURE);
    }
    node->children[node->num_children - 1] = child;
}