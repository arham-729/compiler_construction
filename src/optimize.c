#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "optimize.h"

// Forward declaration
void optimize_ast(ASTNode **node_ptr);

// Helper function to check if a node is dead code (e.g., unreachable)
int is_dead_code(ASTNode *node) {
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

void optimize_ast(ASTNode **node_ptr) {
    if (!node_ptr || !(*node_ptr)) return;
    ASTNode *node = *node_ptr;

    // Handle different node types appropriately
    if (node->type == NODE_IF) {
        optimize_ast(&(node->cond));
        optimize_ast(&(node->left));
        optimize_ast(&(node->right));
    }
    else if (node->type == NODE_WHILE || node->type == NODE_FOR) {
        optimize_ast(&(node->cond));
        optimize_ast(&(node->body));
        if (node->type == NODE_FOR) {
            optimize_ast(&(node->init));
            optimize_ast(&(node->update));
        }
    }
    else if (node->type == NODE_FUNC_DECL) {
        optimize_ast(&(node->body));
    }
    else if (node->type == NODE_ARRAY_ASSIGN) {
        optimize_ast(&(node->right)); // Index 1
        optimize_ast(&(node->cond));  // Index 2 (for 2D)
        optimize_ast(&(node->left));  // Value
    }
    else if (node->type == NODE_ARRAY_ACCESS) {
        optimize_ast(&(node->left));  // Index 1
        optimize_ast(&(node->right)); // Index 2 (for 2D)
    }
    else {
        // Post-order traversal for other nodes
        optimize_ast(&(node->left));
        optimize_ast(&(node->right));
    }

    // CONSTANT FOLDING LOGIC
    if (node->type == NODE_BINOP) {
        // If both the left and right children are raw numbers
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
                ASTNode *new_node = create_num_node(result);
                *node_ptr = new_node;
            }
        }
    }
    // UNARY OPERATOR OPTIMIZATION
    else if (node->type == NODE_UNOP) {
        if (node->left && node->left->type == NODE_NUM) {
            int val = node->left->value;
            int result;
            
            if (strcmp(node->op, "!") == 0) {
                result = !val;
                printf("[Optimizer] Folded unary: %s%d -> %d\n", node->op, val, result);
                ASTNode *new_node = create_num_node(result);
                *node_ptr = new_node;
            }
        }
    }
    // DEAD CODE ELIMINATION
    else if (node->type == NODE_SEQ) {
        // Remove dead code from sequences (but not from argument lists)
        if (is_dead_code(node->left) && node->left->type != NODE_VAR_DECL && node->left->type != NODE_ARRAY_DECL) {
            printf("[Optimizer] Removed dead code from SEQ\n");
            *node_ptr = node->right;
        }
    }
}