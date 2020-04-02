%{
#define YYDEBUG 1
#define YYERROR_VERBSOE 1

%}

%union {

}

%token int string id float
%token or and not let leftarrow

%%

stat:
     expr              |
     let vars '=' expr ;

expr:
    block     |
    bool_expr ;

block:
     '{' vars leftarrow stat '}'
     '{' stat '}' ;

vars:
    id          |
    vars ',' id ;

args:
    expr          |
    args ',' expr ;

tuple:
     '(' args ')' ;

bool_expr:
    bool_expr or cmp_expr  |
    bool_expr and cmp_expr |
    not cmp_expr           |
    cmp_expr               ;

cmp_expr:
    cmp_expr '>' add_expr |
    cmp_expr '<' add_expr |
    add_expr              ;

add_expr:
    add_expr '+' term |
    add_expr '-' term |
    term              ;

term:
    term '*' unary |
    term '/' unary |
    term '%' unary |
    unary          ;

unary:
     '-' factor |
     factor     ;

factor:
    tuple  |
    id     |
    string |
    int    |
    float  ;

%%
