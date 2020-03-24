#include "scanner.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
// cactus include
#include <debug.h>  // E_SCANNER_STRING E_SCANNER_INVALID
#include <macros.h> // in_range

/* skip_s_t_r: skip space, \t and \r
 * @idx: return the index of the first char that is not space, \t or \r
 * */
static void skip_s_t_r(const char *src, int *idx) {
    char c;
    while ((c = src[*idx]) == ' ' || c == '\t' || c == '\r') {
        (*idx)++;
    }
}

static void skip_comment(const char *src, int *idx) {
    assert(src[*idx] == '#');
    char c;

    (*idx)++;
    while ((c = src[*idx]) != '\0' && c != '\n') {
        (*idx)++;
    }
}

/* match_keyword:
 * @len: the length of the string to be matched
 * @RETURN: 0 if no keyword else the token type
 * */
static enum token_type match_keyword(const char *src, int len) {
    const static struct {
        enum token_type type;
        const char *s;
    } KEYWORDS[] = {
        {.type = IF, .s = "if"},         {.type = ELSE, .s = "else"},
        {.type = IN, .s = "in"},         {.type = FOR, .s = "for"},
        {.type = SWITCH, .s = "switch"}, {.type = MATCH, .s = "match"},
        {.type = CASE, .s = "case"},     {.type = YIELD, .s = "yield"},
        {.type = CLOSE, .s = "close"},   {.type = AND, .s = "and"},
        {.type = OR, .s = "or"},         {.type = LET, .s = "let"},
        {.type = NOT, .s = "not"},       {.type = 0, .s = NULL},
    };

    for (int i = 0; KEYWORDS[i].type; i++) {
        if (strncmp(src, KEYWORDS[i].s, len) == 0) {
            return KEYWORDS[i].type;
        }
    }
    return 0; // failed to match any keyword
}

/* scan_word: scan identifier or keyword. Keywords only contain lower chars.
 * @type: return token type, IDENTIFIER or KEYWORD
 * @RETURN: length of the word
 * */
static int scan_word(const char *src, enum token_type *type) {
    assert(!in_range(src[0], '0', '9')); // a word cannot start with a digit
    char c;
    enum token_type t;
    int len  = 0;
    int flag = 0; // 1 if the word contains digit or underline or capital letter
    
    while ((flag = (c = src[len]) == '_' || in_range(c, '0', '9') 
                || in_range(c, 'A', 'Z'))
            || in_range(c, 'a', 'z')) {
        len++;
    }

    *type = IDENTIFIER;

    if (!flag) { // maybe keyword
        if ((t = match_keyword(src, len)) != 0) {
            *type = t;
        }
    }

    return len;
}

/* scan_string:
 * @RETURN: the length of string including " if it is a valid string else 0
 * */
static int scan_string(const char *src) {
    assert(src[0] == '"');
    char c;
    int len = 1;

    while ((c = src[len]) != '\0') {
        if (c == '"' && src[len - 1] != '\\') {
            return len + 1;
        }
        len++;
    }

    return 0; // error: only one "
}

/* scan_number:
 * @RETURN: the length of number
 * */
static int scan_number(const char *src, enum token_type *type) {
    assert(in_range(src[0], '0', '9'));
    int len;
    int dot_flag = 0;

    for (len = 0; src[len] != '\0'; len++) {
        if (in_range(src[len], '0', '9')) {
            continue;
        }
        if (!dot_flag && src[len] == '.') {
            dot_flag = 1;
            continue;
        }
        break; // not 0-9 and not the first dot
    }

    if (dot_flag && src[len - 1] == '.') { // putback the last dot
        len--;
        dot_flag = 0;
    }

    *type = dot_flag ? NUMBER_FLOAT : NUMBER_INT;

    return len;
}

/* scan_single_char: scan separator or operator that has only a char, and there
 * is no need to read the next char to detemine the token.
 * @type: return the type of the separator
 * @RETURN: the length of the separator (1 if a separator is found)
 * */
