#include "lang/parser/parser.h"
#include "lang/parser/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ASTNode_t* parse_expression(Parser_t* parser);

//////////////////////////////////////////////////////////////
// Utils
//////////////////////////////////////////////////////////////

void syntax_error(char* expected, char* found)
{
    fprintf(stderr, "Syntax Error: expected %s%s%s", expected, ", found ", found);
    exit(EXIT_FAILURE);
}

void point_error(size_t line, size_t column)\
{
    fprintf(stderr, "\nLine: %i%s%i", line, ", Column: ", column);
    exit(EXIT_FAILURE);
}

// Maps a token data type to its corresponding AST data type
// Returns NULL if the token is not a data type
int token_to_ast_type(enum TOKEN_TYPE type)
{
    switch(type)
    {
        case(INT_TOKEN):
            return INT_AST;
        case(BOOL_TOKEN):
            return BOOL_AST;
        case(IDENTIFIER_TOKEN):
            return IDEN_AST;
        default:
            return NULL_AST;
    }
}

//////////////////////////////////////////////////////////////
// Parser
//////////////////////////////////////////////////////////////

Parser_t* init_parser(TokenStream* ts, Lexer* lexer)
{
    Parser_t *parser = (malloc(sizeof(Parser_t)));
    if(!parser)
    {
        fprintf(stderr, "Error: Failed to allocate memory!");
        exit(EXIT_FAILURE);
    }
    
    parser->ts = ts;

    parser->pos = 0;
    parser->len = ts->size;
    parser->ptr = ts->head;

    // Free the lexer, as it will no longer be used
    free(lexer);
    return parser;
}

Token *get_next_token(Parser_t *p) {
    if(!p->ptr)
        return NULL;

    if (p->pos < p->len - 1) {
        return p->ptr->next;
    }
    return NULL;
}

bool advance_parser(Parser_t* parser)
{
    if (parser->pos < parser->len && parser->ptr) 
    {
        parser->pos++;
        parser->ptr = parser->ptr->next;

        parser->line = parser->ptr->line;
        parser->column = parser->ptr->column;

        if(!parser->ptr)
            return false;

        return true;
    }
    return false;
}

char* match(Parser_t *p, enum TOKEN_TYPE type) {
    if (p->pos < p->len && p->ptr->type == type) {
        char* tmp = p->ptr->value;

        advance_parser(p);
        return tmp;
    }
    return NULL;
}


// Advance parser and return true if the current token matches the key
bool match_value(Parser_t *p, char *value) {
    if (p->pos < p->len && strcmp(p->ptr->value, value) == 0 && p->ptr) {

        advance_parser(p);
        return true;
    }
    return false;
}

ASTNode_t* parse_field(Parser_t* parser)
{
    if(!match_value(parser, "FIELD"))
        return NULL;

    char* node_identifier;
    if(!(node_identifier = match(parser, IDENTIFIER_TOKEN)))
        return NULL;

    if(!match_value(parser, "DEFAULT"))
    {
        fprintf(stderr, "Error: Expected a DEFAULT value to be set");
        point_error(parser->line, parser->column);
    }

    char* val;
    if(!(val = match(parser, INT_TOKEN)))
        return NULL;

    ASTNode_t* node = init_ast(FIELD_AST, node_identifier);

    if(!node)
        return NULL;

    ast_append(node, init_ast(INT_AST, val));
    return node;
}

ASTNode_t* parse_subsystem(Parser_t* parser)
{
    if(!match_value(parser, "SUBSYSTEM"))
        return NULL;

    char* node_identifier;
    if(!(node_identifier = match(parser, IDENTIFIER_TOKEN)))
        return NULL;

    ASTNode_t* node = init_ast(SUBSYSTEM_AST, node_identifier);


    return node;
}

ASTNode_t* parse_variable_decl(Parser_t* parser)
{
    if(!parser->ptr)
        return NULL;
        
    char* str = parser->ptr->value;
    if(!(match_value(parser, "INT") || match_value(parser, "BOOL")))
        return NULL;

    enum TOKEN_TYPE declared_type = str_to_datatype(str);

    if(declared_type == NULL_TOKEN)
    {
        fprintf(stderr, "Error: Data type not recognized");
        point_error(parser->line, parser->column);
    }

    char* identifier;
    if(!(identifier = match(parser, IDENTIFIER_TOKEN)))
    {
        fprintf(stderr, "Error: Expected identifier after variable declaration!");
        point_error(parser->line, parser->column);
    }

    ASTNode_t* node = init_ast(VARIABLE_DECL_AST, identifier);

    // Set variable definition if applicable
    if(!match_value(parser, "="))
        return node;

    ASTNode_t* child = parse_expression(parser);

    if(!child) 
    {
        return NULL;
    }

    ast_append(node, child);
    return node;
}

