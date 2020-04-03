%{
#define YYDEBUG 1
#define YYERROR_VERBSOE 1

%}

%union {

}

%token int string id float
%token or and not leftarrow sep let def

%%

stats:
     stat           |
     stats sep stat ;

stat:
     expr          |
     vars '=' expr |
     func_def      ;

func_def:
    /*
    def fn = {a, b -> a+b}
    */
    def id '=' block ;

block:
     '{' stats '}'                |
     '{' vars leftarrow stats '}' ;

expr:
    bool_expr    |
    block ;

vars:
    id           |
    vars ',' id  |
    list_pattern ;

list_pattern:  /* [a, *xs], [a, b, *xs], [a, *xs, b] */
    '[' vars ',' list_unpack ']'          |
    '[' vars ',' list_unpack ',' vars ']' ;

list_unpack: /* *xs */
    '*' id;

tuple:
     '(' args ')' ;

args:
    expr          |
    args ',' expr ;

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
