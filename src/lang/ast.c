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

    ast->children = (ASTNode_t**)(calloc(0, sizeof(ASTNode_t*)));
    // Set AST data based on the AST type
    ast->data.str = value;
}


void ast_append(ASTNode_t* node, ASTNode_t* child)
{
    node->num_children++;

    node->children = (ASTNode_t**)(realloc(node->children, node->num_children * sizeof(ASTNode_t*)));
    node->children[node->num_children - 1] = child;
}