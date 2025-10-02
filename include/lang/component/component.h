#ifndef COMPONENT_H
#define COMPONENT_H
#include <string.h>
#include "lang/ast.h"
#include "util/hash.h"

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

typedef struct component
{
    char* identifier;
    ASTNode_t* node;

    struct component** dependencies;
    int num_dependencies;
    struct component** enables;
    COMPONENT_STATUS cyclic_status;
    GRAPH_STATUS graph_status;

} Component_t;

typedef struct {
    Component_t* root;
    Component_t** disjoint_components;
} ComponentGraph_t;


#endif