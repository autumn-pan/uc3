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

const char *KEYWORDS[] = {
    "DEFINE", // Define a module
    "CONFIG", // Configure a module
    "ENABLED", // Whether or not a module is enabled
    "FIELD", // A custom field of any type created in module definition, allows for customization
    "DEPENDENCIES", // Defines what modules must exist for a certain module to work
    "NOTE", // A short string associated with modules for simple descriptions or notes
    "DEFAULT" // Used in definitions to set the default value of a field
};

typedef struct {
    enum TOKEN_TYPE type;
    char *value;
    int line;
    int column;
} Token;

typedef struct {
    const char *src;  // Pointer to source code
    int length;          // Length of source code
    int pos;        // Current index in source
    int line;            // Line number for error reporting
    int column;          // Column number for debugging
} Lexer;

