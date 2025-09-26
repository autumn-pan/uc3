#include "lang/parser/lexer/lexer.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
char * to_string(const char character)
{
    static char str[2];
    str[0] = character;
    str[1] = '\0';
    return str;
}

bool is_alphabetical(char c)
{
    if ((c >= 65 && c <=90) || (c >= 97 && c <=122)) // Checks for lowercase, then uppercase
        return true;
    // If it is neither, return false
    return false;
}

bool is_alphanumerical(char c)
{
    if((c >= 65 && c <=90) || (c >= 48 && c <= 57) || (c >= 97 && c <= 122)) // Checks if it is a capital letter, digit, or lowercase letter
        return true; 
    return false;
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

void skip_whitespace(Lexer* lexer)
{
    while(lexer->src[lexer->pos] == ' ' || lexer->src[lexer->pos] == '\t')
    {
        advance(lexer);
    }
}

//////////////////////////////////////////////////////////////
/// Tokenization
//////////////////////////////////////////////////////////////

Token* init_token(enum TOKEN_TYPE, char* str, uint32_t line, uint32_t col)
{
    Token* token = (Token*)(malloc(sizeof(Token)));
    token->value = str;
    token->next = NULL;
    token->line = line;
    token->column = col;
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
    if(!lexer || lexer->pos+1 >= lexer->length)
        return '\0';

    char c = lexer->src[++lexer->pos];

    return c;
}

Token* next_token(Lexer* lexer)
{    
    skip_whitespace(lexer);

    // The value of the next token, as a string
    char* str;

    // This will suggest the token type of the next token
    char next_char = peek(lexer);

    if(!next_char)
        return NULL;

    // Check if the next token is a string
    if(is_alphabetical(next_char))
    {
        strcat(str, to_string(advance(lexer))); 

        while(is_alphanumerical(peek(lexer)))
        {
            printf("\n char");
            fflush(stdout);
            strcat(str, to_string(advance(lexer))); 
        }

        if(in_array(KEYWORDS, str, 7))
        {
            return init_token(KEYWORD, str, lexer->line, lexer->column);
        }
        else
        {
            return init_token(IDENTIFIER, str, lexer->line, lexer->column);
        }
    }

    // Check if the string is a number
    if(next_char < 10)
    {
        while(peek(lexer) < 10)
        {
            printf("\n%c", peek(lexer));
            strcat(str, to_string(advance(lexer)));
        }
        return init_token(INT_LITERAL, str, lexer->line, lexer->column);
    }

    return NULL;
}

TokenStream* tokenize(Lexer* lexer)
{
    TokenStream* token_stream = (TokenStream*)(malloc(sizeof(TokenStream)));

    Token* token = next_token(lexer);
    
    while(token)
    {
        append_token(token_stream, token);
        printf("\n Token: %s", token->value);
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
