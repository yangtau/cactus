#ifndef _LLL_PARSER_PARSER_H_
#define _LLL_PARSER_PARSER_H_

enum token_type {
    // separator, operator:
    TOKEN_LEFT_PAREN = 1, // (
    TOKEN_RIGHT_PAREN,    // )
    TOKEN_LEFT_BRACE,     // {
    TOKEN_RIGHT_BRACE,    // }
    TOKEN_LEFT_BRACKET,   // [
    TOKEN_RIGHT_BRACKET,  // ]
    TOKEN_COMMA,          // ,
    TOKEN_NEW_LINE,       // \n
    TOKEN_VBAR,           // |
    TOKEN_COLON,          // :
    TOKEN_PLUS,           // +
    TOKEN_SLASH,          // /
    TOKEN_STAR,           // *
    TOKEN_PERCENT,        // %
    // need to read two char
    TOKEN_DOT,           // .
    TOKEN_GREATER,       // >
    TOKEN_EQUAL,         // =
    TOKEN_MINUS,         // -
    TOKEN_LESS,          // <
    TOKEN_DOT_DOT,       // ..
    TOKEN_EQUAL_EQUAL,   // ==
    TOKEN_GREATER_EQUAL, // >=
    TOKEN_LESS_EQUAL,    // <=
    TOKEN_LEFT_ARROW,    // <-
    TOKEN_RIGHT_ARROW,   // ->
    TOKEN_BANG_EQUAL,    // !=
    // literals:
    TOKEN_IDENTIFIER,   // [_a-zA-Z][_a-zA-Z0-9]*
    TOKEN_STRING,       // "(.|\n)*"
    TOKEN_NUMBER_INT,   // integer
    TOKEN_NUMBER_FLOAT, // float
    // keywords:
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    TOKEN_LET,
    TOKEN_CASE,
    TOKEN_SWITCH,
    TOKEN_MATCH,
    TOKEN_IN,
    TOKEN_YIELD,
    TOKEN_TOKEN_CLOSE,
};

struct token {
    enum token_type type;
    const char *lexme;
    int lexme_length;
};

/* scan_token:
 * @tk: return the token scanned. if error, tk->lexme_length indicates the
 * position where the error occurs
 * @RETURN: the end position in source if something is found, 0 if nothing was 
 * found, negative integer if some error occurs
 * */
int scan_token(const char *source, struct token *tk);

#endif // _LLL_PARSER_PARSER_H_
