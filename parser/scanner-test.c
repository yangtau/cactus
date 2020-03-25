#include "scanner.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

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
        {IDENTIFIER, "hello", 5},
        {IDENTIFIER, "_lower_", 7},
        {IDENTIFIER, "number231", 9},
        {IDENTIFIER, "x__0__", 6},
        {NUMBER_FLOAT, "234.3243", 8},
        {NUMBER_INT, "01432", 5},
        {NUMBER_INT, "24214", 5},
        {NUMBER_FLOAT, "00.1343214", 10},
        {IDENTIFIER, "ABHS99", 6},
        {IDENTIFIER, "_33", 3},
        {STRING, "\"hello\"", 7},
        {STRING, "\"hello \\\"world\"", 15},
        {0, "", 0},
    };
    test_tpl(test_case, res);
}

static void test_keywords() {
    printf("%s\n", "test keywords...");
    char *test_case =
        "if else for and or not let case switch match and in yield close";
    struct test_res res[] = {
        {IF, "if", 2},       {ELSE, "else", 4},   {FOR, "for", 3},
        {AND, "and", 3},     {OR, "or", 2},       {NOT, "not", 3},
        {LET, "let", 3},     {CASE, "case", 4},   {SWITCH, "switch", 6},
        {MATCH, "match", 5}, {AND, "and", 3},     {IN, "in", 2},
        {YIELD, "yield", 5}, {CLOSE, "close", 5}, {0, "", 0},
    };
    test_tpl(test_case, res);
}

static void test_op_sep() {
    printf("%s\n", "test operators and separators...");

    char *test_case       = "| () {} [] +*/,\n:.%> - = < >= == != <= -> <- ..";
    struct test_res res[] = {
        {VBAR, "|", 1},
        {LEFT_PAREN, "(", 1},
        {RIGHT_PAREN, ")", 1},
        {LEFT_BRACE, "{", 1},
        {RIGHT_BRACE, "}", 1},
        {LEFT_BRACKET, "[", 1},
        {RIGHT_BRACKET, "]", 1},
        {PLUS, "+", 1},
        {STAR, "*", 1},
        {SLASH, "/", 1},
        {COMMA, ",", 1},
        {NEW_LINE, "\n", 1},
        {COLON, ":", 1},
        {DOT, ".", 1},
        {PERCENT, "%", 1},
        {GREATER, ">", 1},
        {MINUS, "-", 1},
        {EQUAL, "=", 1},
        {LESS, "<", 1},
        {GREATER_EQUAL, ">=", 2},
        {EQUAL_EQUAL, "==", 2},
        {BANG_EQUAL, "!=", 2},
        {LESS_EQUAL, "<=", 2},
        {RIGHT_ARROW, "->", 2},
        {LEFT_ARROW, "<-", 2},
        {DOT_DOT, "..", 2},
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
        {NEW_LINE, "\n", 1},
        {LEFT_BRACKET, "[", 1},
        {NUMBER_INT, "1", 1},
        {DOT_DOT, "..", 2},
        {NUMBER_INT, "10", 2},
        {RIGHT_BRACKET, "]", 1},
        {VBAR, "|", 1},
        {NEW_LINE, "\n", 1},
        {IDENTIFIER, "map", 3},
        {LEFT_BRACE, "{", 1},
        {IDENTIFIER, "x", 1},
        {RIGHT_ARROW, "->", 2},
        {IDENTIFIER, "x", 1},
        {STAR, "*", 1},
        {IDENTIFIER, "x", 1},
        {RIGHT_BRACE, "}", 1},
        {VBAR, "|", 1},
        {IDENTIFIER, "filter", 6},
        {LEFT_BRACE, "{", 1},
        {IDENTIFIER, "x", 1},
        {RIGHT_ARROW, "->", 2},
        {IDENTIFIER, "x", 1},
        {PERCENT, "%", 1},
        {NUMBER_INT, "2", 1},
        {EQUAL_EQUAL, "==", 2},
        {NUMBER_INT, "0", 1},
        {RIGHT_BRACE, "}", 1},
        {VBAR, "|", 1},
        {IDENTIFIER, "stdout", 6},
        {0, "", 0},
    };

    test_tpl(test_case, res);

    char *test_case1 =
        "# this a comment\n"
        "[1..100] | echo #puts and continue the stream \n"
        "\t\t| map{_->x * x}\n"
        "\t\t| take 20 #close the stream after take 20 elements\n";

    struct test_res res1[] = {
        {NEW_LINE, "\n", 1},   {LEFT_BRACKET, "[", 1},  {NUMBER_INT, "1", 1},
        {DOT_DOT, "..", 2},    {NUMBER_INT, "100", 3},  {RIGHT_BRACKET, "]", 1},
        {VBAR, "|", 1},        {IDENTIFIER, "echo", 4}, {NEW_LINE, "\n", 1},
        {VBAR, "|", 1},        {IDENTIFIER, "map", 3},  {LEFT_BRACE, "{", 1},
        {IDENTIFIER, "_", 1},  {RIGHT_ARROW, "->", 2},  {IDENTIFIER, "x", 1},
        {STAR, "*", 1},        {IDENTIFIER, "x", 1},    {RIGHT_BRACE, "}", 1},
        {NEW_LINE, "\n", 1},   {VBAR, "|", 1},          {IDENTIFIER, "take", 4},
        {NUMBER_INT, "20", 2}, {NEW_LINE, "\n", 1},     {0, "", 0},
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
