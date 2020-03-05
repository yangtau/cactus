#ifndef _CACTUS_SCANNER_SCANNER_H_
#define _CACTUS_SCANNER_SCANNER_H_

enum token_type {
    // separator:
    LEFT_PAREN = 1, // (
    RIGHT_PAREN,    // )
    LEFT_BRACE,     // {
    RIGHT_BRACE,    // }
    LEFT_BRACKET,   // [
    RIGHT_BRACKET,  // ]
    COMMA,          // ,
    NEW_LINE,       // \n
    // operator:
    DOT,           // .
    PLUS,          // +
    COLON,         // :
    SLASH,         // /
    STAR,          // *
    GREATER,       // >
    EQUAL,         // =
    MINUS,         // -
    LESS,          // <
    EQUAL_EQUAL,   // ==
    GREATER_EQUAL, // >=
    LESS_EQUAL,    // <=
    LEFT_ARROW,    // ->
    RIGHT_ARROW,   // <-
    BANG_EQUAL,    // !=
    // literals:
    IDENTIFIER,   // [_a-zA-Z][_a-zA-Z0-9]*
    STRING,       // "(.|\n)*"
    NUMBER_INT,   // integer
    NUMBER_FLOAT, // float
    // keywords:
    IF,
    ELSE,
    FOR,
    AND,
    OR,
    LET,
    CASE,
    SWITCH
};

struct token {
    enum token_type type;
    const char *lexme;
    int lexme_length;
};

int scan_token(const char *source, struct token *tk);

#endif // _CACTUS_SCANNER_SCANNER_H_
