#include <stdio.h>
#include "ast.h"
#include "target.h"

static int target_label_count = 0;

static int new_target_label(void) {
    return target_label_count++;
}

static void generate_target_expr(ASTNode *node);

static int target_guarantees_return(ASTNode *node) {
    if (!node) return 0;

    if (node->type == NODE_RETURN) {
        return 1;
    }

    if (node->type == NODE_SEQ) {
        if (target_guarantees_return(node->left)) {
            return 1;
        }
        return target_guarantees_return(node->right);
    }

    if (node->type == NODE_IF) {
        return node->right &&
               target_guarantees_return(node->left) &&
               target_guarantees_return(node->right);
    }

    return 0;
}

static void generate_flat_index(ASTNode *owner, ASTNode *index1, ASTNode *index2) {
    generate_target_expr(index1);

    if (index2) {
        int width = (owner && owner->array_size2 > 0) ? owner->array_size2 : 1;

        printf("  PUSHI %d\n", width);
        printf("  MUL\n");
        generate_target_expr(index2);
        printf("  ADD\n");
    }
}

static void generate_target_args(ASTNode *node, int *count) {
    if (!node) return;

    if (node->type == NODE_ARG_LIST) {
        generate_target_args(node->left, count);
        if (node->right) {
            generate_target_args(node->right, count);
        }
        return;
    }

    generate_target_expr(node);
    (*count)++;
}

static void emit_compare(const char *op) {
    if (!op) return;

    if (op[0] == '<' && op[1] == '=' && op[2] == '\0') printf("  CMPLE\n");
    else if (op[0] == '>' && op[1] == '=' && op[2] == '\0') printf("  CMPGE\n");
    else if (op[0] == '=' && op[1] == '=' && op[2] == '\0') printf("  CMPEQ\n");
    else if (op[0] == '!' && op[1] == '=' && op[2] == '\0') printf("  CMPNE\n");
    else if (op[0] == '<' && op[1] == '\0') printf("  CMPLT\n");
    else if (op[0] == '>' && op[1] == '\0') printf("  CMPGT\n");
}

static void generate_target_expr(ASTNode *node) {
    int arg_count = 0;

    if (!node) {
        printf("  PUSHI 0\n");
        return;
    }

    switch (node->type) {
        case NODE_NUM:
            printf("  PUSHI %d\n", node->value);
            break;

        case NODE_ID:
            printf("  LOAD %s\n", node->name);
            break;

        case NODE_ARRAY_ACCESS:
            generate_flat_index(node, node->left, node->right);
            printf("  LOADARR %s\n", node->name);
            break;

        case NODE_FUNC_CALL:
            generate_target_args(node->left, &arg_count);
            printf("  CALL %s %d\n", node->name, arg_count);
            break;

        case NODE_BINOP:
            generate_target_expr(node->left);
            generate_target_expr(node->right);

            if (node->op[0] == '+' && node->op[1] == '\0') printf("  ADD\n");
            else if (node->op[0] == '-' && node->op[1] == '\0') printf("  SUB\n");
            else if (node->op[0] == '*' && node->op[1] == '\0') printf("  MUL\n");
            else if (node->op[0] == '/' && node->op[1] == '\0') printf("  DIV\n");
            else if (node->op[0] == '&' && node->op[1] == '&' && node->op[2] == '\0') printf("  AND\n");
            else if (node->op[0] == '|' && node->op[1] == '|' && node->op[2] == '\0') printf("  OR\n");
            else emit_compare(node->op);
            break;

        case NODE_UNOP:
            generate_target_expr(node->left);
            if (node->op[0] == '!' && node->op[1] == '\0') {
                printf("  NOT\n");
            }
            break;

        default:
            printf("  PUSHI 0\n");
            break;
    }
}

static void emit_parameter_binds(ASTNode *param_list) {
    int count = 0;
    ASTNode *param = param_list;
    ASTNode *params[128];

    while (param && count < 128) {
        params[count++] = param;
        param = param->next;
    }

    while (count > 0) {
        count--;
        if (params[count]->name) {
            printf("  POPARG %s\n", params[count]->name);
        }
    }
}

void generate_target_code(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_SEQ:
            generate_target_code(node->left);
            if (!target_guarantees_return(node->left)) {
                generate_target_code(node->right);
            }
            break;

        case NODE_VAR_DECL:
            printf("  VAR %s\n", node->name);
            break;

        case NODE_ARRAY_DECL:
            if (node->array_size2 > 0) {
                printf("  ARRAY %s %d %d\n", node->name, node->array_size, node->array_size2);
            } else {
                printf("  ARRAY %s %d\n", node->name, node->array_size);
            }
            break;

        case NODE_ASSIGN:
            generate_target_expr(node->left);
            printf("  STORE %s\n", node->name);
            break;

        case NODE_ARRAY_ASSIGN:
            generate_flat_index(node, node->right, node->cond);
            generate_target_expr(node->left);
            printf("  STOREARR %s\n", node->name);
            break;

        case NODE_IF: {
            int else_label = new_target_label();
            int end_label = new_target_label();

            generate_target_expr(node->cond);
            printf("  JZ L%d\n", else_label);
            generate_target_code(node->left);
            printf("  JMP L%d\n", end_label);
            printf("L%d:\n", else_label);
            if (node->right) {
                generate_target_code(node->right);
            }
            printf("L%d:\n", end_label);
            break;
        }

        case NODE_WHILE: {
            int loop_label = new_target_label();
            int end_label = new_target_label();

            printf("L%d:\n", loop_label);
            generate_target_expr(node->cond);
            printf("  JZ L%d\n", end_label);
            generate_target_code(node->body);
            printf("  JMP L%d\n", loop_label);
            printf("L%d:\n", end_label);
            break;
        }

        case NODE_FOR: {
            int loop_label = new_target_label();
            int end_label = new_target_label();

            generate_target_code(node->init);
            printf("L%d:\n", loop_label);
            generate_target_expr(node->cond);
            printf("  JZ L%d\n", end_label);
            generate_target_code(node->body);
            generate_target_code(node->update);
            printf("  JMP L%d\n", loop_label);
            printf("L%d:\n", end_label);
            break;
        }

        case NODE_FUNC_DECL:
            printf("\nFUNC %s:\n", node->name);
            emit_parameter_binds(node->next);
            generate_target_code(node->body);
            if (!target_guarantees_return(node->body)) {
                printf("  RET\n");
            }
            break;

        case NODE_RETURN:
            if (node->left) {
                generate_target_expr(node->left);
                printf("  RETVAL\n");
            } else {
                printf("  RET\n");
            }
            break;

        case NODE_FUNC_CALL:
            generate_target_expr(node);
            printf("  DROP\n");
            break;

        default:
            break;
    }
}
