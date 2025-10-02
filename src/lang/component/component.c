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

    component->cyclic_status = UNVISITED;
    component->graph_status = UNGRAPHED;

    return component;
}

// Returns true if a component graph is cyclic
bool check_cycles(Component_t* node)
{
    node->cyclic_status = ONGOING;
    for(int i = 0; i < node->num_dependencies; i++)
    {
        bool cyclic = false;

        if(node->dependencies[i]->cyclic_status == ONGOING)
            return true;

        else if(check_cycles(node->dependencies[i]))
            return true;
    }

    node->cyclic_status = COMPLETED;
    return false;
}

HashTable_t* init_component_registry(ASTNode_t* root)
{
    HashTable_t* table = init_hash_table(128);

    if(!table)
        return NULL;

    size_t num_components = 0;
    for(int i = 0; i < root->num_children; i++)
    {
        if(root->children[i]->type != DEFINITION_AST)
            continue;
            
        Component_t* child = init_component(root->children[i]);
        
        if(!child)
            return NULL;

        insert_hash(table, child, child->identifier);
    }

    return table;
}

ComponentGraph_t init_component_graph(HashTable_t* registry)
{
    ComponentGraph_t* graph = malloc(sizeof(ComponentGraph_t));

    

}

void dependency_append(Component_t* node, Component_t* child)
{
    node->num_dependencies++;

    node->dependencies = (realloc(node->num_dependencies, node->num_dependencies * sizeof(Component_t*)));
    node->dependencies[node->num_dependencies - 1] = child;
}
