#include "lang/ast.h"

#include <stdio.h>
#include <stdlib.h>

#include "lang/ast.h"

ASTNode_t *init_ast(AST_TYPE type, char *value) {
  ASTNode_t *ast = (malloc(sizeof(ASTNode_t)));

  if (!ast) {
    fprintf(stderr, "Fatal: Failed to allocate enough memory!");
    exit(EXIT_FAILURE);
  }

  ast->children = (calloc(0, sizeof(ASTNode_t *)));
  if (!ast->children) {
    fprintf(stderr, "Fatal: Failed to allocate enough memory!");
    exit(EXIT_FAILURE);
  }

  ast->num_children = 0;
  ast->data.str = value;
  ast->type = type;
}

// Append an ASTNode as a child to a parent node
void ast_append(ASTNode_t *node, ASTNode_t *child) {
  if(!node || !child)
  {
    fprintf(stderr, "\nError: null node or child passed to ast_append!");
    return false;
  }
  node->num_children++;

  node->children =
      (realloc(node->children, node->num_children * sizeof(ASTNode_t *)));
  if (!node->children) {
    fprintf(stderr, "Error: Failed to reallocate memory!");
    return false;
  
  node->children[node->num_children - 1] = child;
  return true;
}