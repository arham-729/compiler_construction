#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// Helper to initialize all fields of a new node
ASTNode* init_node(NodeType type) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->left = node->right = node->cond = node->body = node->init = node->update = node->next = NULL;
    node->name = node->op = NULL;
    node->value = 0;
    node->array_size = 0;
    node->array_size2 = 0;
    return node;
}

ASTNode* create_num_node(int value) {
    ASTNode *node = init_node(NODE_NUM);
    node->value = value;
    return node;
}

ASTNode* create_id_node(char *name) {
    ASTNode *node = init_node(NODE_ID);
    node->name = strdup(name);
    return node;
}

ASTNode* create_assign_node(char *name, ASTNode *expr) {
    ASTNode *node = init_node(NODE_ASSIGN);
    node->name = strdup(name);
    node->left = expr;
    return node;
}

ASTNode* create_array_assign_node(char *name, ASTNode *index, ASTNode *expr) {
    ASTNode *node = init_node(NODE_ARRAY_ASSIGN);
    node->name = strdup(name);
    node->right = index;  // Store index in right
    node->left = expr;    // Store value in left
    return node;
}

ASTNode* create_array_assign_2d_node(char *name, ASTNode *index1, ASTNode *index2, ASTNode *expr) {
    ASTNode *node = init_node(NODE_ARRAY_ASSIGN);
    node->name = strdup(name);
    node->left = expr;    // Store value in left
    node->right = index1; // Store index1 in right
    node->cond = index2;  // Store index2 in cond
    return node;
}

ASTNode* create_binop_node(char *op, ASTNode *left, ASTNode *right) {
    ASTNode *node = init_node(NODE_BINOP);
    node->op = strdup(op);
    node->left = left;
    node->right = right;
    return node;
}

ASTNode* create_unop_node(char *op, ASTNode *operand) {
    ASTNode *node = init_node(NODE_UNOP);
    node->op = strdup(op);
    node->left = operand;
    return node;
}

ASTNode* create_var_decl_node(char *type, char *name) {
    ASTNode *node = init_node(NODE_VAR_DECL);
    node->name = strdup(name);
    node->op = strdup(type);
    return node;
}

ASTNode* create_array_decl_node(char *type, char *name, int size) {
    ASTNode *node = init_node(NODE_ARRAY_DECL);
    node->name = strdup(name);
    node->op = strdup(type);
    node->array_size = size;
    return node;
}

ASTNode* create_array_decl_2d_node(char *type, char *name, int size1, int size2) {
    ASTNode *node = init_node(NODE_ARRAY_DECL);
    node->name = strdup(name);
    node->op = strdup(type);
    node->array_size = size1;
    node->array_size2 = size2;
    return node;
}

ASTNode* create_seq_node(ASTNode *left, ASTNode *right) {
    ASTNode *node = init_node(NODE_SEQ);
    node->left = left;
    node->right = right;
    return node;
}

ASTNode* create_arg_list_node(ASTNode *left, ASTNode *right) {
    ASTNode *node = init_node(NODE_ARG_LIST);
    node->left = left;
    node->right = right;
    return node;
}

ASTNode* create_if_node(ASTNode *cond, ASTNode *then_body, ASTNode *else_body) {
    ASTNode *node = init_node(NODE_IF);
    node->cond = cond;
    node->left = then_body;
    node->right = else_body;
    return node;
}

ASTNode* create_while_node(ASTNode *cond, ASTNode *body) {
    ASTNode *node = init_node(NODE_WHILE);
    node->cond = cond;
    node->body = body;
    return node;
}

ASTNode* create_for_node(ASTNode *init, ASTNode *cond, ASTNode *update, ASTNode *body) {
    ASTNode *node = init_node(NODE_FOR);
    node->init = init;
    node->cond = cond;
    node->update = update;
    node->body = body;
    return node;
}

ASTNode* create_func_decl_node(char *return_type, char *name, ASTNode *params, ASTNode *body) {
    ASTNode *node = init_node(NODE_FUNC_DECL);
    node->name = strdup(name);
    node->op = strdup(return_type);
    node->next = params;
    node->body = body;
    return node;
}

ASTNode* create_func_call_node(char *name, ASTNode *args) {
    ASTNode *node = init_node(NODE_FUNC_CALL);
    node->name = strdup(name);
    node->left = args;
    return node;
}

ASTNode* create_return_node(ASTNode *expr) {
    ASTNode *node = init_node(NODE_RETURN);
    node->left = expr;
    return node;
}

