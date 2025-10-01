#ifndef COMPONENT_H
#define COMPONENT_H
#include <string.h>
#include "lang/ast.h"

typedef struct component
{
    char* identifier;
    ASTNode_t* node;

    struct component** dependencies;
    struct component** enables;
} Component_t;

#endif