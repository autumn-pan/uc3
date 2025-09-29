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


ProjectRoot_t* init_root()
{
    ProjectRoot_t* root = (ProjectRoot_t*)(malloc(sizeof(ProjectRoot_t)));
    root->nodes = (ASTNode_t**)(calloc(0, sizeof(ASTNode_t*)));
    root->num_nodes = 0;

    return root;
}

void ast_append(ASTNode_t* node, void* child)
{
    node->num_children++;

    node->children = (void**)(realloc(node->children, node->num_children * sizeof(void*)));
    node->children[node->num_children - 1] = child;
}

void root_append_block(ProjectRoot_t* root, ASTNode_t* block)
{
    root->num_nodes++;

    root->nodes = (ASTNode_t**)(realloc(root->nodes, root->num_nodes * sizeof(ASTNode_t*)));
    root->nodes[root->num_nodes - 1] = block;
}