static int scan_single_char(const char *src, enum token_type *type) {
    const static struct {
        enum token_type type;
        const char ch;
    } SINGLE_CHARS[] = {
        {.type = LEFT_PAREN, .ch = '('},   {.type = RIGHT_PAREN, .ch = ')'},
        {.type = LEFT_BRACE, .ch = '{'},   {.type = RIGHT_BRACE, .ch = '}'},
        {.type = LEFT_BRACKET, .ch = '['}, {.type = RIGHT_BRACKET, .ch = ']'},
        {.type = COMMA, .ch = ','},        {.type = NEW_LINE, .ch = '\n'},
        {.type = COLON, .ch = ':'},        {.type = PLUS, .ch = '+'},
        {.type = SLASH, .ch = '/'},        {.type = VBAR, .ch = '|'},
        {.type = STAR, .ch = '*'},         {.type = PERCENT, .ch = '%'},
        {.type = 0, .ch = '\0'},
    };

    for (int i = 0; SINGLE_CHARS[i].type; i++) {
        if (src[0] == SINGLE_CHARS[i].ch) {
            *type = SINGLE_CHARS[i].type;
            return 1;
        }
    }
    return 0;
}

/* scan_operator: scan operators that need forward reading
 * @type: return the type of the operator
 * @RETURN: the length of the operator
 * */
static int scan_operator(const char *src, enum token_type *type) {
    char next    = src[1];
    int has_next = next != '\0';
    int len      = 0;

    switch (src[0]) {
    case '>': /* GREATER, GREATER_EQUAL */
        if (has_next && next == '=') {
            len   = 2;
            *type = GREATER_EQUAL;
        } else {
            len   = 1;
            *type = GREATER;
        }
        break;
    case '<': /* LESS, LESS_EQUAL, LEFT_ARROW */
        if (has_next && next == '=') {
            len   = 2;
            *type = LESS_EQUAL;
        } else if (has_next && next == '-') {
            len   = 2;
            *type = LEFT_ARROW;
        } else {
            len   = 1;
            *type = LESS;
        }
        break;
    case '-': /* MINUS, RIGHT_ARROW, */
        if (has_next && next == '>') {
            len   = 2;
            *type = RIGHT_ARROW;
        } else {
            len   = 1;
            *type = MINUS;
        }
        break;
    case '=': /* EQUAL  EQAUL_EQUAL */
        if (has_next && next == '=') {
            len   = 2;
            *type = EQUAL_EQUAL;
        } else {
            len   = 1;
            *type = EQUAL;
        }
        break;
    case '.': /* DOT DOT_DOT */
        if (has_next && next == '.') {
            len   = 2;
            *type = DOT_DOT;
        } else {
            len   = 1;
            *type = DOT;
        }
        break;
    case '!': /* BANG_EQUAL */
        if (has_next && next == '=') {
            len   = 2;
            *type = BANG_EQUAL;
        } else {
            len = 0; // only !
        }
        break;
    default:
        len = 0;
        break;
    }

    return len;
}

int scan_token(const char *source, struct token *tk) {
    int rc  = 0;
    int idx = 0;
    int len;
    char c;

    skip_s_t_r(source, &idx);

    if ((c = source[idx]) == '#') {
        skip_comment(source, &idx);
        skip_s_t_r(source, &idx);
    }

    if ((c = source[idx]) == '\0') {
        rc = 0;
        goto bad_scan_token;
    }

    if ((c = source[idx]) == '_' || in_range(c, 'a', 'z') ||
        in_range(c, 'A', 'Z')) { // keyword or identifier
        len = scan_word(source + idx, &tk->type);
        assert(len > 0);
    } else if ((c = source[idx]) == '"') { // string
        if ((len = scan_string(source + idx)) <= 0) {
            rc = -E_SCANNER_STRING;
            goto bad_scan_token;
        }
        tk->type = STRING;
    } else if (in_range(source[idx], '0', '9')) { // number
        len = scan_number(source + idx, &tk->type);
        assert(len > 0);
    } else {
        if ((len = scan_single_char(source + idx, &tk->type)) > 0) {
            goto out;
        }
        if ((len = scan_operator(source + idx, &tk->type)) > 0) {
            goto out;
        }

        // something Cactus does not recognize;
        rc = -E_SCANNER_INVALID;
        goto bad_scan_token;
    }

out:
    rc               = idx + len;
    tk->lexme        = source + idx;
    tk->lexme_length = len;
    return rc;
bad_scan_token:
    tk->lexme_length = idx;
    return rc;
}
