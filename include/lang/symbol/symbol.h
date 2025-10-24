#ifndef SYMBOL_H
#define SYMBOL_H

#include "lang/ast.h"
#include "lang/util/hash.h"

#include <stdbool.h>
#include <stdlib.h>

#define SYMBOL_CHILDREN_SIZE 8

typedef enum { INT_T, BOOL_T, UNKNOWN_T } TYPE;
typedef enum { FALSE = 0, NEUTRAL = -1, TRUE = 1 } TRISTATE;

typedef struct {
  int value;
  TYPE type;
} Value_t;

typedef struct {
  const char *identifier;
  bool constant;
  uint32_t hash;
  Value_t value;
  ASTNode_t *expr;
} Symbol_t;

typedef struct {
  size_t num_children;
  // Symbol Nodes come together to form a symbol tree.
  // children represents a node's subscopes
  HashTable_t *children;
  // symbols represents the values that are in the scope
  HashTable_t *symbols;
} SymbolNode_t;

SymbolNode_t *symbolize_ast(ASTNode_t *node);
Symbol_t *init_symbol(ASTNode_t *expr, const char *identifier, bool constant);
Value_t init_value(TYPE type, int data);
Value_t string_to_value(TYPE type, const char *str);
bool str_to_bool(const char *str);
int eval(ASTNode_t *node, SymbolNode_t *table, SymbolNode_t *scope);
void symbolize_fields(HashTable_t *registry, SymbolNode_t *root);
#endif
