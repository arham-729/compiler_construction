#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
COMPILER="$SCRIPT_DIR/../bin/compiler.exe"
PASS_COUNT=0
FAIL_COUNT=0
TOTAL_TESTS=0

if [ ! -f "$COMPILER" ]; then
    echo "Error: Compiler not found at $COMPILER"
    echo "Please compile it first:"
    echo "  gcc src/lex.yy.c src/parser.tab.c src/ast.c src/symtab.c src/semantic.c src/optimize.c src/ir.c src/target.c src/main.c -o bin/compiler.exe"
    exit 1
fi

run_test() {
    local test_num="$1"
    local test_name="$2"
    local test_code="$3"
    local expected="$4"

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    local output
    output=$(echo "$test_code" | "$COMPILER" 2>&1)

    if echo "$output" | grep -Fq "$expected"; then
        echo "PASS - Test $test_num: $test_name"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo "FAIL - Test $test_num: $test_name"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
}

run_test_regex() {
    local test_num="$1"
    local test_name="$2"
    local test_code="$3"
    local pattern="$4"

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    local output
    output=$(echo "$test_code" | "$COMPILER" 2>&1)

    if echo "$output" | grep -Eq "$pattern"; then
        echo "PASS - Test $test_num: $test_name"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo "FAIL - Test $test_num: $test_name"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
}

run_test_not_regex() {
    local test_num="$1"
    local test_name="$2"
    local test_code="$3"
    local pattern="$4"

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    local output
    output=$(echo "$test_code" | "$COMPILER" 2>&1)

    if ! echo "$output" | grep -Eq "$pattern"; then
        echo "PASS - Test $test_num: $test_name"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo "FAIL - Test $test_num: $test_name"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
}

echo "=== C Compiler Test Suite ==="
echo

echo "--- BASIC TESTS ---"
run_test "1.1" "Variable Declaration" \
    'int main() { int x; x = 5; return x; }' \
    "VAR_DECL"
run_test "1.2" "Multiple Variables" \
    'int main() { int x; int y; x = 10; y = 20; return x; }' \
    "SEQ"
run_test "1.3" "Arithmetic Operations" \
    'int main() { int result; result = 10 + 5; return result; }' \
    "Folded constants"

echo
echo "--- ARITHMETIC EXPRESSION TESTS ---"
run_test "2.1" "Addition" \
    'int main() { int c; int a; int b; a = 10; b = 20; c = a + b; return c; }' \
    "ASSIGN"
run_test "2.2" "Subtraction" \
    'int main() { int x; x = 50 - 25; return x; }' \
    "Folded constants"
run_test "2.3" "Multiplication" \
    'int main() { int x; int y; x = 7; y = x * 3; return y; }' \
    "BINOP"
run_test "2.4" "Division" \
    'int main() { int x; x = 100 / 5; return x; }' \
    "Folded constants"

echo
echo "--- RELATIONAL OPERATOR TESTS ---"
run_test "3.1" "Less Than" \
    'int main() { int x; x = 5; if (x < 10) { x = 1; } return x; }' \
    "IF"
run_test "3.2" "Greater Than" \
    'int main() { int x; x = 15; if (x > 10) { x = 0; } return x; }' \
    "IF"
run_test "3.3" "Equal To" \
    'int main() { int x; x = 5; if (x == 5) { x = 100; } return x; }' \
    "BINOP"
run_test "3.4" "Not Equal" \
    'int main() { int x; x = 5; if (x != 10) { x = 1; } return x; }' \
    "BINOP"

echo
echo "--- LOGICAL OPERATOR TESTS ---"
run_test "4.1" "Logical AND" \
    'int main() { int a; int b; a = 5; b = 10; if (a < 10 && b > 5) { a = 1; } return a; }' \
    "if_false"
run_test "4.2" "Logical OR" \
    'int main() { int x; x = 5; if (x < 3 || x > 10) { x = 0; } return x; }' \
    "BINOP"
run_test "4.3" "Logical NOT" \
    'int main() { int x; x = 0; if (!x) { x = 1; } return x; }' \
    "UNOP"

echo
echo "--- CONTROL FLOW TESTS ---"
run_test "5.1" "If Statement" \
    'int main() { int x; x = 5; if (x < 10) { x = 20; } return x; }' \
    "IF"
run_test "5.2" "If-Else Statement" \
    'int main() { int x; x = 15; if (x < 10) { x = 0; } else { x = 1; } return x; }' \
    "ELSE"
run_test "5.3" "Nested If" \
    'int main() { int x; x = 5; if (x > 0) { if (x < 10) { x = 100; } } return x; }' \
    "IF"

echo
echo "--- WHILE LOOP TESTS ---"
run_test "6.1" "Simple While Loop" \
    'int main() { int i; i = 0; while (i < 5) { i = i + 1; } return i; }' \
    "WHILE"
run_test "6.2" "While Loop with Accumulator" \
    'int main() { int i; int sum; i = 0; sum = 0; while (i < 5) { sum = sum + i; i = i + 1; } return sum; }' \
    "WHILE"
