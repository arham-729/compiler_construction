// ast.h
#ifndef AST_H
#define AST_H

typedef enum {
    NODE_VAR_DECL, NODE_FUNC_DECL, NODE_ASSIGN, 
    NODE_BINOP, NODE_UNOP, NODE_IF, NODE_WHILE, NODE_FOR,
    NODE_NUM, NODE_ID, NODE_SEQ, NODE_FUNC_CALL,
    NODE_RETURN, NODE_ARRAY_ACCESS, NODE_ARRAY_DECL, NODE_ARRAY_ASSIGN,
    NODE_ARG_LIST
} NodeType;

typedef struct ASTNode {
    NodeType type;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *cond;      // For if, while, for
    struct ASTNode *body;      // For if, while, for, function
    struct ASTNode *init;      // For for loop
    struct ASTNode *update;    // For for loop
    struct ASTNode *next;      // For function parameters
    char *name;                // For identifiers, functions
    int value;                 // For numbers
    char *op;                  // For operators (+, -, <, ==)
    int array_size;            // For array declarations
    int array_size2;           // For 2D array declarations
} ASTNode;

ASTNode* create_num_node(int value);
ASTNode* create_id_node(char *name);
ASTNode* create_binop_node(char *op, ASTNode *left, ASTNode *right);
ASTNode* create_unop_node(char *op, ASTNode *operand);
ASTNode* create_assign_node(char *name, ASTNode *expr);
ASTNode* create_array_assign_node(char *name, ASTNode *index, ASTNode *expr);
ASTNode* create_array_assign_2d_node(char *name, ASTNode *index1, ASTNode *index2, ASTNode *expr);
ASTNode* create_var_decl_node(char *type, char *name);
ASTNode* create_array_decl_node(char *type, char *name, int size);
ASTNode* create_array_decl_2d_node(char *type, char *name, int size1, int size2);
ASTNode* create_seq_node(ASTNode *left, ASTNode *right);
ASTNode* create_arg_list_node(ASTNode *left, ASTNode *right);
ASTNode* create_if_node(ASTNode *cond, ASTNode *then_body, ASTNode *else_body);
ASTNode* create_while_node(ASTNode *cond, ASTNode *body);
ASTNode* create_for_node(ASTNode *init, ASTNode *cond, ASTNode *update, ASTNode *body);
ASTNode* create_func_decl_node(char *return_type, char *name, ASTNode *params, ASTNode *body);
ASTNode* create_func_call_node(char *name, ASTNode *args);
ASTNode* create_return_node(ASTNode *expr);
ASTNode* create_array_access_node(char *name, ASTNode *index);
ASTNode* create_array_access_2d_node(char *name, ASTNode *index1, ASTNode *index2);
void print_ast(ASTNode *node, int level);

#endif