ASTNode* create_array_access_node(char *name, ASTNode *index) {
    ASTNode *node = init_node(NODE_ARRAY_ACCESS);
    node->name = strdup(name);
    node->left = index;
    return node;
}

ASTNode* create_array_access_2d_node(char *name, ASTNode *index1, ASTNode *index2) {
    ASTNode *node = init_node(NODE_ARRAY_ACCESS);
    node->name = strdup(name);
    node->left = index1;
    node->right = index2;
    return node;
}

void print_ast(ASTNode *node, int level) {
    int i;
    if (!node) return;
    for (i = 0; i < level; i++) printf("  ");
    
    if (node->type == NODE_NUM) printf("NUM: %d\n", node->value);
    else if (node->type == NODE_ID) printf("ID: %s\n", node->name);
    else if (node->type == NODE_ASSIGN) printf("ASSIGN to %s\n", node->name);
    else if (node->type == NODE_ARRAY_ASSIGN) printf("ARRAY_ASSIGN to %s\n", node->name);
    else if (node->type == NODE_BINOP) printf("BINOP: %s\n", node->op);
    else if (node->type == NODE_UNOP) printf("UNOP: %s\n", node->op);
    else if (node->type == NODE_VAR_DECL) printf("VAR_DECL: %s (%s)\n", node->name, node->op);
    else if (node->type == NODE_SEQ) printf("SEQ\n");
    else if (node->type == NODE_ARG_LIST) printf("ARG_LIST\n");
    else if (node->type == NODE_IF) printf("IF\n");
    else if (node->type == NODE_WHILE) printf("WHILE\n");
    else if (node->type == NODE_FOR) printf("FOR\n");
    else if (node->type == NODE_FUNC_DECL) printf("FUNC_DECL: %s\n", node->name);
    else if (node->type == NODE_FUNC_CALL) printf("FUNC_CALL: %s\n", node->name);
    else if (node->type == NODE_RETURN) printf("RETURN\n");
    else if (node->type == NODE_ARRAY_DECL) {
        if (node->array_size2 > 0)
            printf("ARRAY_DECL: %s[%d][%d]\n", node->name, node->array_size, node->array_size2);
        else
            printf("ARRAY_DECL: %s[%d]\n", node->name, node->array_size);
    }
    else if (node->type == NODE_ARRAY_ACCESS) printf("ARRAY_ACCESS: %s\n", node->name);
    
    if (node->type == NODE_IF) {
        for (i = 0; i < level + 1; i++) printf("  ");
        printf("COND:\n");
        print_ast(node->cond, level + 2);
        for (i = 0; i < level + 1; i++) printf("  ");
        printf("THEN:\n");
        print_ast(node->left, level + 2);
        if (node->right) {
            for (i = 0; i < level + 1; i++) printf("  ");
            printf("ELSE:\n");
            print_ast(node->right, level + 2);
        }
    } else if (node->type == NODE_WHILE) {
        for (i = 0; i < level + 1; i++) printf("  ");
        printf("COND:\n");
        print_ast(node->cond, level + 2);
        for (i = 0; i < level + 1; i++) printf("  ");
        printf("BODY:\n");
        print_ast(node->body, level + 2);
    } else if (node->type == NODE_FOR) {
        if (node->init) {
            for (i = 0; i < level + 1; i++) printf("  ");
            printf("INIT:\n");
            print_ast(node->init, level + 2);
        }
        for (i = 0; i < level + 1; i++) printf("  ");
        printf("COND:\n");
        print_ast(node->cond, level + 2);
        if (node->update) {
            for (i = 0; i < level + 1; i++) printf("  ");
            printf("UPDATE:\n");
            print_ast(node->update, level + 2);
        }
        for (i = 0; i < level + 1; i++) printf("  ");
        printf("BODY:\n");
        print_ast(node->body, level + 2);
    } else if (node->type == NODE_FUNC_DECL) {
        for (i = 0; i < level + 1; i++) printf("  ");
        printf("BODY:\n");
        print_ast(node->body, level + 2);
    } else if (node->type == NODE_ARRAY_ASSIGN) {
        for (i = 0; i < level + 1; i++) printf("  ");
        printf("INDEX1:\n");
        print_ast(node->right, level + 2);
        if (node->cond) {
            for (i = 0; i < level + 1; i++) printf("  ");
            printf("INDEX2:\n");
            print_ast(node->cond, level + 2);
        }
        for (i = 0; i < level + 1; i++) printf("  ");
        printf("VALUE:\n");
        print_ast(node->left, level + 2);
    } else {
        print_ast(node->left, level + 1);
        print_ast(node->right, level + 1);
    }
}