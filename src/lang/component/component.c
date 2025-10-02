#include "lang/component/component.h"
#include <stdlib.h>


Component_t* init_component(ASTNode_t* node)
{
    if(!node)
        return NULL;

    Component_t* component = malloc(sizeof(Component_t));

    if(!component)
        return NULL;

    component->identifier = node->data.str;
    component->node = node;

    return component;
}

// Returns true if a component graph is cyclic
bool check_cycles(Component_t* node)
{
    node->color = GREY;
    for(int i = 0; i < node->num_dependencies; i++)
    {
        bool cyclic = false;

        if(node->dependencies[i]->color == GREY)
            return true;

        else if(check_cycles(node->dependencies[i]))
            return true;
    }

    node->color = BLACK;
    return false;
}