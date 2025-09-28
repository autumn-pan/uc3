#include "lang/parser/lexer.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

const char *KEYWORDS[] = {
    "DEFINE", // Define a module
    "CONFIG", // Configure a module
    "ENABLED", // Whether or not a module is enabled
    "FIELD", // A custom field of any type created in module definition, allows for customization
    "DEPENDENCIES", // Defines what modules must exist for a certain module to work
    "NOTE", // A short string associated with modules for simple descriptions or notes
    "DEFAULT" // Used in definitions to set the default value of a field
};

Lexer* init_lexer(const char *src) {
    Lexer *lexer = (Lexer *)malloc(sizeof(Lexer)); 
    lexer->src = src;
    lexer->length = strlen(src);

    //Set the lexer data
    lexer->pos = 0;
    lexer->line = 1;
    lexer->column = 1;

    //Return the finished lexer
    return lexer;
};

//////////////////////////////////////////////////////////////
/// String Utils
//////////////////////////////////////////////////////////////
void cat_char(char* str, char c)
{
    char temp[2] = {c, '\0'};
    strcat(str, temp);
}

bool in_array(const char *arr[], const char *key, uint8_t size)
{
    // For every string of the array
    for(int i = 0; i < size; i++) 
    {
        if(strcmp(arr[i], key) == 0)
            return true; //If they are equal, return true
    }
    // If no string matches, return false
    return false;
}

bool is_whitespace(Lexer *lexer)
{
    char c = lexer->src[lexer->pos];
    // Check if it is a space, tab, or newline
    if(c == ' ' || c == '\t' || c == '\n')
        return true;
    // If not, return false
    return false;
}

void skip_whitespace(Lexer* lexer)
{
    while(is_whitespace(lexer))
    {
        advance(lexer);
    }
}

//////////////////////////////////////////////////////////////
/// Tokenization
//////////////////////////////////////////////////////////////

Token* init_token(enum TOKEN_TYPE type, char* str, uint32_t line, uint32_t col)
{
    Token* token = (Token*)(malloc(sizeof(Token)));
    strcpy(token->value, str);
    token->next = NULL;
    token->line = line;
    token->column = col;
    token->type = type;

    return token;
}

char peek(Lexer* lexer)
{
    if(!lexer || lexer->pos+1 >= lexer->length)
        return '\0';

    char c = lexer->src[lexer->pos+1];

    return c;
}

char advance(Lexer* lexer)
{
    char c = lexer->src[lexer->pos];
    lexer->pos++;
    return c;
}

Token* next_token(Lexer* lexer)
{    
    skip_whitespace(lexer);


    // The value of the next token, as a string
    char str[32] = "";

    // This will suggest the token type of the next token
    char current_char = lexer->src[lexer->pos];

    // Check if the next token is a string
    if(isalpha(lexer->src[lexer->pos]) || lexer->src[lexer->pos] == '_')
    {
        while(isalnum(lexer->src[lexer->pos]) || lexer->src[lexer->pos] == '_')
        {
            cat_char(str, (advance(lexer))); 
        }
        printf("\nstring: %s", str);
        fflush(stdout);
        if(in_array(KEYWORDS, str, 7))
            return init_token(KEYWORD, str, lexer->line, lexer->column);
        return init_token(IDENTIFIER, str, lexer->line, lexer->column);
    }

    // Check if the string is a number
    if(isdigit(lexer->src[lexer->pos])) //Checks for a numeric literal
    {
        while(isdigit(lexer->src[lexer->pos])) //Initiates the first component of the literal
        {
            cat_char(str, (advance(lexer)));
        }

        printf("\n success");
        fflush(stdout);
        return init_token(INT_LITERAL, str, lexer->line, lexer->column); // Return int literal
    }

    // Check if the next token is a miscellaneous token
    enum TOKEN_TYPE misc_token_type = UNDEFINED;

    char temp[2] = {current_char, '\0'};
    switch(current_char)
    {
        case '{':
            misc_token_type = LBRACE;
            advance(lexer);
            break;
        case '}':
            misc_token_type = RBRACE;
            advance(lexer);
            break;
        case '[':
            misc_token_type = LSQBRACE;
            advance(lexer);
            break;
        case ']':
            misc_token_type = RSQBRACE;
            advance(lexer);
            break;
    }

    if(misc_token_type != UNDEFINED)
    {
        return init_token(misc_token_type, temp, lexer->line, lexer->column);
    }
    
    return NULL;
}
TokenStream* tokenize(Lexer* lexer)
{
    TokenStream* token_stream = init_tokenstream();

    Token* token = next_token(lexer);
    
    while(token)
    {
        append_token(token_stream, token);
        token = next_token(lexer);
    }

    return token_stream;
}

//////////////////////////////////////////////////////////////
/// Token Stream and Utils
//////////////////////////////////////////////////////////////

TokenStream* init_tokenstream()
{
    TokenStream* token_stream = (TokenStream*)(malloc(sizeof(TokenStream)));
    token_stream->head = NULL;
    token_stream->tail = NULL;
    token_stream->size = 0;

    return token_stream;
}

void append_token(TokenStream* ts, Token* token)
{
    token->next = NULL;

    if (ts->head == NULL) { // Empty list
        ts->head = token;
        ts->tail = token;
    } else {
        ts->tail->next = token;
        ts->tail = token;
    }
    // Track list size
    ts->size++;
}

void tokenstream_pop_head(TokenStream* ts)
{
    if(!ts->head) 
        return;

    if(ts->head->next)
    {
        ts->head = ts->head->next;
    }
    else
    {
        ts->head = NULL;
        ts->tail = NULL;
    }

    ts->size--;
}
