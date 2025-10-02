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
    component->num_dependencies = 0;
    component->dependencies = calloc(0, sizeof(Component_t*));

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
/* This portion cannot be completed without a tested hash table system.
ComponentGraph_t* init_component_graph(ASTNode_t* root)
{
    if(root->type != DEFINITION_AST)
        return NULL;

    ComponentGraph_t* graph = malloc(sizeof(ComponentGraph_t));

    ASTNode_t* dependency_node;

    for(int i = 0; i < root->num_children; i++)
    {
        if(root->children[i]->type == DEPENDENCY_AST)
        {
            dependency_node = root->children[i];
            break;
        }
    }
    
    if(!dependency_node->children[0])
        return NULL;

    for(int i = 0; i < dependency_node->children[0]->num_children; i++)
    {
        dependency_node->children[0]->children[i]

        
    }
}

void dependency_append(Component_t* node, Component_t* child)
{
    node->num_dependencies++;

    node->dependencies = (realloc(node->num_dependencies, node->num_dependencies * sizeof(Component_t*)));
    node->dependencies[node->num_dependencies - 1] = child;
}
*/