// Check if a token can be placed in a list. 
// Only identifiers and literals are allowed in lists.
bool is_list_compatible(Token* token)
{
    if(!token)
        return false;

    if(token->type == IDENTIFIER_TOKEN || token->type == INT_TOKEN || token->type == STR_TOKEN || token->type == BOOL_TOKEN)
        return true;

    return false;
}

// Parse a list into an AST subtree
ASTNode_t* parse_list(Parser_t* parser)
{
    if(!match(parser, LSQBRACE_TOKEN))
        return NULL;
    
    // Init root
    ASTNode_t* node = init_ast(LIST_AST, "LIST");

    // append everything within the list as children of the list node
    while(is_list_compatible(parser->ptr) && parser->ptr->type != EOF && parser->ptr)
    {
        Token* token = parser->ptr;

        // If the end of the tokenstream is hit early, quit
        if(!parser->ptr)
            return NULL;

        AST_TYPE type = PLACEHOLDER_AST;

        // Convert token type to AST type
        // TODO: consolidate types or create simpler conversions
        switch(token->type)
        {
            case(INT_TOKEN):
                type = INT_AST;
                break;
            case(IDENTIFIER_TOKEN):
                type = IDEN_AST;
                break;
        }

        // Quit if the type is unrecognized for whatever reason
        if(type == NULL_TOKEN)
        {
            fprintf(stderr, "\nError: List contents must be identifiers or literals!");
            point_error(parser->line, parser->column);
        }

        // Append the child to the list root
        ASTNode_t* child = init_ast(type, token->value);
        if(!child)
            return NULL;

        ast_append(node, child);

        advance_parser(parser);
    }

    // Ensure that the lit is closed appropriately
    if(match(parser, RSQBRACE_TOKEN) == NULL)
    {
        fprintf(stderr, "\nError: List contents must be identifiers or literals!");
        point_error(parser->line, parser->column);
    }

    return node;
}

// Parse a dependency list
ASTNode_t* parse_dependency(Parser_t* parser)
{
    if(!match_value(parser, "DEPENDENCIES"))
        return NULL;

    ASTNode_t* node = init_ast(DEPENDENCY_AST, "DEPENDENCY");

    ASTNode_t* list = parse_list(parser);

    if(!list || !node)
        return NULL;

    ast_append(node, list);

    return node;
}

ASTNode_t* parse_macro(Parser_t* parser)
{
    if(!match_value(parser, "MACRO"))
        return NULL;

    char* macro_name = parser->ptr->value;
    if(!match(parser, IDENTIFIER_TOKEN))
    {
        fprintf(stderr, "Error: Expected identifier");
        point_error(parser->line, parser->column);
    }

    ASTNode_t* value;
    if(!(value = parse_expression(parser)))
    {
        fprintf(stderr, "Error: Expression failed to parse");
        point_error(parser->line, parser->column);
    }

    ASTNode_t* node = init_ast(MACRO_AST, macro_name);
    ast_append(node, value);

    return node;
}

ASTNode_t* parse_statement(Parser_t* parser)
{   
    ASTNode_t* node = NULL;
    if((node = parse_field(parser)) != NULL)
        return node;
    else if((node = parse_subsystem(parser)) != NULL)
        return node;
    else if((node = parse_dependency(parser)) != NULL)
        return node;
    else if((node = parse_variable_decl(parser)) != NULL)
        return node;
    else if((node = parse_macro(parser)) != NULL)
        return node;

    
    fprintf(stderr, "\nError: Unrecognized token");
    point_error(parser->line, parser->column);
}

// TODO: Sopport conditional blocks
ASTNode_t* parse_block(Parser_t* parser)
{
    AST_TYPE type;
    ASTNode_t* node;
    if(match_value(parser, "DEFINE"))
    {
        type = DEFINITION_AST;
    }
    else if(match_value(parser, "CONFIG"))
    {
        type = CONFIGURATION_AST;
    }
    else
    {
        return NULL;
    }

    char* block_identifier;
    if(!(block_identifier = match(parser, IDENTIFIER_TOKEN)))
        return NULL;
    
    if(!match(parser, LBRACE_TOKEN))
        return NULL;

    node = init_ast(type, block_identifier);

    if(!node)
        return NULL;

    // Parse each statement within a definition
    ASTNode_t* child;

    ASTNode_t* block = init_ast(BLOCK_AST, "BLOCK");
    if(!block)
        return NULL;

    
    while(parser->ptr->type != RBRACE_TOKEN && parser->ptr->type != EOF)
    {
        child = parse_statement(parser);

        if(!child)
        {
            return NULL;
        }

        ast_append(block, child);
    }

    if(parser->ptr->type == RBRACE_TOKEN)
    {
        parser->pos++;
        parser->ptr = parser->ptr->next;
    }
    else
    {
        fprintf(stderr, "Error: expected ']'");
        point_error(parser->line, parser->column);
    }

    ast_append(node, block);
    return node;
}

