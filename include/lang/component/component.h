#ifndef COMPONENT_H
#define COMPONENT_H
#include <string.h>
#include "lang/ast.h"

typedef enum GRAPH_COLOR
{
    WHITE,
    GREY,
    BLACK
};

typedef struct component
{
    char* identifier;
    ASTNode_t* node;

    struct component** dependencies;
    int num_dependencies;
    struct component** enables;
    enum GRAPH_COLOR color;
} Component_t;


typedef struct {
    Component_t* root;
    Component_t** disjoint_components;
} ComponentGraph_t;

#endif