#ifndef SYMTAB_H
#define SYMTAB_H

typedef struct Symbol {
    char *name;
    char *type;
    int scope_level;
    int array_size;            // First dimension for arrays
    int size2;                 // Second dimension for 2D arrays
    int param_count;           // Parameter count for functions
    int is_function;           // 1 for functions, 0 otherwise
    struct Symbol *next;
} Symbol;

void enter_scope();
void exit_scope();
int insert_symbol(char *name, char *type);
int insert_array_symbol(char *name, char *type, int size1, int size2);
int insert_function_symbol(char *name, char *type, int param_count);
Symbol* lookup_symbol(char *name);
void print_symtab();

#endif
