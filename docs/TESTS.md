# Test Plan

The repository includes automated test runners for both Windows PowerShell and Bash:

- [tests/run_tests.ps1](/D:/Uni/Semester%208/CC/compiler_construction/tests/run_tests.ps1)
- [tests/run_tests.sh](/D:/Uni/Semester%208/CC/compiler_construction/tests/run_tests.sh)

Both runners execute the same 44 regression checks.

## How To Run

### Windows PowerShell

```powershell
.\tests\run_tests.ps1
```

### Linux/macOS Bash

```bash
bash tests/run_tests.sh
```

## Coverage Areas

### Core language
- declarations and assignments
- arithmetic expressions
- relational operators
- logical operators
- nested expressions and precedence

### Control flow
- `if`
- `if/else`
- nested `if`
- `while`
- nested `while`
- `for`

### Functions
- function declaration
- function return values
- local variables
- function calls
- nested calls
- arity mismatch diagnostics

### Arrays
- 1D declarations and access
- 2D declarations and access
- constant out-of-bounds diagnostics

### Optimization and TAC quality
- constant folding
- unary folding
- dead-code cleanup
- duplicate-return regression check

### Target code generation
- stack-VM instruction emission
- function-call emission
- correct 2D-array width in backend lowering

### Error reporting
- undeclared variable use
- redeclaration
- undefined function calls

## Test Philosophy

The suite intentionally checks observable compiler behavior instead of only searching for broad phase names in the output. Examples include:

- validating that `5 + 3 * 2 - 1` folds to `10`
- validating the exact function-arity diagnostic
- validating the array bounds diagnostic
- ensuring TAC does not emit a duplicate `return`

## Manual Demo Programs

Helpful sample inputs for live demonstration:

### Optimization

```c
int main() {
    int x;
    x = 5 + 3 * 2 - 1;
    return x;
}
```

### Functions

```c
int add(int a, int b) {
    return a + b;
}

int main() {
    return add(5, 10);
}
```

### Arrays

```c
int main() {
    int arr[2][3];
    arr[1][2] = 7;
    return arr[1][2];
}
```
