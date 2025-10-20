#include "lang/component/component.h"
#include "lang/symbol/symbol.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

Field_t *init_field(Symbol_t *symbol, int value) {
  Field_t *field = malloc(sizeof(Field_t));

  if (!field)
    return NULL;

  field->variable = symbol;
  field->default_value = value;

  return field;
}

Macro_t *init_macro(char *identifier, ASTNode_t *expr) {
  Macro_t *macro = malloc(sizeof(Macro_t));
  if (!macro) {
    fprintf(stderr, "Error: Memory allocation failed!");
    exit(EXIT_FAILURE);
  }

  macro->identifier = identifier;
  macro->expr = expr;

  return macro;
}

// Append a macro to a component's list
void macro_append(Component_t *node, Macro_t *child) {
  node->num_macros++;

  node->macros = (realloc(node->macros, node->num_macros * sizeof(Macro_t *)));
  node->macros[node->num_macros - 1] = child;
}

// Append a component node's macros to its corresponding component struct
void parse_component_macros(Component_t *component) {
  ASTNode_t *block = component->node->children[0];
  if (!block) {
    fprintf(stderr, "Error: Component block is missing!");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < block->num_children; i++) {
    if (block->children[i]->type != MACRO_AST)
      continue;

    Macro_t *macro = init_macro(block->children[i]->data.str,
                                block->children[i]->children[0]);

    macro_append(component, macro);
  }
}

Component_t *init_component(ASTNode_t *node) {
  if (!node)
    return NULL;

  Component_t *component = malloc(sizeof(Component_t));

  if (!component) {
    fprintf(stderr, "Error: Failed to allocate critical memory!");
    exit(EXIT_FAILURE);
  }

  component->enabled = true;
  component->identifier = node->data.str;
  component->node = node;
  component->num_dependencies = 0;
  component->dependencies = calloc(0, sizeof(Component_t *));

  component->cyclic_status = UNVISITED;
  component->graph_status = UNGRAPHED;

  component->fields = calloc(COMPONENT_MAX_FIELDS, sizeof(Field_t *));

  component->num_macros = 0;
  component->macros = calloc(COMPONENT_MAX_MACROS, sizeof(Macro_t *));
  return component;
}

// Returns true if a component graph is cyclic
bool check_cycles(Component_t *node) {
  node->cyclic_status = ONGOING;
  for (int i = 0; i < node->num_dependencies; i++) {
    if (node->dependencies[i]->cyclic_status == ONGOING)
      return true;
    else if (check_cycles(node->dependencies[i]) &&
             node->dependencies[i]->cyclic_status == UNVISITED)
      return true;
  }

  node->cyclic_status = COMPLETED;
  return false;
}

ASTNode_t *get_component_block(Component_t *component) {
  if (!component || !component->node || !component->node->children)
    return NULL;

  if (component->node->num_children < 1)
    return NULL;

  return component->node->children[0];
}

// Returns true if there is a circular dependency
bool verify_components(HashTable_t *table) {
  if (!table)
    return false;

  bool is_cyclic = false;
  for (int i = 0; i < table->hash_max; i++) {
    if (!table->contents[i])
      continue;

    if (((Component_t *)(table->contents[i]->value))->cyclic_status ==
        COMPLETED)
      continue;

    if (check_cycles(table->contents[i]->value))
      return true;
  }
  return false;
}

// Register the components of a project
HashTable_t *init_component_registry(ASTNode_t *root) {
  HashTable_t *table = init_hash_table(COMPONENT_REGISTRY_SIZE);

  if (!table)
    return NULL;

  size_t num_components = 0;
  for (int i = 0; i < root->num_children; i++) {
    if (root->children[i]->type != DEFINITION_AST)
      continue;

    Component_t *child = init_component(root->children[i]);
    if (!child)
      return NULL;

    if (insert_hash(table, child, child->identifier)) {
      fprintf(stderr, "Error: Component redeclaration detected!");
      exit(EXIT_FAILURE);
    }
  }

  return table;
}

void dependency_append(Component_t *node, Component_t *child) {
  node->num_dependencies++;

  node->dependencies = (realloc(node->dependencies, node->num_dependencies *
                                                        sizeof(Component_t *)));
  node->dependencies[node->num_dependencies - 1] = child;
}

// Attaches the dependencies to each component
bool append_component_dependencies(HashTable_t *registry) {
  if (!registry)
    return NULL;

  // Cycle through each component in the registry
  for (int i = 0; i < registry->hash_max; i++) {
    if (!registry->contents[i])
      continue;

    Component_t *component = registry->contents[i]->value;
    if (!component)
      return false;

    // Find the dependency node
    ASTNode_t *dependency_node;
    for (int j = 0; j < component->node->children[0]->num_children; j++) {
      if (component->node->children[0]->children[j]->type == DEPENDENCY_AST) {
        dependency_node = component->node->children[0]->children[j];
        break;
      }
    }

    if (dependency_node == NULL || dependency_node->num_children <= 0) {
      component->graph_status = GRAPHED;
      continue;
    }

    // Copy dependencies from AST Node to Component Node
    for (int j = 0; j < dependency_node->children[0]->num_children; j++) {
      size_t index = get_hash_pos(
          registry, dependency_node->children[0]->children[j]->data.str);

      if (index == ULONG_MAX) {
        // Reference to undefined dependency
        return false;
      }

      dependency_append(component,
                        (Component_t *)(registry->contents[index]->value));
    }
  }

  return true;
}

bool append_component_fields(HashTable_t *registry) {
  if (!registry) {
    fprintf(stderr, "Error: Null value passed to append_component_fields\n");
    return false;
  }

  if (registry->num_elements == 0)
    return true;

  for (int i = 0; i < registry->hash_max; i++) {
    if (registry->contents[i] == NULL)
      continue;

    Component_t *component = (Component_t *)registry->contents[i]->value;
    ASTNode_t *field_node;
    printf("\nIdentifier: %s", component->identifier);
    fflush(stdout);
    ASTNode_t *block = get_component_block(component);
    if (!block) {
      fprintf(stderr,
              "Error: invalid component passed to append_component_fields!\n");
      return false;
    }

    for (int j = 0; j < component->node->children[0]->num_children; j++) {
      if (component->node->children[0]->children[j]->type != FIELD_AST)
        continue;

      field_node = component->node->children[0]->children[j];

      // The first child of a FIELD node is its required default
      if (!field_node->children[0])
        return false;

      Symbol_t *symbol =
          init_symbol(field_node->children[0], field_node->data.str, false);
      if (!symbol)
        return false;

      Field_t *field =
          init_field(symbol, atoi(field_node->children[0]->data.str));
      if (!field)
        return false;

      component->fields[component->num_fields] = field;
      component->num_fields += 1;
    }
  }

  return true;
}