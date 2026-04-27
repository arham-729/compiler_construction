# Grammar Specification

This is a report-friendly summary of the grammar implemented in [src/parser.y](/D:/Uni/Semester%208/CC/compiler_construction/src/parser.y).

## High-Level Grammar

```ebnf
program         ::= item | program item
item            ::= declaration | func_decl

declaration     ::= TYPE ID ";"
                  | TYPE ID "[" NUM "]" ";"
                  | TYPE ID "[" NUM "]" "[" NUM "]" ";"

func_decl       ::= TYPE ID "(" param_list ")" func_body
                  | TYPE ID "(" ")" func_body

param_list      ::= params
params          ::= TYPE ID | params "," TYPE ID

func_body       ::= "{" statement_list "}" | "{" "}"
block           ::= "{" statement_list "}" | "{" "}" | statement

statement_list  ::= statement | statement_list statement
statement       ::= declaration
                  | assignment ";"
                  | if_statement
                  | while_statement
                  | for_statement
                  | return_statement ";"

assignment      ::= ID "=" expr
                  | ID "[" expr "]" "=" expr
                  | ID "[" expr "]" "[" expr "]" "=" expr

if_statement    ::= "if" "(" expr ")" block "else" block
                  | "if" "(" expr ")" block

while_statement ::= "while" "(" expr ")" block

for_statement   ::= "for" "(" assignment ";" expr ";" assignment ")" block

return_statement ::= "return" expr | "return"
```

## Expression Grammar

```ebnf
expr            ::= logical_or

logical_or      ::= logical_and
                  | logical_or "||" logical_and

logical_and     ::= relational
                  | logical_and "&&" relational

relational      ::= additive
                  | relational REL_OP additive

additive        ::= multiplicative
                  | additive ADD_OP multiplicative

multiplicative  ::= unary
                  | multiplicative MUL_OP unary

unary           ::= "!" unary | primary

primary         ::= NUM
                  | ID
                  | ID "[" expr "]"
                  | ID "[" expr "]" "[" expr "]"
                  | ID "(" arg_list ")"
                  | ID "(" ")"
                  | "(" expr ")"

arg_list        ::= expr | arg_list "," expr
```

## Operator Precedence

From lowest to highest:

1. `||`
2. `&&`
3. relational operators
4. `+`, `-`
5. `*`, `/`
6. unary `!`

## Notes

- Only `int` and `void` are part of the supported subset.
- `for` loops are implemented even though the project brief marks them as optional.
- Arrays are limited to one and two dimensions in the current grammar.
