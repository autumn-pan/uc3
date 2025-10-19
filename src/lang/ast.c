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

  ast->children = NULL;

  ast->num_children = 0;
  ast->data.str = value;
  ast->type = type;

  return ast;
}

void free_ast(ASTNode_t *ast)
{
  if(!ast)
    return;

  /* The AST doesn't own the string, but it will in the future
  if(ast->data.str)
    free(ast->data.str);
  */

  free(ast);
}

// Append an ASTNode as a child to a parent node
bool ast_append(ASTNode_t *node, ASTNode_t *child) {
  if (!node || !child) {
    fprintf(stderr, "Error: null node or child passed to ast_append!\n");
    return false;
  }

  ASTNode_t** tmp =
      (realloc(node->children, (node->num_children + 1) * sizeof(ASTNode_t *)));
  if (!tmp) {
    fprintf(stderr, "Error: Failed to reallocate memory!\n");
    return false;
  }

  node->children = tmp;
  node->children[node->num_children] = child;
  node->num_children++;

  return true;
}