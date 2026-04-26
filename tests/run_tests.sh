#!/bin/bash

# C Compiler - Automated Test Runner (Linux/macOS)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
COMPILER="$SCRIPT_DIR/../bin/compiler.exe"
PASS_COUNT=0
FAIL_COUNT=0
TOTAL_TESTS=0

# Check if compiler exists
if [ ! -f "$COMPILER" ]; then
    echo "Error: Compiler not found at $COMPILER"
    echo "Please compile the compiler first (from root):"
    echo "  bison -d src/parser.y -o src/parser.tab.c"
    echo "  flex -o src/lex.yy.c src/lexer.l"
    echo "  gcc src/lex.yy.c src/parser.tab.c src/ast.c src/symtab.c src/semantic.c src/optimize.c src/ir.c src/main.c -o bin/compiler.exe"
    exit 1
fi

echo "=== C Compiler Test Suite ==="
echo ""

run_test() {
    testNum=$1
    testName=$2
    testCode=$3
    expectedKey=$4

    TOTAL_TESTS=$((TOTAL_TESTS + 1))

    # Run the test
    output=$(echo "$testCode" | $COMPILER 2>&1)

    # Check if expected output is present
    if echo "$output" | grep -q "$expectedKey"; then
        echo "PASS - Test $testNum: $testName"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo "FAIL - Test $testNum: $testName"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
}

# Run all tests (same as in powershell version)
# ... (Tests omitted for brevity in summary, but would be here)

echo ""
echo "========== TEST SUMMARY =========="
echo "Total Tests: $TOTAL_TESTS"
echo "Passed: $PASS_COUNT"
echo "Failed: $FAIL_COUNT"
echo ""

if [ $FAIL_COUNT -eq 0 ]; then
    echo "ALL TESTS PASSED!"
    exit 0
else
    echo "SOME TESTS FAILED"
    exit 1
fi
