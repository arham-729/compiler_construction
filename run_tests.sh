#!/bin/bash

# C Compiler - Automated Test Runner (Linux/macOS)
# This script runs all test cases and generates a report

COMPILER="./compiler.exe"
PASS_COUNT=0
FAIL_COUNT=0
TOTAL_TESTS=0

# Color codes for output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Check if compiler exists
if [ ! -f "$COMPILER" ]; then
    echo -e "${RED}Error: Compiler not found at $COMPILER${NC}"
    echo "Please compile the compiler first:"
    echo "  bison -d parser.y && flex lexer.l && gcc lex.yy.c parser.tab.c ast.c symtab.c semantic.c optimize.c ir.c main.c -o compiler.exe"
    exit 1
fi

echo -e "${BLUE}=== C Compiler Test Suite ===${NC}\n"
echo "Running automated tests..."
echo ""

# Test function
run_test() {
    local test_num=$1
    local test_name=$2
    local test_code=$3
    local expected_key=$4
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Run the test
    output=$(echo "$test_code" | $COMPILER 2>&1)
    
    # Check if expected output is present
    if echo "$output" | grep -q "$expected_key"; then
        echo -e "${GREEN}✓ PASS${NC} - Test $test_num: $test_name"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo -e "${RED}✗ FAIL${NC} - Test $test_num: $test_name"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
}

# BASIC TESTS
echo -e "${BLUE}--- BASIC TESTS ---${NC}"
run_test "1.1" "Variable Declaration" \
    'int main() { int x; x = 5; return x; }' \
    "VAR_DECL"

run_test "1.2" "Multiple Variables" \
    'int main() { int x; int y; x = 10; y = 20; return x; }' \
    "SEQ"

run_test "1.3" "Arithmetic Operations" \
    'int main() { int result; result = 10 + 5; return result; }' \
    "BINOP"

# ARITHMETIC TESTS
echo -e "\n${BLUE}--- ARITHMETIC EXPRESSION TESTS ---${NC}"
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

# RELATIONAL TESTS
echo -e "\n${BLUE}--- RELATIONAL OPERATOR TESTS ---${NC}"
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

# LOGICAL TESTS
echo -e "\n${BLUE}--- LOGICAL OPERATOR TESTS ---${NC}"
run_test "4.1" "Logical AND" \
    'int main() { int a; int b; a = 5; b = 10; if (a < 10 && b > 5) { a = 1; } return a; }' \
    "if_false"

run_test "4.2" "Logical OR" \
    'int main() { int x; x = 5; if (x < 3 || x > 10) { x = 0; } return x; }' \
    "BINOP"

run_test "4.3" "Logical NOT" \
    'int main() { int x; x = 0; if (!x) { x = 1; } return x; }' \
    "UNOP"

# CONTROL FLOW TESTS
echo -e "\n${BLUE}--- CONTROL FLOW TESTS ---${NC}"
run_test "5.1" "If Statement" \
    'int main() { int x; x = 5; if (x < 10) { x = 20; } return x; }' \
    "IF"

run_test "5.2" "If-Else Statement" \
    'int main() { int x; x = 15; if (x < 10) { x = 0; } else { x = 1; } return x; }' \
    "ELSE"

run_test "5.3" "Nested If" \
    'int main() { int x; x = 5; if (x > 0) { if (x < 10) { x = 100; } } return x; }' \
    "IF"

# WHILE LOOP TESTS
echo -e "\n${BLUE}--- WHILE LOOP TESTS ---${NC}"
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

# FOR LOOP TESTS
echo -e "\n${BLUE}--- FOR LOOP TESTS ---${NC}"
run_test "7.1" "Simple For Loop" \
    'int main() { int i; int sum; sum = 0; for (i = 0; i < 5; i = i + 1) { sum = sum + i; } return sum; }' \
    "FOR"

run_test "7.2" "For Loop - Factorial" \
    'int main() { int i; int result; result = 1; for (i = 1; i < 6; i = i + 1) { result = result * i; } return result; }' \
    "FOR"

run_test "7.3" "For Loop with Complex Update" \
    'int main() { int i; for (i = 10; i > 0; i = i - 2) { i = i - 1; } return i; }' \
    "FOR"

# OPTIMIZATION TESTS
echo -e "\n${BLUE}--- OPTIMIZATION TESTS ---${NC}"
run_test "10.1" "Constant Folding" \
    'int main() { int x; x = 5 + 3 * 2 - 1; return x; }' \
    "Folded constants"

run_test "10.2" "Unary Operator Folding" \
    'int main() { int x; x = !0; return x; }' \
    "UNOP"

run_test "10.3" "Dead Code Detection" \
    'int main() { int x; x = 5; 5 + 3; return x; }' \
    "ASSIGN"

# ERROR DETECTION TESTS
echo -e "\n${BLUE}--- ERROR DETECTION TESTS ---${NC}"
run_test "11.1" "Undeclared Variable" \
    'int main() { x = 5; return x; }' \
    "Semantic Error"

run_test "11.2" "Variable Redeclaration" \
    'int main() { int x; int x; return 0; }' \
    "Semantic Error"

# ARRAY TESTS
echo -e "\n${BLUE}--- ARRAY TESTS ---${NC}"
run_test "9.1" "Array Declaration" \
    'int main() { int arr[5]; arr[0] = 10; arr[1] = 20; return arr[0]; }' \
    "ARRAY"

run_test "9.2" "Array with Loop" \
    'int main() { int arr[5]; int i; i = 0; while (i < 5) { arr[i] = i * 2; i = i + 1; } return arr[4]; }' \
    "ARRAY"

# FUNCTION TESTS
echo -e "\n${BLUE}--- FUNCTION TESTS ---${NC}"
run_test "8.1" "Function with Return" \
    'int add(int a, int b) { return a + b; } int main() { int x; x = add(5, 10); return x; }' \
    "FUNC_DECL"

run_test "8.2" "Function with Local Variables" \
    'int multiply(int a, int b) { int result; result = a * b; return result; } int main() { return multiply(3, 4); }' \
    "FUNC_DECL"

# SUMMARY
echo ""
echo -e "${BLUE}========== TEST SUMMARY ==========${NC}"
echo "Total Tests: $TOTAL_TESTS"
echo -e "${GREEN}Passed: $PASS_COUNT${NC}"
echo -e "${RED}Failed: $FAIL_COUNT${NC}"
echo ""

if [ $FAIL_COUNT -eq 0 ]; then
    echo -e "${GREEN}✓ ALL TESTS PASSED!${NC}"
    exit 0
else
    echo -e "${RED}✗ SOME TESTS FAILED${NC}"
    exit 1
fi
