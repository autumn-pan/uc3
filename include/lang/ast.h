#ifndef AST_H
#define AST_H

#define AST_MAX_CHILDREN 16
#define PROJECT_HEADER_MAX_CHILDREN 32
#include <stdbool.h>
#include "parser/lexer.h"

/*
A UC3 project is one that consists of a central project, with multiple subsystems.
Each subsystem each have multiple components attached.
A component can be any piece of code, whether central to the subsystem or an optional plugin. 
This is designed such that components are interchangable, leading to a highly modular system.
*/

// Types of abstract syntax tree nodes
typedef enum
{
    ROOT, // The root of the entire abstract syntax tree, contains every global definition
    HEADER, // Declares a project's required components and subsystems
    DEFINITION, // Declares a component
    SUBSYSTEM, // Declares a subsystem
    CONFIGURATION, // Defunct, but configures a component
    FIELD, // Declares a value associated with a component that can be configured
    DEFAULT, // Defines the default value for a FIELD. Defaults are required and non-optional.
    INT, // Integer data type
    STRING, // String data type
    IDENTIFIER, // Refers to another subsystem or variable
    DEPENDENCY // Declares which components or subsystems a component needs. Analogous with KConfig depends_on
} AST_TYPE;

typedef struct node
{
    AST_TYPE type;
    struct node** children;
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
void ast_append(ASTNode_t* node, ASTNode_t* child);

#endif