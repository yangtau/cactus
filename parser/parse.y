%{
#define YYDEBUG 1
#define YYERROR_VERBSOE 1

%}

%union {

}

%token int string id float
%token or and not leftarrow sep let def to_left /* |> */

%%

stats:
     stat           |
     stats sep stat ;

stat:
     expr      |
     vars_def  ;

vars_def:
    let vars '=' expr   |
    /* stm |> s */
    stream to_left vars ;

stream:
    /* [1..20] | map id */
    expr '|' pipeline    |
    stream '|' pipeline  ;

pipeline:
    id         |
    func_call  ;

func_call:
    id args;

vars:
    id           |
    vars ',' id  ;

expr:
    bool_expr |
    block     ;

block:
     '{' leftarrow stats '}'      |
     '{' vars leftarrow stats '}' ;

list:
    '[' ']'                          |
    '[' args ']'                     | 
    /* [*id] or [*(func_call)] */
    '[' unpack ']'                   | 
    '[' args ',' unpack ']'          |
    '[' args ',' unpack ',' args ']' ;

unpack:
   '*' id                |
   '*' '(' func_call ')' ;

dict:
    '{' '}'        |
    '{' tuples '}' ;

tuples:
    tuple        |
    tuples tuple ;

tuple:
    '(' ')'      |
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
     '(' '-' factor  ')' |
     factor              ;

factor:
    '(' func_call  ')'|
    tuple             |
    list              |
    dict              |
    id                |
    string            |
    int               |
    float             ;

%%
