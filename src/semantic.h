#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "ast.h"

void analyze_semantics(ASTNode *node);
void reset_semantic_errors(void);
int get_semantic_error_count(void);

#endif
