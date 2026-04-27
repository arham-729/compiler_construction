# Compilation Guide

## Fastest Build Path

If you already have the generated parser and lexer files in the repo, a GCC-only rebuild is enough:

```bash
gcc src/lex.yy.c src/parser.tab.c src/ast.c src/symtab.c src/semantic.c src/optimize.c src/ir.c src/target.c src/main.c -o bin/compiler.exe
```

This is the simplest option on machines where `gcc` is installed but `bison` and `flex` are not.

## Full Regeneration Build

If you want to regenerate the parser and lexer from source:

```bash
bison -d src/parser.y -o src/parser.tab.c
flex -o src/lex.yy.c src/lexer.l
gcc src/lex.yy.c src/parser.tab.c src/ast.c src/symtab.c src/semantic.c src/optimize.c src/ir.c src/target.c src/main.c -o bin/compiler.exe
```

## Run

```bash
.\bin\compiler.exe
```

Then paste a program and finish input with:

- `Ctrl+Z` on Windows
- `Ctrl+D` on Linux/macOS

## Example

```c
int main() {
    int x;
    x = 5 + 3 * 2 - 1;
    return x;
}
```

## Tool Requirements

### Minimum
- `gcc`

### For source regeneration
- `bison`
- `flex`

## Verification

After building, run:

```powershell
.\tests\run_tests.ps1
```

or:

```bash
bash tests/run_tests.sh
```

## Notes

- The repository checks in `src/parser.tab.c`, `src/parser.tab.h`, and `src/lex.yy.c`, so regeneration is optional.
- If `bison` or `flex` are missing on a demo machine, you can still rebuild from the generated sources with GCC only.
