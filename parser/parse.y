%{
#define YYDEBUG 1
#define YYERROR_VERBSOE 1

%}

%union {

}

%right op_assign
%left  op_and op_or
%right op_not
%left op_eq op_neq op_ge op_le op_gt op_ls
%left op_add op_sub
%left op_mul op_mod op_div

%left op_toleft // |>
%left op_vb     // |

// seperator
%token sep_leftarrow   // ->
       sep_lcb sep_rcb // {}
       sep_lsb sep_rsb // []
       sep_lb sep_rb   // ()
       sep_sem sep_com // ;,
       sep_nl          // \n

// keyword
%token kw_if kw_let kw_match

// literal
%token lit_int lit_float lit_string id

%%

stats:
     stat          |
     stats sep stat;

sep:
   sep_sem|
   sep_nl ;

stat:
     expr    |
     vars_def;

vars_def:
    kw_let vars op_assign expr|
    /* stm |> s */
    stream op_toleft vars  ;

stream:
    /* [1..20] | map id */
    expr op_vb pipeline  |
    stream op_vb pipeline;

pipeline:
    id       |
    func_call;

func_call:
    id args;

vars:
    id             |
    vars sep_com id;

expr:
    bool_expr|
    block    ;

block:
     sep_lcb sep_leftarrow stats sep_rcb     |
     sep_lcb vars sep_leftarrow stats sep_rcb;

list:
    sep_lsb sep_rsb                                 |
    sep_lsb args sep_rsb                            | 
    /* [*id] or [*(func_call)] */
    sep_lsb unpack sep_rsb                          | 
    sep_lsb args sep_com unpack sep_rsb             |
    sep_lsb args sep_com unpack sep_com args sep_rsb;

unpack:
   /* *a */
   op_mul id                     |
   op_mul sep_lb func_call sep_rb;

dict:
    sep_lcb sep_rcb       |
    sep_lcb tuples sep_rcb;

tuples:
    tuple       |
    tuples tuple;

tuple:
    sep_lb sep_rb     |
    sep_lb args sep_rb;

args:
    expr             |
    args sep_com expr;

bool_expr:
    bool_expr op_or cmp_expr |
    bool_expr op_and cmp_expr|
    op_not cmp_expr          |
    cmp_expr              ;

cmp_expr:
    cmp_expr op_gt add_expr|
    cmp_expr op_ls add_expr|
    add_expr               ;

add_expr:
    add_expr op_add term|
    add_expr op_sub term|
    term                ;

term:
    term op_mul unary|
    term op_div unary|
    term op_mod unary|
    unary            ;

unary:
     sep_lb op_sub factor sep_rb|
     factor                     ;

factor:
    sep_lb func_call sep_rb|
    tuple                  |
    list                   |
    dict                   |
    id                     |
    lit_string             |
    lit_int                |
    lit_float              ;

%%
