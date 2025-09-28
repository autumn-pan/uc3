#include "lang/ast.h"
#include <stdlib.h>
#include <stdio.h>

BinaryASTNode_t* init_ast(AST_TYPE type, char* value)
{
    BinaryASTNode_t* ast = (BinaryASTNode_t*)(malloc(sizeof(BinaryASTNode_t)));

    if(!ast)
    {
        return NULL;
    }

    // Set AST data based on the AST type
    ast->data.str = value;
}

BlockASTNode_t* init_block_ast(AST_TYPE type, char* identifier)
{
    BlockASTNode_t* block = (BlockASTNode_t*)(malloc(sizeof(BlockASTNode_t)));
    if(!block)
        return NULL;

    block->identifier = identifier;
    block->type = type;
    block->num_children = 0;
    return block;
}

ProjectRoot_t* init_root()
{
    ProjectRoot_t* root = (ProjectRoot_t*)(malloc(sizeof(ProjectRoot_t)));
    root->nodes = (BlockASTNode_t**)(calloc(0, sizeof(BlockASTNode_t*)));
    root->num_nodes = 0;

    return root;
}

ListASTNode_t* init_ast_list()
{
    ListASTNode_t* list = (ListASTNode_t*)(malloc(sizeof(ListASTNode_t)));
    list->children = (char**)(calloc(0, sizeof(char*)));
    list->num_children = 0;

    return list;
}

void ast_list_append(ListASTNode_t* list, char* str)
{
    list->num_children++;

    list->children = (char**)(realloc(list->children, list->num_children * sizeof(char*)));
    list->children[list->num_children - 1] = str;
}

void root_append_block(ProjectRoot_t* root, BlockASTNode_t* block)
{
    root->num_nodes++;

    root->nodes = (BlockASTNode_t**)(realloc(root->nodes, root->num_nodes * sizeof(BlockASTNode_t*)));
    root->nodes[root->num_nodes - 1] = block;
}