run_test "6.3" "While Loop - Multiplication" \
    'int main() { int x; x = 2; while (x < 10) { x = x * 2; } return x; }' \
    "goto L"
run_test "6.4" "Nested While Loops" \
    'int main() { int i; int j; int count; count = 0; i = 0; while (i < 3) { j = 0; while (j < 3) { count = count + 1; j = j + 1; } i = i + 1; } return count; }' \
    "WHILE"

echo
echo "--- FOR LOOP TESTS ---"
run_test "7.1" "Simple For Loop" \
    'int main() { int i; int sum; sum = 0; for (i = 0; i < 5; i = i + 1) { sum = sum + i; } return sum; }' \
    "FOR"
run_test "7.2" "For Loop - Factorial" \
    'int main() { int i; int result; result = 1; for (i = 1; i < 6; i = i + 1) { result = result * i; } return result; }' \
    "FOR"
run_test "7.3" "For Loop with Complex Update" \
    'int main() { int i; for (i = 10; i > 0; i = i - 2) { i = i - 1; } return i; }' \
    "FOR"

echo
echo "--- FUNCTION TESTS ---"
run_test "8.1" "Function with Return" \
    'int add(int a, int b) { return a + b; } int main() { int x; x = add(5, 10); return x; }' \
    "FUNC_DECL"
run_test "8.2" "Function with Local Variables" \
    'int multiply(int a, int b) { int result; result = a * b; return result; } int main() { return multiply(3, 4); }' \
    "FUNC_DECL"

echo
echo "--- ARRAY TESTS ---"
run_test "9.1" "Array Declaration" \
    'int main() { int arr[5]; return 0; }' \
    "ARRAY_DECL"
run_test "9.2" "2D Array Declaration" \
    'int main() { int arr[5][10]; return 0; }' \
    "ARRAY_DECL: arr[5][10]"
run_test "9.3" "Array Access (1D)" \
    'int main() { int arr[5]; arr[2] = 100; return arr[2]; }' \
    "arr[2] = 100"
run_test "9.4" "Array Access (2D)" \
    'int main() { int arr[5][10]; arr[1][2] = 50; return arr[1][2]; }' \
    "arr[t2] = 50"

echo
echo "--- FUNCTION CALL IR TESTS ---"
run_test "8.3" "Function Call with Params" \
    'int add(int a, int b) { return a + b; } int main() { return add(5, 10); }' \
    "param 5"
run_test "8.4" "Nested Function Calls" \
    'int f(int x) { return x; } int main() { return f(f(1)); }' \
    "call f, 1"

echo
echo "--- OPTIMIZATION TESTS ---"
run_test "10.1" "Constant Folding with Precedence" \
    'int main() { int x; x = 5 + 3 * 2 - 1; return x; }' \
    "x = 10"
run_test "10.2" "Unary Operator Folding" \
    'int main() { int x; x = !0; return x; }' \
    "Folded unary"
run_test "10.3" "Dead Code Detection" \
    'int main() { int x; x = 5; return x; }' \
    "ASSIGN"

echo
echo "--- ERROR DETECTION TESTS ---"
run_test "11.1" "Undeclared Variable" \
    'int main() { x = 5; return x; }' \
    "Semantic Error"
run_test "11.2" "Variable Redeclaration" \
    'int main() { int x; int x; return 0; }' \
    "Semantic Error"
run_test "11.3" "Undefined Function" \
    'int main() { return foo(5); }' \
    "Undefined function 'foo'"
run_test "11.4" "Function Arity Mismatch" \
    'int add(int a, int b) { return a + b; } int main() { return add(1); }' \
    "Function 'add' expects 2 arguments but got 1"
run_test "11.5" "Array Bounds Checking" \
    'int main() { int arr[2]; return arr[5]; }' \
    "Array 'arr' index 5 out of bounds"

echo
echo "--- TAC CORRECTNESS TESTS ---"
run_test_not_regex "12.1" "No Duplicate Return After Explicit Return" \
    'int main() { return 1; }' \
    'return 1[[:space:]]*return'

echo
echo "--- TARGET CODE GENERATION TESTS ---"
run_test "13.1" "Stack VM Code For Folded Assignment" \
    'int main() { int x; x = 5 + 3 * 2 - 1; return x; }' \
    "PUSHI 10"
run_test "13.2" "Stack VM Function Call" \
    'int add(int a, int b) { return a + b; } int main() { return add(5, 10); }' \
    "CALL add 2"
run_test_regex "13.3" "Correct 2D Array Width In Backends" \
    'int main() { int arr[2][3]; arr[1][2] = 7; return arr[1][2]; }' \
    't1 = 1 \* 3|PUSHI 3'

echo
echo "========== TEST SUMMARY =========="
echo "Total Tests: $TOTAL_TESTS"
echo "Passed: $PASS_COUNT"
echo "Failed: $FAIL_COUNT"
echo

if [ "$FAIL_COUNT" -eq 0 ]; then
    echo "ALL TESTS PASSED!"
    exit 0
else
    echo "SOME TESTS FAILED"
    exit 1
fi
