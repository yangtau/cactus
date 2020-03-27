#include "parser.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
// common
#include <lll_error.h>

struct test_res {
    enum token_type type;
    const char *s;
    int len;
};

static void test_tpl(const char *src, struct test_res res[]) {
    int i = 0;
    struct token tk;
    while (*src && res[i].type != 0) {
        int rc = scan_token(src, &tk);
        // printf("---rc = %d\n%s\n", rc, src);
        assert(tk.lexme_length == res[i].len);
        assert(strncmp(tk.lexme, res[i].s, tk.lexme_length) == 0);
        assert(tk.type == res[i++].type);
        src += rc;
    }
    assert(res[i].type == 0);
}

static void test_literals() {
    printf("%s\n", "test literals...");
    char *test_case =
        "hello _lower_ number231 x__0__ 234.3243 01432 24214 "
        "00.1343214ABHS99 _33 \"hello\" \"hello \\\"world\""
        "\"hello\\\n word\\\t\"";
    struct test_res res[] = {
        {TOKEN_IDENTIFIER, "hello", 5},
        {TOKEN_IDENTIFIER, "_lower_", 7},
        {TOKEN_IDENTIFIER, "number231", 9},
        {TOKEN_IDENTIFIER, "x__0__", 6},
        {TOKEN_NUMBER_FLOAT, "234.3243", 8},
        {TOKEN_NUMBER_INT, "01432", 5},
        {TOKEN_NUMBER_INT, "24214", 5},
        {TOKEN_NUMBER_FLOAT, "00.1343214", 10},
        {TOKEN_IDENTIFIER, "ABHS99", 6},
        {TOKEN_IDENTIFIER, "_33", 3},
        {TOKEN_STRING, "\"hello\"", 7},
        {TOKEN_STRING, "\"hello \\\"world\"", 15},
        {0, "", 0},
    };
    test_tpl(test_case, res);
}

static void test_keywords() {
    printf("%s\n", "test keywords...");
    char *test_case =
        "if else for and or not let case switch match and in yield close";
    struct test_res res[] = {
        {TOKEN_IF, "if", 2},       {TOKEN_ELSE, "else", 4},   {TOKEN_FOR, "for", 3},
        {TOKEN_AND, "and", 3},     {TOKEN_OR, "or", 2},       {TOKEN_NOT, "not", 3},
        {TOKEN_LET, "let", 3},     {TOKEN_CASE, "case", 4},   {TOKEN_SWITCH, "switch", 6},
        {TOKEN_MATCH, "match", 5}, {TOKEN_AND, "and", 3},     {TOKEN_IN, "in", 2},
        {TOKEN_YIELD, "yield", 5}, {TOKEN_TOKEN_CLOSE, "close", 5}, {0, "", 0},
    };
    test_tpl(test_case, res);
}

static void test_op_sep() {
    printf("%s\n", "test operators and separators...");

    char *test_case       = "| () {} [] +*/,\n:.%> - = < >= == != <= -> <- ..";
    struct test_res res[] = {
        {TOKEN_VBAR, "|", 1},
        {TOKEN_LEFT_PAREN, "(", 1},
        {TOKEN_RIGHT_PAREN, ")", 1},
        {TOKEN_LEFT_BRACE, "{", 1},
        {TOKEN_RIGHT_BRACE, "}", 1},
        {TOKEN_LEFT_BRACKET, "[", 1},
        {TOKEN_RIGHT_BRACKET, "]", 1},
        {TOKEN_PLUS, "+", 1},
        {TOKEN_STAR, "*", 1},
        {TOKEN_SLASH, "/", 1},
        {TOKEN_COMMA, ",", 1},
        {TOKEN_NEW_LINE, "\n", 1},
        {TOKEN_COLON, ":", 1},
        {TOKEN_DOT, ".", 1},
        {TOKEN_PERCENT, "%", 1},
        {TOKEN_GREATER, ">", 1},
        {TOKEN_MINUS, "-", 1},
        {TOKEN_EQUAL, "=", 1},
        {TOKEN_LESS, "<", 1},
        {TOKEN_GREATER_EQUAL, ">=", 2},
        {TOKEN_EQUAL_EQUAL, "==", 2},
        {TOKEN_BANG_EQUAL, "!=", 2},
        {TOKEN_LESS_EQUAL, "<=", 2},
        {TOKEN_RIGHT_ARROW, "->", 2},
        {TOKEN_LEFT_ARROW, "<-", 2},
        {TOKEN_DOT_DOT, "..", 2},
        {0, "", 0},
    };
    test_tpl(test_case, res);
}

