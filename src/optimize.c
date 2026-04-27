#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "optimize.h"

// Helper function to check if a node is dead code (e.g., unreachable)
static int is_dead_code(ASTNode *node) {
    // A node is dead if it has no side effects and is not a declaration/function
    if (!node) return 0;
    
    if (node->type == NODE_VAR_DECL || node->type == NODE_ARRAY_DECL ||
        node->type == NODE_FUNC_DECL || node->type == NODE_RETURN) {
        return 0; // These have side effects
    }
    
    if (node->type == NODE_ASSIGN) {
        return 0; // Assignments have side effects
    }
    
    // Leaf nodes with no side effects
    if (node->type == NODE_NUM || node->type == NODE_ID) {
        return 1;
    }
    
    return 0;
}

static ASTNode* optimize_node(ASTNode *node) {
    if (!node) return NULL;

    if (node->type == NODE_IF) {
        node->cond = optimize_node(node->cond);
        node->left = optimize_node(node->left);
        node->right = optimize_node(node->right);
    }
    else if (node->type == NODE_WHILE || node->type == NODE_FOR) {
        if (node->type == NODE_FOR) {
            node->init = optimize_node(node->init);
        }
        node->cond = optimize_node(node->cond);
        node->body = optimize_node(node->body);
        if (node->type == NODE_FOR) {
            node->update = optimize_node(node->update);
        }
    }
    else if (node->type == NODE_FUNC_DECL) {
        node->body = optimize_node(node->body);
    }
    else if (node->type == NODE_ARRAY_ASSIGN) {
        node->right = optimize_node(node->right);
        node->cond = optimize_node(node->cond);
        node->left = optimize_node(node->left);
    }
    else if (node->type == NODE_ARRAY_ACCESS) {
        node->left = optimize_node(node->left);
        node->right = optimize_node(node->right);
    }
    else {
        node->left = optimize_node(node->left);
        node->right = optimize_node(node->right);
    }

    if (node->type == NODE_BINOP) {
        if (node->left && node->left->type == NODE_NUM &&
            node->right && node->right->type == NODE_NUM) {
            int val1 = node->left->value;
            int val2 = node->right->value;
            int result = 0;
            int valid = 1;

            if (strcmp(node->op, "+") == 0) result = val1 + val2;
            else if (strcmp(node->op, "-") == 0) result = val1 - val2;
            else if (strcmp(node->op, "*") == 0) result = val1 * val2;
            else if (strcmp(node->op, "/") == 0) {
                if (val2 != 0) result = val1 / val2;
                else valid = 0;
            }
            else if (strcmp(node->op, "<") == 0) result = (val1 < val2) ? 1 : 0;
            else if (strcmp(node->op, ">") == 0) result = (val1 > val2) ? 1 : 0;
            else if (strcmp(node->op, "==") == 0) result = (val1 == val2) ? 1 : 0;
            else if (strcmp(node->op, "!=") == 0) result = (val1 != val2) ? 1 : 0;
            else if (strcmp(node->op, "<=") == 0) result = (val1 <= val2) ? 1 : 0;
            else if (strcmp(node->op, ">=") == 0) result = (val1 >= val2) ? 1 : 0;
            else if (strcmp(node->op, "&&") == 0) result = (val1 && val2) ? 1 : 0;
            else if (strcmp(node->op, "||") == 0) result = (val1 || val2) ? 1 : 0;
            else valid = 0;

            if (valid) {
                printf("[Optimizer] Folded constants: %d %s %d -> %d\n", val1, node->op, val2, result);
                return create_num_node(result);
            }
        }
    }
    else if (node->type == NODE_UNOP) {
        if (node->left && node->left->type == NODE_NUM) {
            int val = node->left->value;
            int result;
            
            if (strcmp(node->op, "!") == 0) {
                result = !val;
                printf("[Optimizer] Folded unary: %s%d -> %d\n", node->op, val, result);
                return create_num_node(result);
            }
        }
    }
    else if (node->type == NODE_SEQ) {
        if (is_dead_code(node->left) && node->left->type != NODE_VAR_DECL && node->left->type != NODE_ARRAY_DECL) {
            printf("[Optimizer] Removed dead code from SEQ\n");
            return node->right;
        }
    }

    return node;
}

void optimize_ast(ASTNode **node_ptr) {
    if (!node_ptr || !(*node_ptr)) return;
    *node_ptr = optimize_node(*node_ptr);
}
