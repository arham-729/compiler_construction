# Demo Checklist

Use this as a short script for the final presentation and live demo.

## Suggested Demo Flow

1. Start with the project objective.
2. Explain the supported C subset.
3. Walk through the compiler pipeline:
   - lexer
   - parser
   - semantic analysis
   - optimization
   - TAC generation
4. Run one valid program that shows optimization.
5. Run one valid program that shows arrays or functions.
6. Run one invalid program that shows semantic diagnostics.
7. Close with limitations and future work.

## Recommended Demo Inputs

### Expression And Optimization

```c
int main() {
    int x;
    x = 5 + 3 * 2 - 1;
    return x;
}
```

What to point out:
- precedence is parsed correctly
- optimizer folds the expression to `10`
- TAC is emitted after optimization

### Functions

```c
int add(int a, int b) {
    return a + b;
}

int main() {
    return add(5, 10);
}
```

What to point out:
- function symbol registration
- parameter tracking
- call-site TAC with `param` instructions

### Arrays

```c
int main() {
    int arr[2][3];
    arr[1][2] = 7;
    return arr[1][2];
}
```

What to point out:
- 2D array parsing
- semantic validation
- flattened TAC addressing

### Error Case

```c
int add(int a, int b) {
    return a + b;
}

int main() {
    return add(1);
}
```

What to point out:
- semantic phase catches the arity mismatch
- optimization and TAC are skipped when semantic errors exist

## Likely Viva Questions

### Why use an AST?
- It separates parsing from later phases and gives optimization and semantic analysis a structured representation.

### Why does TAC matter?
- TAC is a clean intermediate form for control flow, temporaries, and later backend work.

### What scope model is implemented?
- Global scope plus function-level local scope.

### What optimizations are implemented?
- Constant folding, unary folding, and simple dead code elimination.

### What are the current limitations?
- Only `int`/`void`
- no execution backend
- no nested block scope model
- array bounds checking only for constant indexes
