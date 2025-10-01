#include "lang/parser/parser.h"
#include "lang/parser/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Parser_t* init_parser(TokenStream* ts, Lexer* lexer)
{
    Parser_t *parser = (malloc(sizeof(Parser_t)));
    if(!parser)
        return NULL;
    
    parser->ts = ts;

    parser->pos = 0;
    parser->len = ts->size;
    parser->ptr = ts->head;

    // Free the lexer, as it will no longer be used
    free(lexer);
    return parser;
}

Token *get_next_token(Parser_t *p) {
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

// Match value function
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
        return NULL;

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
    if(!(match_value(parser, "INT") || match_value(parser, "BOOL")))
        return NULL;

    char* identifier;
    if(!(identifier = match(parser, IDENTIFIER_TOKEN)))
        return NULL;

    ASTNode_t* node = init_ast(VARIABLE_DECL_AST, identifier);
    // TODO: add definitions (requires operator parsing)
    return node;
}

// Check if a token can be placed in a list. 
// Only identifiers and literals are allowed in lists.
bool is_list_compatible(Token* token)
{
    if(!token)
        return false;

    if(token->type == IDENTIFIER_TOKEN || token->type == INT_TOKEN || token->type == STR_TOKEN)
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
            case(STR_TOKEN):
                type = STRING_AST;
                break;
            case(INT_TOKEN):
                type = INT_AST;
                break;
            case(IDENTIFIER_TOKEN):
                type = IDEN_AST;
                break;
        }

        // Quit if the type is unrecognized for whatever reason
        if(type == NULL_TOKEN)
            return NULL;

        // Append the child to the list root
        ASTNode_t* child = init_ast(type, token->value);
        if(!child)
            return NULL;

        ast_append(node, child);

        advance_parser(parser);
    }

    // Ensure that the lit is closed appropriately
    if(match(parser, RSQBRACE_TOKEN) == NULL)
        return NULL;

    return node;
}

// Parse a dependency list
ASTNode_t* parse_dependency(Parser_t* parser)
{
    if(!match_value(parser, "DEPENDENCIES"))
        return NULL;

    ASTNode_t* node = init_ast(DEPENDENCY_AST, "");

    ASTNode_t* list = parse_list(parser);

    if(!list || !node)
        return NULL;

    ast_append(node, list);

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

    return NULL;
}

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
    return root;
}