#ifndef AST_H
#define AST_H

#define AST_MAX_CHILDREN 16
#define PROJECT_HEADER_MAX_CHILDREN 32
#include <stdbool.h>
#include "parser/lexer.h"

typedef enum
{
    ROOT,
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

typedef struct
{
    AST_TYPE type;
    void** children;
    int num_children;

    union 
    {
        char* str;
        char character;
        bool boolean;
        int integer;
        float floating_point;
    } data;
} ASTNode_t;

// Defines the intrinsic components for a project. UC3 will refuse to compile if any modules are missing.
typedef struct 
{
    char* component_dependencies[PROJECT_HEADER_MAX_CHILDREN];
} ProjectHeader_t;



ASTNode_t* init_ast(AST_TYPE type, char* value);
void ast_append(ASTNode_t* list, void* str);

#endif