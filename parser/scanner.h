#ifndef _CACTUS_PARSER_SCANNER_H_
#define _CACTUS_PARSER_SCANNER_H_

enum token_type {
    // separator, operator:
    LEFT_PAREN = 1, // (
    RIGHT_PAREN,    // )
    LEFT_BRACE,     // {
    RIGHT_BRACE,    // }
    LEFT_BRACKET,   // [
    RIGHT_BRACKET,  // ]
    COMMA,          // ,
    NEW_LINE,       // \n
    VBAR,           // |
    COLON,          // :
    PLUS,           // +
    SLASH,          // /
    STAR,           // *
    PERCENT,        // %
    // need to read two char
    DOT,           // .
    GREATER,       // >
    EQUAL,         // =
    MINUS,         // -
    LESS,          // <
    DOT_DOT,       // ..
    EQUAL_EQUAL,   // ==
    GREATER_EQUAL, // >=
    LESS_EQUAL,    // <=
    LEFT_ARROW,    // <-
    RIGHT_ARROW,   // ->
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
    NOT,
    LET,
    CASE,
    SWITCH,
    MATCH,
    IN,
    YIELD,
    CLOSE
};

struct token {
    enum token_type type;
    const char *lexme;
    int lexme_length;
};

/* scan_token:
 * @tk: return the token scanned. if error, tk->lexme_length indicates the
 * position that raises error
 * @RETURN: the end position in source if something is found, 0 if nothing was 
 * found, negative integer if some error occurs
 * */
int scan_token(const char *source, struct token *tk);

#endif // _CACTUS_PARSER_SCANNER_H_
