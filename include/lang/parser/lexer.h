#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>

enum TOKEN_TYPE
{
    KEYWORD_TOKEN,
    IDENTIFIER_TOKEN,
    TYPE_SPECIFIER_TOKEN,
    STR_TOKEN,
    FLOAT_TOKEN,
    INT_TOKEN,
    CHAR_TOKEN,
    BOOL_TOKEN,
    DEPENDENCY_TOKEN_TOKEN,
    LBRACE_TOKEN,
    RBRACE_TOKEN,
    EOF_TOKEN,
    EOL_TOKEN,
    LSQBRACE_TOKEN,
    RSQBRACE_TOKEN,
    NULL_TOKEN,
    DATA_TYPE_TOKEN,

    OPERATOR_TOKEN
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

char advance(Lexer* lexer);
Lexer* init_lexer(const char* str);
Token* init_token(enum TOKEN_TYPE type, char *value, uint32_t line, uint32_t column);
TokenStream* tokenize(Lexer* lexer);
TokenStream* init_tokenstream();
void append_token(TokenStream* ts, Token* token);

void free_tokenstream(TokenStream* ts);
char * to_string(const char character);

#endif