// Returns a project tree
ASTNode_t* parse(Parser_t* parser)
{
    ASTNode_t* root = init_ast(ROOT_AST, "ROOT");

    while(1)
    {
        ASTNode_t* block;

        if (block = parse_variable_decl(parser))
        {
            ast_append(root, block);
            continue;
        }

        block = parse_block(parser);

        if(!block)
            break;

        ast_append(root, block);
    }

    free(parser);
    return root;
}

///////////////////////////////////////////////////////////
// Expressions -- Recursive Descent
///////////////////////////////////////////////////////////

// Parse a literal value (int, bool, char, string)
ASTNode_t* parse_literal(Parser_t* parser)
{
    enum TOKEN_TYPE type = parser->ptr->type;
    char* value = parser->ptr->value;

    if(!(type == INT_TOKEN || type == BOOL_TOKEN || type == STR_TOKEN || type == CHAR_TOKEN))
        return NULL;

    ASTNode_t* node = init_ast(token_to_ast_type(type), parser->ptr->value);;
    advance_parser(parser);

    return node;
}

// Parse a variable call (identifier)
ASTNode_t* parse_variable_call(Parser_t* parser)
{
    if(parser->ptr->type != IDENTIFIER_TOKEN)
        return NULL;

    ASTNode_t* node = init_ast(IDEN_AST, parser->ptr->value);
    advance_parser(parser);
    return node;
}

// Parse a factor, which consists of either a literal, variable call, or parenthesized expression
ASTNode_t * parse_factor(Parser_t* parser) {
    if (match(parser, LPAR_TOKEN)) {
        ASTNode_t * expr = parse_expression(parser);

        if (!match(parser, RPAR_TOKEN)) {
            fprintf(stderr, "Error: Right Parenthesis expected!");
            point_error(parser->line, parser->column);
        }
        return expr;
    }

    ASTNode_t * node = NULL;
    if ((node = parse_literal(parser)) != NULL) return node;
    if ((node = parse_variable_call(parser)) != NULL) return node;

    fprintf(stderr, "Error: expected a factor");
    point_error(parser->line, parser->column);
}

// Parse a term, which represents a multiplication or division operation
ASTNode_t* parse_term(Parser_t* parser)
{
    ASTNode_t* node = parse_factor(parser);
    if(node == NULL)
    {
        fprintf(stderr, "Error: Left side of term is missing!");
        point_error(parser->line, parser->column);
    }

    while(1)
    {
        Token* token = parser->ptr;
        if (token->type == OPERATOR_TOKEN && (token->value[0] == '*' || token->value[0] == '/'))
        {
            advance_parser(parser);
            ASTNode_t* right = parse_factor(parser);

            // Obtain binary operation type
            AST_TYPE type = NULL_AST;
            if(token->value[0] == '*')
                type = MULT_AST;
            else if(token->value[0] == '/')
                type = DIV_AST;
            else
            {
                fprintf(stderr, "Error: Unrecognized binary operator");
                point_error(parser->line, parser->column);
            }

            ASTNode_t* operator = init_ast(type, "OPERATOR");
            ast_append(operator, node);
            ast_append(operator, right);

            node = operator;
        }
        else
            break;
    }
    return node;
}

// Recursively parse a full expression
ASTNode_t* parse_expression(Parser_t* parser) 
{
    ASTNode_t* node = parse_term(parser);
    if(node == NULL)
    {
        fprintf(stderr, "Error: Left side of term is missing!");
        point_error(parser->line, parser->column);
    }

    while(1)
    {
        Token* token = parser->ptr;
        if (token->type == OPERATOR_TOKEN && (token->value[0] == '+' || token->value[0] == '-'))
        {

            advance_parser(parser);

            ASTNode_t* right = parse_term(parser);

            // Obtain binary operation type
            AST_TYPE type = NULL_AST;
            if(token->value[0] == '+')
                type = PLUS_AST;
            else if(token->value[0] == '-')
                type = MINUS_AST;
            else
            {
                fprintf(stderr, "Error: Unrecognized binary operator");
                point_error(parser->line, parser->column);
            }

            ASTNode_t* operator = init_ast(type, "OPERATOR");
            ast_append(operator, node);
            ast_append(operator, right);
            node = operator;
        }
        else
        {
            break;
        }
    }

    return node;
}