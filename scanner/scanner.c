#include "scanner.h"

#include <assert.h>

/* in_range: lo<=a<=hi */
#define in_range(a, lo, hi)               \
    ({                                    \
        typeof(a) __a  = (a);             \
        typeof(a) __hi = (typeof(a))(hi); \
        typeof(a) __lo = (typeof(a))(hi); \
        __a >= __lo &&__a <= __hi;        \
    })

/* skip_s_t_r: skip space, \t and \r
 * @idx: return the index of the first char that is not space, \t or \r
 * */
static void skip_s_t_r(const char *src, int *idx) {
    char c;
    while ((c = src[*idx]) == ' ' || c == '\t' || c == '\r') {
        (*idx)++;
    }
}

/* scan_identifier:
 * @RETURN: length of the identifier
 * */
static int scan_identifier(const char *src) {
    assert(!in_range(src[0], '0', '9'));
    char c;
    int len = 0;

    while ((c = src[len]) == '_' || in_range(c, 'a', 'z') ||
           in_range(c, 'A', 'Z') || in_range(c, '0', '9')) {
        len++;
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

    return 0;
}

/* scan_number:
 * @RETURN: the length of number
 * */
static int scan_number(const char *src, enum token_type *type) {
    assert(in_range(src[0], '0', '9'));
    char c;
    int len;
    int dot_flag = 0;

    for (len = 0; src[len] != '\0'; len++) {
        if (in_range(src[len], '0', '9')) {
            continue;
        }
        if (!dot_flag && src[len] == '.') {
            continue;
        }
        break;
    }

    *type = dot_flag ? NUMBER_FLOAT : NUMBER_INT;

    return len;
}

/* match_keyword:
 * @len: the length of the string to be matched
 * @RETURN: 0 if no keyword else the token type
 * */
static enum token_type match_keyword(const char *src, int len) {

}
/* scan_operator:
 * @len: the length of the operator
 * @RETURN: the type of the operator if an operator is found else 0
 * */
static enum token_type match_operator(const char *src, int *len) {

}

int scan_token(const char *source, struct token *tk) {
    int idx = 0;
    int len;
    char c;
    enum token_type tkt = 0;

    skip_s_t_r(source, &idx);

    if ((c = source[idx]) == '_' || in_range(c, 'a', 'z') ||
        in_range(c, 'A', 'Z')) { // keyword or identifier
        len = scan_identifier(source + idx);
        assert(len > 0);

        tk->type = IDENTIFIER;

        if ((tkt = match_keyword(source + idx, len)) != 0) {
            tk->type = tkt;
        }

    } else if ((c = source[idx]) == '"') { // string
        len = scan_string(source + idx);
        assert(len > 0);

        tk->type = STRING;
    } else if (in_range(source[idx], '0', '9')) { // number
        len = scan_number(source + idx, &tk->type);
        assert(len > 0);
    } else {
        // operator
        if ((tkt = match_operator(source + idx, &len)) == 0) {
            return 0; // failed to find a lexme
        }
        tk->type = tkt;
    }

    tk->lexme        = source + idx;
    tk->lexme_length = len;

    return idx + len;
}
