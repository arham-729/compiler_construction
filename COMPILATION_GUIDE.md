# Compilation Guide

## Quick Start

### One-Line Build (All Platforms)

```bash
bison -d parser.y && flex lexer.l && gcc lex.yy.c parser.tab.c ast.c symtab.c semantic.c optimize.c ir.c main.c -o compiler.exe
```

---

## Step-by-Step Compilation

### Step 1: Generate Parser from Grammar

```bash
bison -d parser.y
```

**Output Files:**
- `parser.tab.c` - Parser implementation
- `parser.tab.h` - Parser header (contains token definitions)

**Flags:**
- `-d` - Generate header file

---

### Step 2: Generate Lexer from Rules

```bash
flex lexer.l
```

**Output Files:**
- `lex.yy.c` - Lexer implementation

**Flags:**
- (none required for this project)

---

### Step 3: Compile All Source Files

```bash
gcc lex.yy.c parser.tab.c ast.c symtab.c semantic.c optimize.c ir.c main.c -o compiler.exe
```

**Source Files Compiled:**
| File | Purpose |
|------|---------|
| `lex.yy.c` | Lexer (generated from lexer.l) |
| `parser.tab.c` | Parser (generated from parser.y) |
| `ast.c` | AST implementation |
| `symtab.c` | Symbol table |
| `semantic.c` | Semantic analyzer |
| `optimize.c` | Optimizer |
| `ir.c` | Intermediate code generator |
| `main.c` | Compiler driver |

**Output:**
- `compiler.exe` - The compiled compiler executable

---

## Platform-Specific Instructions

### Windows (MinGW with Git Bash)

```bash
# Navigate to project directory
cd d:\important\cc_project\compiler_construction

# Build
bison -d parser.y && flex lexer.l && gcc lex.yy.c parser.tab.c ast.c symtab.c semantic.c optimize.c ir.c main.c -o compiler.exe

# Run
.\compiler.exe
```

### Linux (Ubuntu/Debian)

```bash
# Install required tools
sudo apt-get install bison flex gcc

# Navigate to project directory
cd ~/cc_project/compiler_construction

# Build
bison -d parser.y && flex lexer.l && gcc lex.yy.c parser.tab.c ast.c symtab.c semantic.c optimize.c ir.c main.c -o compiler

# Run
./compiler
```

### macOS

```bash
# Install required tools (using Homebrew)
brew install bison flex gcc

# Navigate to project directory
cd ~/cc_project/compiler_construction

# Build
bison -d parser.y && flex lexer.l && gcc lex.yy.c parser.tab.c ast.c symtab.c semantic.c optimize.c ir.c main.c -o compiler

# Run
./compiler
```

---

## Troubleshooting

### Error: "bison: command not found"

**Windows:**
```bash
choco install bison
# or download from https://gnuwin32.sourceforge.net/packages/bison.htm
```

**Linux:**
```bash
sudo apt-get install bison
```

**macOS:**
```bash
brew install bison
```

---

### Error: "flex: command not found"

**Windows:**
```bash
choco install flex
# or download from https://gnuwin32.sourceforge.net/packages/flex.htm
```

**Linux:**
```bash
sudo apt-get install flex
```

**macOS:**
```bash
brew install flex
```

---

### Error: "gcc: command not found"

**Windows:**
```bash
# Install MinGW or use MinGW-w64
choco install mingw
```

**Linux:**
```bash
sudo apt-get install gcc
```

**macOS:**
```bash
brew install gcc
```

---

### Compilation Warnings (Can be ignored)

```
warning: 5 shift/reduce conflicts [-Wconflicts-sr]
```

These are normal bison warnings due to the grammar having some ambiguity. They don't affect compiler functionality.

---

### Linking Errors

If you get linking errors like "undefined reference to...", make sure:

1. All source files are included in the gcc command
2. Flex and bison generated files are included
3. The order of files doesn't matter with gcc

```bash
# Correct order doesn't matter with gcc
gcc lex.yy.c parser.tab.c ast.c symtab.c semantic.c optimize.c ir.c main.c -o compiler.exe
# Same as
gcc main.c ast.c semantic.c symtab.c optimize.c ir.c parser.tab.c lex.yy.c -o compiler.exe
```

---

## Clean Build (Remove Generated Files)

### Windows (PowerShell)

```powershell
Remove-Item -Path lex.yy.c, parser.tab.c, parser.tab.h, compiler.exe -Force -ErrorAction SilentlyContinue
```

### Linux/macOS (Bash)

```bash
rm -f lex.yy.c parser.tab.c parser.tab.h compiler
```

Then rebuild:
```bash
bison -d parser.y && flex lexer.l && gcc lex.yy.c parser.tab.c ast.c symtab.c semantic.c optimize.c ir.c main.c -o compiler
```

---

## Build Verification

After compilation, verify the executable was created:

### Windows
```powershell
Test-Path .\compiler.exe
```

### Linux/macOS
```bash
ls -la ./compiler
```

### Run the compiler
```bash
# Windows
.\compiler.exe

# Linux/macOS
./compiler
```

Then paste test code and press Ctrl+Z (Windows) or Ctrl+D (Linux/macOS).

---

## Advanced Build Options

### Enable Debug Symbols (for debugging)

```bash
bison -d parser.y && flex lexer.l && gcc -g lex.yy.c parser.tab.c ast.c symtab.c semantic.c optimize.c ir.c main.c -o compiler
```

### Enable Optimization

```bash
bison -d parser.y && flex lexer.l && gcc -O2 lex.yy.c parser.tab.c ast.c symtab.c semantic.c optimize.c ir.c main.c -o compiler
```

### With All Warnings

```bash
bison -d parser.y && flex lexer.l && gcc -Wall -Wextra lex.yy.c parser.tab.c ast.c symtab.c semantic.c optimize.c ir.c main.c -o compiler
```

---

## Using Make (Optional)

Create a `Makefile`:

```makefile
CC = gcc
BISON = bison
FLEX = flex

SOURCES = lex.yy.c parser.tab.c ast.c symtab.c semantic.c optimize.c ir.c main.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = compiler

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

lex.yy.c: lexer.l
	$(FLEX) lexer.l

parser.tab.c parser.tab.h: parser.y
	$(BISON) -d parser.y

clean:
	rm -f $(OBJECTS) lex.yy.c parser.tab.c parser.tab.h $(TARGET)

.PHONY: all clean
```

Then build with:
```bash
make
```

And clean with:
```bash
make clean
```

---

## Files Generated During Build

| File | Purpose | Can Delete? |
|------|---------|------------|
| `lex.yy.c` | Generated lexer | Yes (regenerated from lexer.l) |
| `parser.tab.c` | Generated parser | Yes (regenerated from parser.y) |
| `parser.tab.h` | Generated parser header | Yes (regenerated from parser.y) |
| `compiler.exe` | Executable | Yes (rebuilt) |
| `*.o` | Object files | Yes (rebuilt) |

All other files (lexer.l, parser.y, ast.c, etc.) are source files and should NOT be deleted.

---

## Version Information

- **GCC:** 7.0 or later
- **Bison:** 3.0 or later
- **Flex:** 2.5 or later

Check versions:
```bash
gcc --version
bison --version
flex --version
```

---

**Last Updated:** April 2026
