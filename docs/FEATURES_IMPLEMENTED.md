# Features Implemented

This document summarizes the current compiler against the Spring 2026 project brief.

## Required Components

### Lexical Analyzer
- Implemented in [src/lexer.l](/D:/Uni/Semester%208/CC/compiler_construction/src/lexer.l)
- Recognizes keywords, identifiers, integer literals, operators, delimiters, and reports invalid characters

### Syntax Analyzer
- Implemented in [src/parser.y](/D:/Uni/Semester%208/CC/compiler_construction/src/parser.y)
- Parses:
  - variable declarations
  - 1D and 2D array declarations
  - assignments
  - `if` / `else`
  - `while`
  - `for`
  - function declarations and calls
  - `return`
- Expression precedence is separated correctly across:
  - logical OR
  - logical AND
  - relational operators
  - additive operators
  - multiplicative operators
  - unary NOT

### Semantic Analysis
- Implemented in [src/semantic.c](/D:/Uni/Semester%208/CC/compiler_construction/src/semantic.c) and [src/symtab.c](/D:/Uni/Semester%208/CC/compiler_construction/src/symtab.c)
- Checks:
  - variable redeclaration
  - undeclared identifiers
  - function existence
  - function-call argument count
  - array-vs-scalar misuse
  - constant out-of-bounds array indexing
- Scope support:
  - global scope
  - function-level local scope

### Intermediate Representation
- Implemented in [src/ir.c](/D:/Uni/Semester%208/CC/compiler_construction/src/ir.c)
- Generates Three-Address Code for:
  - expressions
  - assignments
  - arrays
  - branches
  - loops
  - functions
  - returns
  - function calls

### Target Code Generation
- Implemented in [src/target.c](/D:/Uni/Semester%208/CC/compiler_construction/src/target.c)
- Generates stack-machine target code for:
  - integer expressions
  - variables and arrays
  - branching and loops
  - function calls and returns

## Defined Subset Of C

### Supported
- `int` and `void`
- arithmetic operators: `+`, `-`, `*`, `/`
- relational operators: `<`, `>`, `<=`, `>=`, `==`, `!=`
- logical operators: `&&`, `||`, `!`
- variable declarations and assignments
- arrays
- `if`, `while`, `for`
- user-defined functions and `main`
- global and function-local symbols

### Not Supported
- floating-point types
- characters and strings
- pointers
- structs
- preprocessor directives
- execution of the generated target code

## Optional Features Implemented

### Array Support
- 1D arrays
- 2D arrays
- array reads and writes in TAC
- constant bounds diagnostics in semantic analysis

### Optimization
- constant folding
- unary folding
- dead code elimination for unused expression statements

### Target backend
- stack-VM target code generation after TAC

### Experimental non-C phase
- Separate Python implementation of TAC-to-target-code lowering in [experimental/python_backend/python_target_codegen.py](/D:/Uni/Semester%208/CC/compiler_construction/experimental/python_backend/python_target_codegen.py)
- Kept outside the main compiler build as a parallel backend experiment

## Current Limitations
- Local scope is tracked at function level; nested block scopes are not modeled as separate symbol-table scopes.
- Array bounds checking only triggers when the index is a compile-time constant.
- The project does not yet include a runtime or assembler for the generated stack-VM code.
- The Python backend is a parallel phase implementation, not a full compiler rewrite in another language.
