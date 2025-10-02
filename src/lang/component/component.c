#include "lang/component/component.h"
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>


// Constructor for Component_t
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

bool verify_components(HashTable_t* table)
{
    if(!table)
        return false;

    bool is_cyclic = false;
    for(int i = 0; i < table->hash_max; i++)
    {
        if(!table->contents[i])
            continue;
        
        if(((Component_t*)(table->contents[i]->value))->cyclic_status == COMPLETED)
            continue;

        is_cyclic = check_cycles(table->contents[i]->value);

        if(is_cyclic)
            return true;
    }

    return false;
}

// Register the components of a project
HashTable_t* init_component_registry(ASTNode_t* root)
{
    HashTable_t* table = init_hash_table(8);

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

        bool duplicate_key = insert_hash(table, child, child->identifier);
        // Quit if there's a duplicate key (redefinition error)
        if(duplicate_key)
            return NULL;
    }

    return table;
}

void dependency_append(Component_t* node, Component_t* child)
{
    node->num_dependencies++;

    node->dependencies = (realloc(node->dependencies, node->num_dependencies * sizeof(Component_t*)));
    node->dependencies[node->num_dependencies - 1] = child;
}

// Attaches the dependencies to each component
bool append_component_dependencies(HashTable_t* registry)
{
    if(!registry)
        return NULL;

    // Cycle through each component in the registry
    for(int i = 0; i < registry->hash_max; i++)
    {
        if(!registry->contents[i])
            continue;
        
        Component_t* component = registry->contents[i]->value;
        if(!component)
            return false;
        
        // Find the dependency node
        ASTNode_t* dependency_node;
        printf("flag");
        fflush(stdout);
        for(int j = 0; j < component->node->children[0]->num_children; j++)
        {
            if(component->node->children[0]->children[j]->type == DEPENDENCY_AST)
            {                

                dependency_node = component->node->children[0]->children[j];
                break;
            }
        }

        if(!dependency_node || !dependency_node->children[0])
        {
            component->graph_status = GRAPHED;
            continue;
        }

        // Copy dependencies from AST Node to Component Node
        for(int j = 0; j < dependency_node->children[0]->num_children; j++)
        {
            size_t index = get_hash_pos(registry, dependency_node->children[0]->children[j]->data.str);

            if(index == ULONG_MAX)
            {
                // Reference to undefined dependency
                return false;
            }

            dependency_append(component, (Component_t*)(registry->contents[index]->value));
        }
    }

    return true;
}
