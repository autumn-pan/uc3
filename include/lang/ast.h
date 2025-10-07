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
    ROOT_AST, // The root of the entire abstract syntax tree, contains every global definition
    HEADER_AST, // Declares a project's required components and subsystems
    DEFINITION_AST, // Declares a component
    SUBSYSTEM_AST, // Declares a subsystem
    CONFIGURATION_AST, // Defunct, but configures a component
    FIELD_AST, // Declares a value associated with a component that can be configured
    DEFAULT_AST, // Defines the default value for a FIELD. Defaults are required and non-optional.
    LIST_AST, // Stores an array of literals or identifiers
    IDEN_AST, // Refers to another subsystem or variable
    DEPENDENCY_AST, // Declares which components or subsystems a component needs. Analogous with KConfig depends_on
    PLACEHOLDER_AST,
    VARIABLE_DECL_AST,
    BLOCK_AST,
    MACRO_AST,

    INT_AST,
    BOOL_AST,

    // Operators
    AND_AST,
    NOT_AST,
    OR_AST,
    PLUS_AST,
    MINUS_AST,
    MULT_AST,
    DIV_AST,

    NULL_AST
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