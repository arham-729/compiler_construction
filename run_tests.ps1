# C Compiler - Automated Test Runner (Windows PowerShell)
# This script runs all test cases and generates a report

$COMPILER = ".\compiler.exe"
$PASS_COUNT = 0
$FAIL_COUNT = 0
$TOTAL_TESTS = 0

# Check if compiler exists
if (!(Test-Path $COMPILER)) {
    Write-Host "Error: Compiler not found at $COMPILER" -ForegroundColor Red
    Write-Host "Please compile the compiler first:"
    Write-Host "  bison -d parser.y && flex lexer.l && gcc lex.yy.c parser.tab.c ast.c symtab.c semantic.c optimize.c ir.c main.c -o compiler.exe"
    exit 1
}

Write-Host "=== C Compiler Test Suite ===" -ForegroundColor Blue
Write-Host ""
Write-Host "Running automated tests..." 
Write-Host ""

# Test function
function Run-Test {
    param(
        [string]$testNum,
        [string]$testName,
        [string]$testCode,
        [string]$expectedKey
    )
    
    [int]$script:TOTAL_TESTS++
    
    # Run the test
    $output = $testCode | & $COMPILER 2>&1 | Out-String
    
    # Check if expected output is present
    if ($output -match [regex]::Escape($expectedKey)) {
        Write-Host "PASS - Test $testNum`: $testName" -ForegroundColor Green
        [int]$script:PASS_COUNT++
    }
    else {
        Write-Host "FAIL - Test $testNum`: $testName" -ForegroundColor Red
        [int]$script:FAIL_COUNT++
    }
}

# BASIC TESTS
Write-Host ""
Write-Host "--- BASIC TESTS ---" -ForegroundColor Blue
Run-Test "1.1" "Variable Declaration" `
    'int main() { int x; x = 5; return x; }' `
    "VAR_DECL"

Run-Test "1.2" "Multiple Variables" `
    'int main() { int x; int y; x = 10; y = 20; return x; }' `
    "SEQ"

Run-Test "1.3" "Arithmetic Operations" `
    'int main() { int result; result = 10 + 5; return result; }' `
    "Folded constants"

# ARITHMETIC TESTS
Write-Host ""
Write-Host "--- ARITHMETIC EXPRESSION TESTS ---" -ForegroundColor Blue
Run-Test "2.1" "Addition" `
    'int main() { int c; int a; int b; a = 10; b = 20; c = a + b; return c; }' `
    "ASSIGN"

Run-Test "2.2" "Subtraction" `
    'int main() { int x; x = 50 - 25; return x; }' `
    "Folded constants"

Run-Test "2.3" "Multiplication" `
    'int main() { int x; int y; x = 7; y = x * 3; return y; }' `
    "BINOP"

Run-Test "2.4" "Division" `
    'int main() { int x; x = 100 / 5; return x; }' `
    "Folded constants"

# RELATIONAL TESTS
Write-Host ""
Write-Host "--- RELATIONAL OPERATOR TESTS ---" -ForegroundColor Blue
Run-Test "3.1" "Less Than" `
    'int main() { int x; x = 5; if (x < 10) { x = 1; } return x; }' `
    "IF"

Run-Test "3.2" "Greater Than" `
    'int main() { int x; x = 15; if (x > 10) { x = 0; } return x; }' `
    "IF"

Run-Test "3.3" "Equal To" `
    'int main() { int x; x = 5; if (x == 5) { x = 100; } return x; }' `
    "BINOP"

Run-Test "3.4" "Not Equal" `
    'int main() { int x; x = 5; if (x != 10) { x = 1; } return x; }' `
    "BINOP"

# LOGICAL TESTS
Write-Host ""
Write-Host "--- LOGICAL OPERATOR TESTS ---" -ForegroundColor Blue
Run-Test "4.1" "Logical AND" `
    'int main() { int a; int b; a = 5; b = 10; if (a < 10 && b > 5) { a = 1; } return a; }' `
    "if_false"

Run-Test "4.2" "Logical OR" `
    'int main() { int x; x = 5; if (x < 3 || x > 10) { x = 0; } return x; }' `
    "BINOP"

Run-Test "4.3" "Logical NOT" `
    'int main() { int x; x = 0; if (!x) { x = 1; } return x; }' `
    "UNOP"

# CONTROL FLOW TESTS
Write-Host ""
Write-Host "--- CONTROL FLOW TESTS ---" -ForegroundColor Blue
Run-Test "5.1" "If Statement" `
    'int main() { int x; x = 5; if (x < 10) { x = 20; } return x; }' `
    "IF"

Run-Test "5.2" "If-Else Statement" `
    'int main() { int x; x = 15; if (x < 10) { x = 0; } else { x = 1; } return x; }' `
    "ELSE"

Run-Test "5.3" "Nested If" `
    'int main() { int x; x = 5; if (x > 0) { if (x < 10) { x = 100; } } return x; }' `
    "IF"

