#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ast.h"
#include "symtab.h"

static int semantic_error_count = 0;

static void report_semantic_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "Semantic Error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    semantic_error_count++;
}

static int count_function_params(ASTNode *param) {
    int count = 0;
    while (param) {
        count++;
        param = param->next;
    }
    return count;
}

static int count_call_args(ASTNode *args) {
    if (!args) return 0;
    if (args->type == NODE_ARG_LIST) {
        return count_call_args(args->left) + count_call_args(args->right);
    }
    return 1;
}

static void validate_array_index(ASTNode *index, int bound, const char *name, int dimension) {
    if (!index || index->type != NODE_NUM || bound <= 0) return;

    if (index->value < 0 || index->value >= bound) {
        report_semantic_error("Array '%s' index %d out of bounds for dimension %d (size %d)",
                              name, index->value, dimension, bound);
    }
}

void reset_semantic_errors(void) {
    semantic_error_count = 0;
}

int get_semantic_error_count(void) {
    return semantic_error_count;
}

void analyze_semantics(ASTNode *node) {
    if (!node) return;

    // Pre-order for declarations, post-order for usage
    if (node->type == NODE_VAR_DECL) {
        if (!insert_symbol(node->name, node->op)) {
            report_semantic_error("Redeclaration of variable '%s'", node->name);
        }
    }
    else if (node->type == NODE_ARRAY_DECL) {
        char array_type[50];
        if (node->array_size2 > 0) {
            sprintf(array_type, "int[%d][%d]", node->array_size, node->array_size2);
            if (!insert_array_symbol(node->name, array_type, node->array_size, node->array_size2)) {
                report_semantic_error("Redeclaration of array '%s'", node->name);
            }
        } else {
            sprintf(array_type, "int[%d]", node->array_size);
            if (!insert_array_symbol(node->name, array_type, node->array_size, 0)) {
                report_semantic_error("Redeclaration of array '%s'", node->name);
            }
        }
    }
    else if (node->type == NODE_FUNC_DECL) {
        if (!insert_function_symbol(node->name, node->op, count_function_params(node->next))) {
            report_semantic_error("Redeclaration of function '%s'", node->name);
        }
        enter_scope();
        // Add parameters to local scope
        ASTNode *param = node->next;
        while (param) {
            if (param->type == NODE_VAR_DECL) {
                if (!insert_symbol(param->name, param->op)) {
                    report_semantic_error("Redeclaration of parameter '%s'", param->name);
                }
            }
            param = param->next;
        }
        analyze_semantics(node->body);
        exit_scope();
    }
    else if (node->type == NODE_FUNC_CALL) {
        Symbol *sym = lookup_symbol(node->name);
        if (!sym) {
            report_semantic_error("Undefined function '%s'", node->name);
        } else if (!sym->is_function) {
            report_semantic_error("'%s' is not a function", node->name);
        } else {
            int actual_args = count_call_args(node->left);
            if (sym->param_count != actual_args) {
                report_semantic_error("Function '%s' expects %d arguments but got %d",
                                      node->name, sym->param_count, actual_args);
            }
        }
        analyze_semantics(node->left); // Analyze arguments
    }
    else if (node->type == NODE_ID) {
        if (!lookup_symbol(node->name)) {
            report_semantic_error("Undeclared variable '%s'", node->name);
        }
    }
    else if (node->type == NODE_ARRAY_ACCESS) {
        Symbol *sym = lookup_symbol(node->name);
        if (!sym) {
            report_semantic_error("Undeclared array '%s'", node->name);
        } else if (sym->array_size <= 0) {
            report_semantic_error("'%s' is not an array", node->name);
        } else {
            node->array_size = sym->array_size;
            node->array_size2 = sym->size2;
            validate_array_index(node->left, sym->array_size, node->name, 1);
            if (node->right) {
                if (sym->size2 <= 0) {
                    report_semantic_error("Array '%s' does not have a second dimension", node->name);
                } else {
                    validate_array_index(node->right, sym->size2, node->name, 2);
                }
            }
        }
        analyze_semantics(node->left);  // Index 1
        analyze_semantics(node->right); // Index 2
    }
    else if (node->type == NODE_ASSIGN) {
        if (!lookup_symbol(node->name)) {
            report_semantic_error("Assignment to undeclared variable '%s'", node->name);
        }
        analyze_semantics(node->left);
    }
    else if (node->type == NODE_ARRAY_ASSIGN) {
        Symbol *sym = lookup_symbol(node->name);
        if (!sym) {
            report_semantic_error("Assignment to undeclared array '%s'", node->name);
        } else if (sym->array_size <= 0) {
            report_semantic_error("'%s' is not an array", node->name);
        } else {
            node->array_size = sym->array_size;
            node->array_size2 = sym->size2;
            validate_array_index(node->right, sym->array_size, node->name, 1);
            if (node->cond) {
                if (sym->size2 <= 0) {
                    report_semantic_error("Array '%s' does not have a second dimension", node->name);
                } else {
                    validate_array_index(node->cond, sym->size2, node->name, 2);
                }
            }
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
        if (node->type == NODE_FOR) {
            analyze_semantics(node->init);
        }
        analyze_semantics(node->cond);
        analyze_semantics(node->body);
        if (node->type == NODE_FOR) {
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
