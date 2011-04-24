#ifndef H_FUNK_SYMTABLE
#define H_FUNK_SYMTABLE

#include "model.h"

typedef struct _list_node{
    struct _list_node* next;
    object* symbol;
} list_node;

typedef list_node** symtable;

symtable make_symtable();
object* symtable_lookup(symtable table, char* name);
object* symtable_put(symtable table, char *name, object* symbol);

#endif

