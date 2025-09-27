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
    switch(type)
    {
        case INT_LITERAL:
            ast->data.integer = atoi(value);
            break;
        case STRING_LITERAL:
            ast->data.str = value;
    }
}

BlockASTNode_t* init_block_ast(AST_TYPE type)
{
    BlockASTNode_t* block = (BlockASTNode_t*)(malloc(sizeof(BlockASTNode_t)));
    if(!block)
        return NULL;

    block->type = type;
    return block;
}
