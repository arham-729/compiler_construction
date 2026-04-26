# C Compiler - Compiler Construction Project

## Project Overview

A complete compiler implementation for a well-defined subset of the C programming language. This project demonstrates understanding of core compilation phases including lexical analysis, parsing, semantic analysis, and intermediate code generation.

### Supported Features

**Core Language Features:**
- ✅ Variables and expressions (arithmetic, relational, logical)
- ✅ Arithmetic operators: `+`, `-`, `*`, `/`
- ✅ Relational operators: `<`, `>`, `<=`, `>=`, `==`, `!=`
- ✅ Logical operators: `&&`, `||`, `!`
- ✅ Control structures: `if/else`, `while`, `for`
- ✅ Functions: User-defined and main function
- ✅ Arrays: Declarations and access
- ✅ Symbol table with scope management (global and local)

**Optimization Features:**
- ✅ Constant folding (e.g., `9 - 7` → `2`)
- ✅ Dead code elimination
- ✅ Unary operator optimization

---

## System Requirements

- **OS:** Windows, Linux, or macOS
- **Compiler Tools:** GCC, Bison, Flex
- **Languages:** C

### Installation

**On Windows (with MinGW/Git Bash):**
```bash
# Install required tools
choco install bison flex gcc
```

**On Linux (Ubuntu/Debian):**
```bash
sudo apt-get install bison flex gcc
```

**On macOS:**
```bash
brew install bison flex gcc
```

---

## Project Structure

```
compiler_construction/
├── src/                 # Handwritten source code
│   ├── lexer.l          # Lexical analyzer (flex)
│   ├── parser.y         # Syntax analyzer (bison)
│   ├── ast.h / ast.c    # AST definitions
│   ├── semantic.h / .c  # Semantic analysis
│   ├── symtab.h / .c    # Symbol table
│   ├── optimize.h / .c  # Code optimization
│   ├── ir.h / ir.c      # Intermediate Representation (TAC)
│   └── main.c           # Compiler driver
├── docs/                # Project documentation
│   ├── TESTS.md
│   ├── COMPILATION_GUIDE.md
│   └── FEATURES_IMPLEMENTED.md
├── bin/                 # Compiled binaries
├── tests/               # Example programs and benchmarks
├── README.md            # Project overview (this file)
└── run_tests.ps1        # Automated test suite
```

### File Descriptions

| File | Purpose |
|------|---------|
| `lexer.l` | Tokenizes input C code (flex rules) |
| `parser.y` | Defines grammar and parsing rules (bison) |
| `ast.h/ast.c` | AST node structures and creation functions |
| `semantic.c` | Type checking and scope validation |
| `symtab.c` | Symbol table with scope management |
| `optimize.c` | Constant folding and dead code elimination |
| `ir.c` | Three-Address Code (TAC) generation |
| `main.c` | Main compiler driver |

---

## Compilation Instructions

### Standard Compilation

```bash
# 1. Generate grammar files
bison -d src/parser.y -o src/parser.tab.c

# 2. Generate lexer files
flex -o src/lex.yy.c src/lexer.l

# 3. Compile the compiler
gcc src/lex.yy.c src/parser.tab.c src/ast.c src/symtab.c src/semantic.c src/optimize.c src/ir.c src/main.c -o bin/compiler.exe
```

**One-line command:**
```bash
bison -d src/parser.y -o src/parser.tab.c; flex -o src/lex.yy.c src/lexer.l; gcc src/lex.yy.c src/parser.tab.c src/ast.c src/symtab.c src/semantic.c src/optimize.c src/ir.c src/main.c -o bin/compiler.exe
```

---

## Running the Compiler

### Interactive Mode

```bash
.\compiler.exe          # Windows
# or
./compiler.exe          # Linux/macOS
```

Then enter your C code and press **Ctrl+Z** (Windows) or **Ctrl+D** (Linux/macOS) to finish.

### Example Input:
```c
int main() {
    int x;
    x = 2;
    while (x < 10) {
        x = x * 2;
    }
    return x;
}
```

### Compiler Output:

The compiler generates output in 4 phases:

1. **Semantic Analysis** - Reports any errors (undeclared variables, etc.)
2. **Symbol Table** - Lists all declared variables and functions
3. **Code Optimization** - Shows constant folding transformations
4. **Intermediate Representation (TAC)** - Three-Address Code output

---

## Output Explanation

The compiler generates **Three-Address Code (TAC)**, an intermediate representation of the code. Example:

```
Func main:
x = 2              # Variable initialization
L0:                # Loop label
t1 = x < 10        # Condition evaluation
if_false t1 goto L1 # Conditional branch
t2 = x * 2         # Temporary computation
x = t2             # Variable assignment
goto L0            # Loop jump
L1:                # Exit label
return x           # Return statement
```

This TAC can be compiled to machine code or interpreted directly.

---

## Compiler Phases Explanation

### Phase 1: Lexical Analysis
- Input: C source code
- Output: Tokens (keywords, identifiers, operators, literals)
- Tool: Flex (lexer.l)

### Phase 2: Syntax Analysis
- Input: Token stream
- Output: Abstract Syntax Tree (AST)
- Tool: Bison (parser.y)

