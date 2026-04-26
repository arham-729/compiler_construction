#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "ir.h"
#include "symtab.h"

int temp_count = 1;
int label_count = 0;

// Generates temporary variable names like t1, t2, t3
int new_temp() {
    return temp_count++;
}

// Generates unique labels for control flow
int new_label() {
    return label_count++;
}

// Forward declaration
char* generate_tac_expr(ASTNode *node);

// Helper to generate param instructions for function calls
void generate_tac_args(ASTNode *node, int *count) {
    if (!node) return;
    if (node->type == NODE_ARG_LIST) {
        generate_tac_args(node->left, count);
        if (node->right) generate_tac_args(node->right, count);
    } else {
        char *val = generate_tac_expr(node);
        printf("param %s\n", val);
        (*count)++;
    }
}

// Generates TAC for expressions and returns the variable/temp holding the result
char* generate_tac_expr(ASTNode *node) {
    if (!node) {
        char *buf = malloc(4);
        strcpy(buf, "0");
        return buf;
    }
    
    if (node->type == NODE_NUM) {
        char *buf = malloc(16);
        sprintf(buf, "%d", node->value);
        return buf;
    } 
    else if (node->type == NODE_ID) {
        char *buf = malloc(strlen(node->name) + 1);
        strcpy(buf, node->name);
        return buf;
    }
    else if (node->type == NODE_ARRAY_ACCESS) {
        char *idx1 = generate_tac_expr(node->left);
        if (node->right) { // 2D Access
            char *idx2 = generate_tac_expr(node->right);
            Symbol *sym = lookup_symbol(node->name);
            int width = (sym && sym->size2 > 0) ? sym->size2 : 1;
            
            int t1 = new_temp();
            printf("t%d = %s * %d\n", t1, idx1, width);
            int t2 = new_temp();
            printf("t%d = t%d + %s\n", t2, t1, idx2);
            
            int t3 = new_temp();
            printf("t%d = %s[t%d]\n", t3, node->name, t2);
            char *buf = malloc(16);
            sprintf(buf, "t%d", t3);
            return buf;
        } else { // 1D Access
            int t = new_temp();
            printf("t%d = %s[%s]\n", t, node->name, idx1);
            char *buf = malloc(16);
            sprintf(buf, "t%d", t);
            return buf;
        }
    }
    else if (node->type == NODE_FUNC_CALL) {
        int arg_count = 0;
        generate_tac_args(node->left, &arg_count);
        int t = new_temp();
        printf("t%d = call %s, %d\n", t, node->name, arg_count);
        char *buf = malloc(16);
        sprintf(buf, "t%d", t);
        return buf;
    }
    else if (node->type == NODE_BINOP) {
        char *left_val = generate_tac_expr(node->left);
        char *right_val = generate_tac_expr(node->right);
        
        int t = new_temp();
        printf("t%d = %s %s %s\n", t, left_val, node->op, right_val);
        
        char *buf = malloc(16);
        sprintf(buf, "t%d", t);
        return buf;
    }
    else if (node->type == NODE_UNOP) {
        char *val = generate_tac_expr(node->left);
        int t = new_temp();
        printf("t%d = %s%s\n", t, node->op, val);
        char *buf = malloc(16);
        sprintf(buf, "t%d", t);
        return buf;
    }
    
    char *buf = malloc(4);
    strcpy(buf, "0");
    return buf;
}

void generate_tac(ASTNode *node) {
    if (!node) return;

    if (node->type == NODE_SEQ) {
        generate_tac(node->left);
        generate_tac(node->right);
    }
    else if (node->type == NODE_ASSIGN) {
        char *expr_val = generate_tac_expr(node->left);
        printf("%s = %s\n", node->name, expr_val);
    }
    else if (node->type == NODE_ARRAY_ASSIGN) {
        char *idx1 = generate_tac_expr(node->right);
        if (node->cond) { // 2D Assign
            char *idx2 = generate_tac_expr(node->cond);
            char *expr_val = generate_tac_expr(node->left);
            Symbol *sym = lookup_symbol(node->name);
            int width = (sym && sym->size2 > 0) ? sym->size2 : 1;
            
            int t1 = new_temp();
            printf("t%d = %s * %d\n", t1, idx1, width);
            int t2 = new_temp();
            printf("t%d = t%d + %s\n", t2, t1, idx2);
            
            printf("%s[t%d] = %s\n", node->name, t2, expr_val);
        } else { // 1D Assign
            char *expr_val = generate_tac_expr(node->left);
            printf("%s[%s] = %s\n", node->name, idx1, expr_val);
        }
    }
    else if (node->type == NODE_VAR_DECL || node->type == NODE_ARRAY_DECL) {
        // Variable declarations don't generate TAC
    }
    else if (node->type == NODE_IF) {
        int else_label = new_label();
        int end_label = new_label();
        
        char *cond = generate_tac_expr(node->cond);
        printf("if_false %s goto L%d\n", cond, else_label);
        
        generate_tac(node->left);
        printf("goto L%d\n", end_label);
        
        printf("L%d:\n", else_label);
        if (node->right) {
            generate_tac(node->right);
        }
        printf("L%d:\n", end_label);
    }
    else if (node->type == NODE_WHILE) {
        int loop_label = new_label();
        int end_label = new_label();
        
        printf("L%d:\n", loop_label);
        char *cond = generate_tac_expr(node->cond);
        printf("if_false %s goto L%d\n", cond, end_label);
        
        generate_tac(node->body);
        printf("goto L%d\n", loop_label);
        printf("L%d:\n", end_label);
    }
    else if (node->type == NODE_FOR) {
        int loop_label = new_label();
        int end_label = new_label();
        
        // Initialization
        generate_tac(node->init);
        
        // Loop condition
        printf("L%d:\n", loop_label);
        char *cond = generate_tac_expr(node->cond);
        printf("if_false %s goto L%d\n", cond, end_label);
        
        // Loop body
        generate_tac(node->body);
        
        // Update
        generate_tac(node->update);
        printf("goto L%d\n", loop_label);
        printf("L%d:\n", end_label);
    }
    else if (node->type == NODE_FUNC_DECL) {
        printf("\nFunc %s:\n", node->name);
        generate_tac(node->body);
        printf("return\n");
    }
    else if (node->type == NODE_RETURN) {
        if (node->left) {
            char *val = generate_tac_expr(node->left);
            printf("return %s\n", val);
        } else {
            printf("return\n");
        }
    }
    else if (node->type == NODE_FUNC_CALL) {
        generate_tac_expr(node);
    }
}