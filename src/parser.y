/* parser.y */
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex();
void yyerror(const char *s);

ASTNode *root;
%}

%union {
    int num;
    char *str;
    struct ASTNode *node;
}

%token <str> TYPE ID ARITH_OP REL_OP
%token <num> NUM
%token IF ELSE WHILE FOR RETURN ASSIGN SEMICOLON LPAREN RPAREN LBRACE RBRACE
%token AND OR NOT LBRACKET RBRACKET COMMA

%type <node> program item statement expr assignment declaration func_decl func_body
%type <node> param_list params if_statement while_statement for_statement return_statement
%type <node> block statement_list arg_list

%left OR
%left AND
%left REL_OP
%left ARITH_OP

%%
program:
    item { root = $1; }
    | program item { root = create_seq_node($1, $2); }
    ;

item:
    declaration { $$ = $1; }
    | func_decl { $$ = $1; }
    ;

declaration:
    TYPE ID SEMICOLON { $$ = create_var_decl_node($1, $2); }
    | TYPE ID LBRACKET NUM RBRACKET SEMICOLON { $$ = create_array_decl_node($1, $2, $4); }
    | TYPE ID LBRACKET NUM RBRACKET LBRACKET NUM RBRACKET SEMICOLON { $$ = create_array_decl_2d_node($1, $2, $4, $7); }
    ;

func_decl:
    TYPE ID LPAREN param_list RPAREN func_body { $$ = create_func_decl_node($1, $2, $4, $6); }
    | TYPE ID LPAREN RPAREN func_body { $$ = create_func_decl_node($1, $2, NULL, $5); }
    ;

param_list:
    params { $$ = $1; }
    ;

params:
    TYPE ID { $$ = create_var_decl_node($1, $2); }
    | params COMMA TYPE ID { $$ = $1; $$->next = create_var_decl_node($3, $4); }
    ;

func_body:
    LBRACE statement_list RBRACE { $$ = $2; }
    | LBRACE RBRACE { $$ = NULL; }
    ;

block:
    LBRACE statement_list RBRACE { $$ = $2; }
    | LBRACE RBRACE { $$ = NULL; }
    | statement { $$ = $1; }
    ;

statement_list:
    statement { $$ = $1; }
    | statement_list statement { $$ = create_seq_node($1, $2); }
    ;

statement:
    declaration { $$ = $1; }
    | assignment SEMICOLON { $$ = $1; }
    | if_statement { $$ = $1; }
    | while_statement { $$ = $1; }
    | for_statement { $$ = $1; }
    | return_statement SEMICOLON { $$ = $1; }
    ;

assignment:
    ID ASSIGN expr { $$ = create_assign_node($1, $3); }
    | ID LBRACKET expr RBRACKET ASSIGN expr { $$ = create_array_assign_node($1, $3, $6); }
    | ID LBRACKET expr RBRACKET LBRACKET expr RBRACKET ASSIGN expr { $$ = create_array_assign_2d_node($1, $3, $6, $9); }
    ;

if_statement:
    IF LPAREN expr RPAREN block ELSE block { $$ = create_if_node($3, $5, $7); }
    | IF LPAREN expr RPAREN block { $$ = create_if_node($3, $5, NULL); }
    ;

while_statement:
    WHILE LPAREN expr RPAREN block { $$ = create_while_node($3, $5); }
    ;

for_statement:
    FOR LPAREN assignment SEMICOLON expr SEMICOLON assignment RPAREN block 
    { $$ = create_for_node($3, $5, $7, $9); }
    ;

return_statement:
    RETURN expr { $$ = create_return_node($2); }
    | RETURN { $$ = create_return_node(NULL); }
    ;

expr:
    NUM { $$ = create_num_node($1); }
    | ID { $$ = create_id_node($1); }
    | ID LBRACKET expr RBRACKET { $$ = create_array_access_node($1, $3); }
    | ID LBRACKET expr RBRACKET LBRACKET expr RBRACKET { $$ = create_array_access_2d_node($1, $3, $6); }
    | ID LPAREN arg_list RPAREN { $$ = create_func_call_node($1, $3); }
    | ID LPAREN RPAREN { $$ = create_func_call_node($1, NULL); }
    | expr ARITH_OP expr { $$ = create_binop_node($2, $1, $3); }
    | expr REL_OP expr { $$ = create_binop_node($2, $1, $3); }
    | expr AND expr { $$ = create_binop_node("&&", $1, $3); }
    | expr OR expr { $$ = create_binop_node("||", $1, $3); }
    | NOT expr { $$ = create_unop_node("!", $2); }
    | LPAREN expr RPAREN { $$ = $2; }
    ;

arg_list:
    expr { $$ = create_arg_list_node($1, NULL); }
    | arg_list COMMA expr { $$ = create_arg_list_node($1, $3); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error: %s\n", s);
}