### Phase 3: Semantic Analysis
- Input: AST
- Checks: Variable declarations, function calls, type consistency
- Output: Validated AST + Symbol Table
- Files: semantic.c, symtab.c

### Phase 4: Code Optimization
- Input: AST
- Optimizations: Constant folding, dead code elimination
- Output: Optimized AST
- File: optimize.c

### Phase 5: Intermediate Code Generation
- Input: Optimized AST
- Output: Three-Address Code (TAC)
- File: ir.c

---

## Limitations

- **No actual execution** - Compiler generates TAC but doesn't run code
- **Limited type system** - Only integer type supported
- **No preprocessor** - No #include, #define, etc.
- **No optimization passes** - Only basic optimizations
- **Single scope levels** - Limited to function-level scoping

---

## Example Programs

### Example 1: Simple Variables and Arithmetic
```c
int main() {
    int x;
    int y;
    x = 5;
    y = 10;
    return x + y;
}
```

### Example 2: If-Else Statement
```c
int main() {
    int n;
    n = 7;
    if (n < 10) {
        n = n + 1;
    } else {
        n = n - 1;
    }
    return n;
}
```

### Example 3: While Loop
```c
int main() {
    int i;
    int sum;
    i = 0;
    sum = 0;
    while (i < 5) {
        sum = sum + i;
        i = i + 1;
    }
    return sum;
}
```

### Example 4: For Loop
```c
int main() {
    int i;
    int result;
    result = 1;
    for (i = 0; i < 5; i = i + 1) {
        result = result * 2;
    }
    return result;
}
```

### Example 5: Nested Loops
```c
int main() {
    int i;
    int j;
    int count;
    count = 0;
    i = 0;
    while (i < 3) {
        j = 0;
        while (j < 3) {
            count = count + 1;
            j = j + 1;
        }
        i = i + 1;
    }
    return count;
}
```

### Example 6: Logical Operators
```c
int main() {
    int a;
    int b;
    a = 5;
    b = 10;
    if (a < b && b > 3) {
        a = 1;
    }
    if (a > 10 || b < 5) {
        b = 0;
    }
    return a;
}
```

### Example 7: Function Definition and Call
```c
int add(int a, int b) {
    return a + b;
}

int main() {
    int x;
    int y;
    x = 5;
    y = 10;
    x = add(x, y);
    return x;
}
```

---

## Testing

Detailed test results and methodologies can be found in [docs/TESTS.md](docs/TESTS.md).

### Automated Testing
Run the comprehensive test suite (37 tests):
```powershell
.\run_tests.ps1
```

Run the compiler and test with a simple example:

```bash
.\compiler.exe
```

Input:
```c
int main() {
    int x;
    x = 5 + 3;
    return x;
}
```

Expected output includes:
- ✓ Semantic Analysis (no errors)
- ✓ Symbol Table (with variable x)
- ✓ Code Optimization (8 might be constant-folded from 5+3)
- ✓ TAC generation

---

## Error Handling

The compiler reports semantic errors such as:

- **Undeclared variable:** "Undeclared variable 'x'"
- **Redeclaration:** "Redeclaration of variable 'x'"
- **Undefined function:** "Undefined function 'foo'"
- **Syntax errors:** "Syntax Error: ..."

---

## Implementation Details

### AST Node Types
- `NODE_NUM` - Number literal
- `NODE_ID` - Identifier
- `NODE_BINOP` - Binary operation
- `NODE_UNOP` - Unary operation
- `NODE_ASSIGN` - Assignment
- `NODE_VAR_DECL` - Variable declaration
- `NODE_ARRAY_DECL` - Array declaration
- `NODE_IF` - If statement
- `NODE_WHILE` - While loop
- `NODE_FOR` - For loop
- `NODE_FUNC_DECL` - Function declaration
- `NODE_FUNC_CALL` - Function call
- `NODE_RETURN` - Return statement
- `NODE_SEQ` - Sequence of statements

### TAC Features
- **Temporary variables:** t1, t2, t3, ... for intermediate results
- **Labels:** L0, L1, L2, ... for control flow
- **Conditional jumps:** `if_false` for conditions
- **Unconditional jumps:** `goto` for loops

---

## Code Quality

- **Modular Design:** Separate files for each compilation phase
- **Clear Naming:** Functions and variables are self-documenting
- **Comments:** Key sections explained in code
- **Error Handling:** Semantic validation with informative messages
- **Memory Management:** Dynamic allocation with proper cleanup

---

## Future Enhancements

- [ ] Add LLVM integration for target code generation
- [ ] Implement code execution (interpreter)
- [ ] Support for more data types (float, char, etc.)
- [ ] String literals and printf support
- [ ] More aggressive optimizations (register allocation, etc.)
- [ ] Debugging information (line numbers, error recovery)

---

## References

- Compiler Design by Aho, Sethi, Ullman (Dragon Book)
- Flex & Bison Documentation
- C Language Standard (ISO/IEC 9899:1999)

---

## Authors

Compiler Construction Project - Educational Implementation

## License

Educational use only

---

## Support

For issues or questions:
1. Check the test cases in `TESTS.md`
2. Review example programs in this README
3. Check compiler error messages for semantic issues
4. Verify input syntax matches the supported grammar

---

**Last Updated:** April 2026
**Status:** Fully Functional ✅
