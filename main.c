#include <stdio.h>
#include "ast.h"
#include "symtab.h"
#include "semantic.h"
#include "optimize.h"
#include "ir.h"

extern int yyparse();
extern ASTNode *root;

int main() {
    printf("Enter C subset code (e.g., int n; n = 9 - 7;) -> Ctrl+Z to end:\n");
    if (yyparse() == 0) {
        printf("\n--- Starting Semantic Analysis ---\n");
        analyze_semantics(root);
        print_symtab();
        
        printf("\n--- Starting Code Optimization ---\n");
        optimize_ast(&root);
        
        printf("\n--- Optimized Abstract Syntax Tree ---\n");
        print_ast(root, 0); 
        
        printf("\n--- Generating Intermediate Representation (TAC) ---\n");
        generate_tac(root);
        printf("\n");
        
    } else {
        printf("\nParsing Failed.\n");
    }
    return 0;
}