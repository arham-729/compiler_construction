#include <stdio.h>
#include "ast.h"
#include "symtab.h"
#include "semantic.h"
#include "optimize.h"
#include "ir.h"
#include "target.h"

extern int yyparse();
extern ASTNode *root;

int main() {
    printf("Enter C subset code (e.g., int n; n = 9 - 7;) -> Ctrl+Z to end:\n");
    if (yyparse() == 0) {
        printf("\n--- Starting Semantic Analysis ---\n");
        reset_semantic_errors();
        analyze_semantics(root);
        print_symtab();

        if (get_semantic_error_count() > 0) {
            printf("\nSemantic analysis failed with %d error(s). Skipping optimization and TAC generation.\n",
                   get_semantic_error_count());
            return 1;
        }
        
        printf("\n--- Starting Code Optimization ---\n");
        optimize_ast(&root);
        
        printf("\n--- Optimized Abstract Syntax Tree ---\n");
        print_ast(root, 0); 
        
        printf("\n--- Generating Intermediate Representation (TAC) ---\n");
        generate_tac(root);
        printf("\n");

        printf("\n--- Generating Target Code (Stack VM) ---\n");
        generate_target_code(root);
        printf("\n");
        
    } else {
        printf("\nParsing Failed.\n");
    }
    return 0;
}
