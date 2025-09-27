#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>

enum TOKEN_TYPE
{
    KEYWORD,
    IDENTIFIER,
    TYPE_SPECIFIER,
    STR_LITERAL,
    FLOAT_LITERAL,
    INT_LITERAL,
    CHAR_LITERAL,
    BOOL_LITERAL,
    LBRACE,
    RBRACE
};

extern const char *KEYWORDS[];


typedef struct token {
    enum TOKEN_TYPE type;
    struct token* next;
    char value[64];
    int line;
    int column;
} Token;

typedef struct {
    Token* head;
    Token* tail;
    uint32_t size;
} TokenStream;

typedef struct {
    const char *src;  // Pointer to source code
    int length;          // Length of source code
    int pos;        // Current index in source
    int line;            // Line number for error reporting
    int column;          // Column number for debugging
} Lexer;

char peek(Lexer* lexer);
char advance(Lexer* lexer);
Lexer* init_lexer(const char* str);
Token* init_token(enum TOKEN_TYPE type, char *value, uint32_t line, uint32_t column);
Token* next_token(Lexer *lexer);
TokenStream* tokenize(Lexer* lexer);
TokenStream* init_tokenstream();
void append_token(TokenStream* ts, Token* token);
void tokenstream_pop_head(TokenStream* ts);

char * to_string(const char character);
#endif