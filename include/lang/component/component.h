#ifndef COMPONENT_H
#define COMPONENT_H

#define COMPONENT_MAX_FIELDS 32

#include <string.h>
#include "lang/ast.h"
#include "lang/util/hash.h"
#include "lang/symbol/symbol.h"


typedef enum
{
    UNVISITED,
    ONGOING,
    COMPLETED
} COMPONENT_STATUS;

typedef enum
{
    UNGRAPHED,
    GRAPHED
} GRAPH_STATUS;

typedef struct
{
    char* identifier;
    ASTNode_t* expr;
    int value;
} Macro_t;

typedef struct
{
    Symbol_t* variable;
    int default_value;
} Field_t;

typedef struct component
{
    char* identifier;
    bool enabled;
    ASTNode_t* node;

    struct component** dependencies;
    int num_dependencies;

    // What components this component enables
    struct component** enables;

    // What values are configurable
    Field_t** fields;
    size_t num_fields;

    // C Macros associated with the component
    Macro_t** macros;
    size_t num_macros;
    
    // Metadata for graph verificaion
    COMPONENT_STATUS cyclic_status;
    GRAPH_STATUS graph_status;
} Component_t;

typedef struct {
    Component_t* root;
    Component_t** disjoint_components;
} ComponentGraph_t;

HashTable_t* init_component_registry(ASTNode_t* root);
Macro_t* init_macro(char* identifier, ASTNode_t* value);
bool append_component_dependencies(HashTable_t* registry);
bool verify_components(HashTable_t* table);
Macro_t* init_macro(char* identifier, ASTNode_t* value);
void macro_append(Component_t* node, Macro_t* child);
#endif