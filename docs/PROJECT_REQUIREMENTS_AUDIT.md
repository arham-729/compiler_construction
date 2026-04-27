# Project Requirements Audit

This audit maps the repository to the Spring 2026 compiler-construction brief.

## Status Legend

- `Handled`: present in the repository and aligned with the brief
- `Prepared`: supporting artifact exists, but the final assessed performance is still live/human

## 1. Required Components

| Requirement | Status | Evidence |
|-------------|--------|----------|
| Lexical analyzer | `Handled` | [src/lexer.l](/D:/Uni/Semester%208/CC/compiler_construction/src/lexer.l) |
| Syntax analyzer | `Handled` | [src/parser.y](/D:/Uni/Semester%208/CC/compiler_construction/src/parser.y) |
| Semantic analysis | `Handled` | [src/semantic.c](/D:/Uni/Semester%208/CC/compiler_construction/src/semantic.c), [src/symtab.c](/D:/Uni/Semester%208/CC/compiler_construction/src/symtab.c) |
| Intermediate representation (TAC) | `Handled` | [src/ir.c](/D:/Uni/Semester%208/CC/compiler_construction/src/ir.c) |

## 2. Defined Subset Of C

| Requirement | Status | Evidence |
|-------------|--------|----------|
| Variables and expressions | `Handled` | [src/parser.y](/D:/Uni/Semester%208/CC/compiler_construction/src/parser.y), [src/optimize.c](/D:/Uni/Semester%208/CC/compiler_construction/src/optimize.c) |
| Control structures (`if`, `while`, optional `for`) | `Handled` | [src/parser.y](/D:/Uni/Semester%208/CC/compiler_construction/src/parser.y), [src/ir.c](/D:/Uni/Semester%208/CC/compiler_construction/src/ir.c) |
| Functions (`main` and user-defined) | `Handled` | [src/parser.y](/D:/Uni/Semester%208/CC/compiler_construction/src/parser.y), [src/semantic.c](/D:/Uni/Semester%208/CC/compiler_construction/src/semantic.c) |
| Symbol table with global and local scope | `Handled` | [src/symtab.c](/D:/Uni/Semester%208/CC/compiler_construction/src/symtab.c) |

## 3. Optional Features

| Requirement | Status | Evidence |
|-------------|--------|----------|
| Array support | `Handled` | [src/parser.y](/D:/Uni/Semester%208/CC/compiler_construction/src/parser.y), [src/semantic.c](/D:/Uni/Semester%208/CC/compiler_construction/src/semantic.c), [src/ir.c](/D:/Uni/Semester%208/CC/compiler_construction/src/ir.c) |
| Code optimization | `Handled` | [src/optimize.c](/D:/Uni/Semester%208/CC/compiler_construction/src/optimize.c) |
| Target code generation | `Handled` | [src/target.c](/D:/Uni/Semester%208/CC/compiler_construction/src/target.c) |
| Implementation in a non-C language | Partial | The main compiler remains in C, but a separate Python implementation of the TAC-to-target-code phase exists under [experimental/python_backend](/D:/Uni/Semester%208/CC/compiler_construction/experimental/python_backend). |

## 4. Deliverables

### Project Report

| Item | Status | Evidence |
|------|--------|----------|
| Introduction and objectives | `Handled` | [README.md](/D:/Uni/Semester%208/CC/compiler_construction/README.md) |
| Clearly defined subset of C | `Handled` | [README.md](/D:/Uni/Semester%208/CC/compiler_construction/README.md), [docs/FEATURES_IMPLEMENTED.md](/D:/Uni/Semester%208/CC/compiler_construction/docs/FEATURES_IMPLEMENTED.md) |
| Grammar specification | `Handled` | [docs/GRAMMAR_SPECIFICATION.md](/D:/Uni/Semester%208/CC/compiler_construction/docs/GRAMMAR_SPECIFICATION.md) |
| Design of compiler phases | `Handled` | [README.md](/D:/Uni/Semester%208/CC/compiler_construction/README.md) |
| Sample inputs and outputs | `Handled` | [README.md](/D:/Uni/Semester%208/CC/compiler_construction/README.md), [docs/TESTS.md](/D:/Uni/Semester%208/CC/compiler_construction/docs/TESTS.md) |
| Limitations and future work | `Handled` | [README.md](/D:/Uni/Semester%208/CC/compiler_construction/README.md), [docs/FEATURES_IMPLEMENTED.md](/D:/Uni/Semester%208/CC/compiler_construction/docs/FEATURES_IMPLEMENTED.md) |

### Executable Code

| Item | Status | Evidence |
|------|--------|----------|
| Working implementation | `Handled` | [bin/compiler.exe](/D:/Uni/Semester%208/CC/compiler_construction/bin/compiler.exe), 44/44 PowerShell regression checks passing |
| Clear compile/run instructions | `Handled` | [README.md](/D:/Uni/Semester%208/CC/compiler_construction/README.md), [docs/COMPILATION_GUIDE.md](/D:/Uni/Semester%208/CC/compiler_construction/docs/COMPILATION_GUIDE.md) |
| Well-structured and readable code | `Handled` | modular `src/` layout |
| Test cases | `Handled` | [tests/run_tests.ps1](/D:/Uni/Semester%208/CC/compiler_construction/tests/run_tests.ps1), [tests/run_tests.sh](/D:/Uni/Semester%208/CC/compiler_construction/tests/run_tests.sh), [docs/TESTS.md](/D:/Uni/Semester%208/CC/compiler_construction/docs/TESTS.md) |

### Presentation

| Item | Status | Evidence |
|------|--------|----------|
| Project overview | `Handled` | [deliverables/compiler_construction_presentation.pptx](/D:/Uni/Semester%208/CC/compiler_construction/deliverables/compiler_construction_presentation.pptx) |
| Design explanation | `Handled` | presentation deck and grammar/design docs |
| Live demonstration | `Prepared` | [docs/DEMO_CHECKLIST.md](/D:/Uni/Semester%208/CC/compiler_construction/docs/DEMO_CHECKLIST.md) |

### Demo

| Item | Status | Evidence |
|------|--------|----------|
| Understanding of implementation | `Prepared` | [docs/DEMO_CHECKLIST.md](/D:/Uni/Semester%208/CC/compiler_construction/docs/DEMO_CHECKLIST.md) |
| Ability to answer technical questions | `Prepared` | [docs/DEMO_CHECKLIST.md](/D:/Uni/Semester%208/CC/compiler_construction/docs/DEMO_CHECKLIST.md) |

## Bottom Line

All repository-based requirements from the brief are now covered.

The only items that remain inherently live are the final presentation delivery and the spoken demo/Q&A, and the repo now includes support materials for those as well.
