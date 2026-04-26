#ifndef OPTIMIZE_H
#define OPTIMIZE_H
#include "ast.h"

// We pass a pointer to the node pointer so we can replace the node itself
void optimize_ast(ASTNode **node);

#endif