# WHILE LOOP TESTS
Write-Host ""
Write-Host "--- WHILE LOOP TESTS ---" -ForegroundColor Blue
Run-Test "6.1" "Simple While Loop" `
    'int main() { int i; i = 0; while (i < 5) { i = i + 1; } return i; }' `
    "WHILE"

Run-Test "6.2" "While Loop with Accumulator" `
    'int main() { int i; int sum; i = 0; sum = 0; while (i < 5) { sum = sum + i; i = i + 1; } return sum; }' `
    "WHILE"

Run-Test "6.3" "While Loop - Multiplication" `
    'int main() { int x; x = 2; while (x < 10) { x = x * 2; } return x; }' `
    "goto L"

Run-Test "6.4" "Nested While Loops" `
    'int main() { int i; int j; int count; count = 0; i = 0; while (i < 3) { j = 0; while (j < 3) { count = count + 1; j = j + 1; } i = i + 1; } return count; }' `
    "WHILE"

# FOR LOOP TESTS
Write-Host ""
Write-Host "--- FOR LOOP TESTS ---" -ForegroundColor Blue
Run-Test "7.1" "Simple For Loop" `
    'int main() { int i; int sum; sum = 0; for (i = 0; i < 5; i = i + 1) { sum = sum + i; } return sum; }' `
    "FOR"

Run-Test "7.2" "For Loop - Factorial" `
    'int main() { int i; int result; result = 1; for (i = 1; i < 6; i = i + 1) { result = result * i; } return result; }' `
    "FOR"

Run-Test "7.3" "For Loop with Complex Update" `
    'int main() { int i; for (i = 10; i > 0; i = i - 2) { i = i - 1; } return i; }' `
    "FOR"

# FUNCTION TESTS
Write-Host ""
Write-Host "--- FUNCTION TESTS ---" -ForegroundColor Blue
Run-Test "8.1" "Function with Return" `
    'int add(int a, int b) { return a + b; } int main() { int x; x = add(5, 10); return x; }' `
    "FUNC_DECL"

Run-Test "8.2" "Function with Local Variables" `
    'int multiply(int a, int b) { int result; result = a * b; return result; } int main() { return multiply(3, 4); }' `
    "FUNC_DECL"

# ARRAY TESTS
Write-Host ""
Write-Host "--- ARRAY TESTS ---" -ForegroundColor Blue
Run-Test "9.1" "Array Declaration" `
    'int main() { int arr[5]; return 0; }' `
    "ARRAY_DECL"

Run-Test "9.2" "2D Array Declaration" `
    'int main() { int arr[5][10]; return 0; }' `
    "ARRAY_DECL: arr[5][10]"

Run-Test "9.3" "Array Access (1D)" `
    'int main() { int arr[5]; arr[2] = 100; return arr[2]; }' `
    "arr[2] = 100"

Run-Test "9.4" "Array Access (2D)" `
    'int main() { int arr[5][10]; arr[1][2] = 50; return arr[1][2]; }' `
    "arr[t2] = 50"

# FUNCTION CALL IR TESTS
Write-Host ""
Write-Host "--- FUNCTION CALL IR TESTS ---" -ForegroundColor Blue
Run-Test "8.3" "Function Call with Params" `
    'int add(int a, int b) { return a + b; } int main() { return add(5, 10); }' `
    "param 5"

Run-Test "8.4" "Nested Function Calls" `
    'int f(int x) { return x; } int main() { return f(f(1)); }' `
    "call f, 1"

# OPTIMIZATION TESTS
Write-Host ""
Write-Host "--- OPTIMIZATION TESTS ---" -ForegroundColor Blue
Run-Test "10.1" "Constant Folding" `
    'int main() { int x; x = 5 + 3 * 2 - 1; return x; }' `
    "Folded constants"

Run-Test "10.2" "Unary Operator Folding" `
    'int main() { int x; x = !0; return x; }' `
    "Folded unary"

Run-Test "10.3" "Dead Code Detection" `
    'int main() { int x; x = 5; return x; }' `
    "ASSIGN"

# ERROR DETECTION TESTS
Write-Host ""
Write-Host "--- ERROR DETECTION TESTS ---" -ForegroundColor Blue
Run-Test "11.1" "Undeclared Variable" `
    'int main() { x = 5; return x; }' `
    "Semantic Error"

Run-Test "11.2" "Variable Redeclaration" `
    'int main() { int x; int x; return 0; }' `
    "Semantic Error"

Write-Host ""
Write-Host "========== TEST SUMMARY ==========" -ForegroundColor Blue
Write-Host "Total Tests: $script:TOTAL_TESTS"
Write-Host "Passed: $script:PASS_COUNT" -ForegroundColor Green
Write-Host "Failed: $script:FAIL_COUNT" -ForegroundColor Red
Write-Host ""

if ($script:FAIL_COUNT -eq 0) {
    Write-Host "ALL TESTS PASSED!" -ForegroundColor Green
    exit 0
} else {
    Write-Host "SOME TESTS FAILED" -ForegroundColor Red
    exit 1
}
