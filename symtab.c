#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

Symbol *symbol_table = NULL;
int current_scope = 0;

void enter_scope() { 
    current_scope++; 
}

void exit_scope() {
    Symbol *curr = symbol_table;
    Symbol *prev = NULL;
    // Remove all symbols from the current scope when exiting
    while (curr != NULL) {
        if (curr->scope_level == current_scope) {
            if (prev == NULL) symbol_table = curr->next;
            else prev->next = curr->next;
            Symbol *temp = curr;
            curr = curr->next;
            free(temp->name); free(temp->type); free(temp);
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    current_scope--;
}

int insert_symbol_full(char *name, char *type, int size2) {
    // Check if it already exists in the CURRENT scope
    Symbol *curr = symbol_table;
    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0 && curr->scope_level == current_scope) {
            return 0; // Error: Redeclaration
        }
        curr = curr->next;
    }
    
    // Insert new symbol
    Symbol *new_sym = (Symbol*)malloc(sizeof(Symbol));
    new_sym->name = strdup(name);
    new_sym->type = strdup(type);
    new_sym->scope_level = current_scope;
    new_sym->size2 = size2;
    new_sym->next = symbol_table;
    symbol_table = new_sym;
    return 1;
}

int insert_symbol(char *name, char *type) {
    return insert_symbol_full(name, type, 0);
}

int insert_symbol_2d(char *name, char *type, int size2) {
    return insert_symbol_full(name, type, size2);
}

Symbol* lookup_symbol(char *name) {
    Symbol *curr = symbol_table;
    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) return curr;
        curr = curr->next;
    }
    return NULL; // Not found
}

void print_symtab() {
    printf("\n--- Symbol Table ---\n");
    Symbol *curr = symbol_table;
    while (curr != NULL) {
        if (curr->size2 > 0)
            printf("Name: %s | Type: %s | Scope: %d | Width: %d\n", curr->name, curr->type, curr->scope_level, curr->size2);
        else
            printf("Name: %s | Type: %s | Scope: %d\n", curr->name, curr->type, curr->scope_level);
        curr = curr->next;
    }
    printf("--------------------\n");
}