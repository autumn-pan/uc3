#include "lang/ast.h"
#include <stdlib.h>


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
