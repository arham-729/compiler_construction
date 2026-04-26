#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symtab.h"

void analyze_semantics(ASTNode *node) {
    if (!node) return;

    // Pre-order for declarations, post-order for usage
    if (node->type == NODE_VAR_DECL) {
        if (!insert_symbol(node->name, node->op)) {
            fprintf(stderr, "Semantic Error: Redeclaration of variable '%s'\n", node->name);
        }
    }
    else if (node->type == NODE_ARRAY_DECL) {
        char array_type[50];
        if (node->array_size2 > 0) {
            sprintf(array_type, "int[%d][%d]", node->array_size, node->array_size2);
            if (!insert_symbol_2d(node->name, array_type, node->array_size2)) {
                fprintf(stderr, "Semantic Error: Redeclaration of array '%s'\n", node->name);
            }
        } else {
            sprintf(array_type, "int[%d]", node->array_size);
            if (!insert_symbol(node->name, array_type)) {
                fprintf(stderr, "Semantic Error: Redeclaration of array '%s'\n", node->name);
            }
        }
    }
    else if (node->type == NODE_FUNC_DECL) {
        if (!insert_symbol(node->name, node->op)) {
            fprintf(stderr, "Semantic Error: Redeclaration of function '%s'\n", node->name);
        }
        enter_scope();
        // Add parameters to local scope
        ASTNode *param = node->next;
        while (param) {
            if (param->type == NODE_VAR_DECL) {
                insert_symbol(param->name, param->op);
            }
            param = param->next;
        }
        analyze_semantics(node->body);
        exit_scope();
    }
    else if (node->type == NODE_FUNC_CALL) {
        if (!lookup_symbol(node->name)) {
            fprintf(stderr, "Semantic Error: Undefined function '%s'\n", node->name);
        }
        analyze_semantics(node->left); // Analyze arguments
    }
    else if (node->type == NODE_ID) {
        if (!lookup_symbol(node->name)) {
            fprintf(stderr, "Semantic Error: Undeclared variable '%s'\n", node->name);
        }
    }
    else if (node->type == NODE_ARRAY_ACCESS) {
        if (!lookup_symbol(node->name)) {
            fprintf(stderr, "Semantic Error: Undeclared array '%s'\n", node->name);
        }
        analyze_semantics(node->left);  // Index 1
        analyze_semantics(node->right); // Index 2
    }
    else if (node->type == NODE_ASSIGN) {
        if (!lookup_symbol(node->name)) {
            fprintf(stderr, "Semantic Error: Assignment to undeclared variable '%s'\n", node->name);
        }
        analyze_semantics(node->left);
    }
    else if (node->type == NODE_ARRAY_ASSIGN) {
        if (!lookup_symbol(node->name)) {
            fprintf(stderr, "Semantic Error: Assignment to undeclared array '%s'\n", node->name);
        }
        analyze_semantics(node->right);  // Index 1
        analyze_semantics(node->cond);   // Index 2 (for 2D)
        analyze_semantics(node->left);   // Value
    }
    else if (node->type == NODE_IF) {
        analyze_semantics(node->cond);
        analyze_semantics(node->left);
        analyze_semantics(node->right);
    }
    else if (node->type == NODE_WHILE || node->type == NODE_FOR) {
        analyze_semantics(node->cond);
        analyze_semantics(node->body);
        if (node->type == NODE_FOR) {
            analyze_semantics(node->init);
            analyze_semantics(node->update);
        }
    }
    else if (node->type == NODE_RETURN) {
        analyze_semantics(node->left);
    }
    else if (node->type == NODE_SEQ || node->type == NODE_ARG_LIST) {
        analyze_semantics(node->left);
        analyze_semantics(node->right);
    }
    else {
        analyze_semantics(node->left);
        analyze_semantics(node->right);
    }
}