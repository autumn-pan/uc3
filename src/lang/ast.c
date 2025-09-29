#include "lang/ast.h"
#include <stdlib.h>
#include <stdio.h>

ASTNode_t* init_ast(AST_TYPE type, char* value)
{
    ASTNode_t* ast = (ASTNode_t*)(malloc(sizeof(ASTNode_t)));

    if(!ast)
    {
        return NULL;
    }

    ast->children = (void**)(calloc(0, sizeof(void*)));
    // Set AST data based on the AST type
    ast->data.str = value;
}


void ast_append(ASTNode_t* node, void* child)
{
    node->num_children++;

    node->children = (void**)(realloc(node->children, node->num_children * sizeof(void*)));
    node->children[node->num_children - 1] = child;
}