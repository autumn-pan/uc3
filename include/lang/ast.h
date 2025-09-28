#ifndef AST_H
#define AST_H

#define AST_MAX_CHILDREN 16
#define PROJECT_HEADER_MAX_CHILDREN 32
#include <stdbool.h>
#include "parser/lexer.h"

typedef enum
{
    HEADER,
    DEFINITION,
    SUBSYSTEM_DEFINITION,
    CONFIGURATION,
    SUBSYSTEM,
    FIELD,
    DEFAULT,
    INT,
    STRING,
} AST_TYPE;

typedef struct node
{
    AST_TYPE type;
    struct node* children[2];
    union 
    {
        char* str;
        char character;
        bool boolean;
        int integer;
        float floating_point;
    } data;
} BinaryASTNode_t;

// Larger, n-ary AST nodes that are capable of holding n children. Used specifically for blocks
typedef struct
{
    AST_TYPE type;
    char* identifier;
    BinaryASTNode_t* children[AST_MAX_CHILDREN];
    uint8_t num_children;
} BlockASTNode_t;

// Defines the intrinsic components for a project. UC3 will refuse to compile if any modules are missing.
typedef struct 
{
    char* component_dependencies[PROJECT_HEADER_MAX_CHILDREN];
} ProjectHeader_t;

typedef struct 
{
    BlockASTNode_t** nodes;
    int num_nodes;
} ProjectRoot_t;


BinaryASTNode_t* init_ast(AST_TYPE type, char* value);
BlockASTNode_t* init_block_ast(AST_TYPE type, char* identifier);
ProjectRoot_t* init_root();
void root_append_block(ProjectRoot_t* root, BlockASTNode_t* block);

#endif