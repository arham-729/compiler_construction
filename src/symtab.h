#ifndef SYMTAB_H
#define SYMTAB_H

typedef struct Symbol {
    char *name;
    char *type;
    int scope_level;
    int size2;                 // Second dimension for 2D arrays
    struct Symbol *next;
} Symbol;

void enter_scope();
void exit_scope();
int insert_symbol(char *name, char *type);
int insert_symbol_2d(char *name, char *type, int size2);
Symbol* lookup_symbol(char *name);
void print_symtab();

#endif