static void test_source_code() {
    printf("%s\n", "test source codes...");
    char *test_case =
        " #\t\r this is a comment\n[1..10] |\n\tmap{x->x * x} | filter{x->x % "
        "2 "
        "== 0} | stdout \t";
    struct test_res res[] = {
        {TOKEN_NEW_LINE, "\n", 1},
        {TOKEN_LEFT_BRACKET, "[", 1},
        {TOKEN_NUMBER_INT, "1", 1},
        {TOKEN_DOT_DOT, "..", 2},
        {TOKEN_NUMBER_INT, "10", 2},
        {TOKEN_RIGHT_BRACKET, "]", 1},
        {TOKEN_VBAR, "|", 1},
        {TOKEN_NEW_LINE, "\n", 1},
        {TOKEN_IDENTIFIER, "map", 3},
        {TOKEN_LEFT_BRACE, "{", 1},
        {TOKEN_IDENTIFIER, "x", 1},
        {TOKEN_RIGHT_ARROW, "->", 2},
        {TOKEN_IDENTIFIER, "x", 1},
        {TOKEN_STAR, "*", 1},
        {TOKEN_IDENTIFIER, "x", 1},
        {TOKEN_RIGHT_BRACE, "}", 1},
        {TOKEN_VBAR, "|", 1},
        {TOKEN_IDENTIFIER, "filter", 6},
        {TOKEN_LEFT_BRACE, "{", 1},
        {TOKEN_IDENTIFIER, "x", 1},
        {TOKEN_RIGHT_ARROW, "->", 2},
        {TOKEN_IDENTIFIER, "x", 1},
        {TOKEN_PERCENT, "%", 1},
        {TOKEN_NUMBER_INT, "2", 1},
        {TOKEN_EQUAL_EQUAL, "==", 2},
        {TOKEN_NUMBER_INT, "0", 1},
        {TOKEN_RIGHT_BRACE, "}", 1},
        {TOKEN_VBAR, "|", 1},
        {TOKEN_IDENTIFIER, "stdout", 6},
        {0, "", 0},
    };

    test_tpl(test_case, res);

    char *test_case1 =
        "# this a comment\n"
        "[1..100] | echo #puts and continue the stream \n"
        "\t\t| map{_->x * x}\n"
        "\t\t| take 20 #close the stream after take 20 elements\n";

    struct test_res res1[] = {
        {TOKEN_NEW_LINE, "\n", 1},   {TOKEN_LEFT_BRACKET, "[", 1},  {TOKEN_NUMBER_INT, "1", 1},
        {TOKEN_DOT_DOT, "..", 2},    {TOKEN_NUMBER_INT, "100", 3},  {TOKEN_RIGHT_BRACKET, "]", 1},
        {TOKEN_VBAR, "|", 1},        {TOKEN_IDENTIFIER, "echo", 4}, {TOKEN_NEW_LINE, "\n", 1},
        {TOKEN_VBAR, "|", 1},        {TOKEN_IDENTIFIER, "map", 3},  {TOKEN_LEFT_BRACE, "{", 1},
        {TOKEN_IDENTIFIER, "_", 1},  {TOKEN_RIGHT_ARROW, "->", 2},  {TOKEN_IDENTIFIER, "x", 1},
        {TOKEN_STAR, "*", 1},        {TOKEN_IDENTIFIER, "x", 1},    {TOKEN_RIGHT_BRACE, "}", 1},
        {TOKEN_NEW_LINE, "\n", 1},   {TOKEN_VBAR, "|", 1},          {TOKEN_IDENTIFIER, "take", 4},
        {TOKEN_NUMBER_INT, "20", 2}, {TOKEN_NEW_LINE, "\n", 1},     {0, "", 0},
    };
    test_tpl(test_case1, res1);
}

static void test_invalid_lexme() {
    struct token tk;
    int rc;
    char *test_case;

    printf("test invalid lexmes...\n");

    // case 1: string error
    test_case = " \"hello";
    rc        = scan_token(test_case, &tk);
    assert(rc == -E_SCANNER_INVALID_STRING);
    assert(tk.lexme_length == 1);

    // case 2: invalid operator
    test_case = "@!~$";

    while (*test_case) {
        rc = scan_token(test_case, &tk);
        assert(rc == -E_SCANNER_INVALID_SYMBOL);
        assert(tk.lexme_length == 0);
        test_case++;
    }
}

static void test() {
    test_op_sep();
    test_keywords();
    test_literals();
    test_source_code();
    test_invalid_lexme();
}

int main() {
    printf("######## scanner test ########\n");
    test();
    printf("Passed all tests\n");
    printf("######## scanner test ########\n");
}
