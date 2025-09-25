#include "lexer.h"
#include <stdbool.h>
#include <string.h>

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

bool in_array(const char **arr, const char *key)
{
    // For every string of the array
    for(int i = 0; arr[i] != '\0'; i++) 
    {
        // Compare each value with the key
        if(strcmp((const char *)arr[i], key) == 0)
            return true; //If they are equal, return true
    }
    // If no string matches, return false
    return false;
}

void skip_whitespace(Lexer* lexer)
{
    while(lexer->src[lexer->pos] == " " || lexer->src[lexer->pos] == "\t")
    {
        lexer->pos++;
    }
}

//////////////////////////////////////////////////////////////
/// Tokenization
//////////////////////////////////////////////////////////////

Token* next_token(Lexer* lexer)
{
    skip_whitespace(lexer);

    // Check if the next token is a string
    char* str;
    if(is_alphabetical(peek(lexer)))
    {
        strcat(str, advance(lexer));
        while(is_alphanumerical(peek(lexer)))
        {
            strcat(str, advance(lexer)); 
        }

        if(in_array(KEYWORDS, str))
        {
            return init_token(KEYWORD, str, lexer->line, lexer->column);
        }
    }
}

TokenStream* tokenize(Lexer* lexer)
{
    TokenStream* token_stream = (TokenStream*)(malloc(sizeof(TokenStream)));

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
