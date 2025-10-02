#ifndef COMPONENT_H
#define COMPONENT_H
#include <string.h>
#include "lang/ast.h"

typedef enum
{
    WHITE,
    GREY,
    BLACK
} GRAPH_COLOR;

typedef struct component
{
    char* identifier;
    ASTNode_t* node;

    struct component** dependencies;
    int num_dependencies;
    struct component** enables;
    GRAPH_COLOR color;
} Component_t;


typedef struct {
    Component_t* root;
    Component_t** disjoint_components;
} ComponentGraph_t;

typedef struct {
    Component_t** components;
} ComponentTable_t;

#endif