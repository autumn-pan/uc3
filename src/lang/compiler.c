#include "lang/compiler.h"

#include <stdio.h>

#include "lang/ast.h"
#include "lang/component/component.h"
#include "lang/parser/parser.h"
#include "lang/reader.h"
#include "lang/symbol/symbol.h"
#include "lang/util/hash.h"

void gen_config(HashTable_t *component_registry, SymbolNode_t *global_symbols);
void config(HashTable_t *components);

// Writes the entire tokenstream to stdout for debugging
void dump_tokenstream(TokenStream *tokenstream) {
  Token *token = tokenstream->head;
  while (token) {
    printf("\nToken: %s", token->value);
    token = token->next;
  }
}

// Compiles a uc3 script
void compile(char *file_name) {
  // Tokenize the source code
  char *src = read(file_name);
  Lexer *lexer = init_lexer(src);
  TokenStream *tokenstream = tokenize(lexer);

  // Parse and register an abstract syntax tree
  ASTNode_t *root = parse(init_parser(tokenstream, lexer));
  HashTable_t *table = init_component_registry(root);

  SymbolNode_t *symbol_table = symbolize_ast(root);

  append_component_fields(table);
  symbolize_fields(table, symbol_table);

  // Prompt the user for configuration options
  config(table);

  // Post-Config generation
  append_component_dependencies(table);

  if (verify_components(table) == 1) {
    fprintf(stderr, "Error: Circular dependency detected!");
    exit(EXIT_FAILURE);
  }

  gen_config(table, symbol_table);
}

// Autogenerate the config.h file
void gen_config(HashTable_t *component_registry, SymbolNode_t *global_symbols) {
  // Create the file
  FILE *file = fopen("autoconfig.h", "w");
  if (!file) {
    fprintf(stderr, "Error: autoconfig.h failed to open!");
    exit(EXIT_FAILURE);
  }

  uint8_t num_macros = 0;
  // Measures which component the generator is on
  size_t current_component = -1;
  for (int i = 0; i < component_registry->hash_max; i++) {
    // Get the next component
    HashElement_t *element = component_registry->contents[i];

    if (!element)
      continue;

    Component_t *component = (Component_t *)element->value;
    if (!component)
      continue;

    current_component++;
    // Set up the component's macro registry
    parse_component_macros(component);

    // Write macro values to the config file
    for (int j = 0; j < component->num_macros; j++) {
      // Ensure that the macro exists
      Macro_t *macro = component->macros[j];
      if (!macro) {
        fprintf(stderr, "Error: Macro was improperly appended!");
        exit(EXIT_FAILURE);
      }

      // Evaluate the macro's value
      size_t index = get_hash_pos(component_registry, component->identifier);
      HashElement_t *local_scope = global_symbols->children->contents[index];

      if (!local_scope) {
        fprintf(stderr, "Error: local_scope is not defined");
        exit(EXIT_FAILURE);
      }

      macro->value =
          eval(macro->expr, global_symbols, (SymbolNode_t *)local_scope->value);
      // Print the macro to the file
      fprintf(file, "#define %s%s%i", macro->identifier, " ", macro->value);
      fprintf(file, "\n");
    }
  }
}

// Prompts the user for config options and sets FIELD values
void config(HashTable_t *components) {
  for (int i = 0; i < components->hash_max; i++) {
    if (components->contents[i] == NULL)
      continue;

    Component_t *component = (Component_t *)components->contents[i]->value;

    printf("\nConfiguring %s", component->identifier);
    printf("\nFields to Configure (enter only integers): %i\n",
           component->num_fields);

    // Prompt users for configs
    for (int j = 0; j < component->num_fields; j++) {
      if (component->fields[j] == NULL)
        continue;

      Field_t *field = component->fields[j];
      printf("\t%s: ", field->variable->identifier);

      // Prompt the user for a config value
      int val;
      scanf("%i", &val);
      char *out = malloc(16);
      sprintf(out, "%i", val);

      // Write config to the appropriate field expression
      if (!field->variable->expr->data.str) {
        fprintf(stderr, "Error: Field '%s' has null expression!",
                field->variable->identifier);
      }

      field->variable->expr->data.str = out;
    }
  }
}