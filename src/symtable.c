#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "symtable.h"

#define HASHSIZE 1001

static unsigned hash(char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

symtable make_symtable(){
    symtable table = (symtable)malloc(sizeof(list_node*)*HASHSIZE);
    if (table == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    for(int i = 0; i< HASHSIZE; ++i){
        table[i] = NULL;
    }
    return table;
}

static list_node* symtable_lookup_node(symtable table, char *s)
{
    list_node* node;
    for (node = table[hash(s)]; node != NULL;  node = node->next){
        if(equals_to_symbol(s, node->symbol)){
            return node;
        }
    }

    return NULL;
}

object* symtable_lookup(symtable table, char* name){
    list_node* node = symtable_lookup_node(table, name);
    if(node != NULL){
        return node->symbol;
    }
    else{
        return NULL;
    }
}

object* symtable_put(symtable table, char *name, object* symbol)
{
    list_node* node;

    if ((node = symtable_lookup_node(table, name)) == NULL) {
        node = malloc(sizeof(list_node));
        if (node == NULL){
            fprintf(stderr, "out of memory\n");
            exit(1);
        }
        node->symbol = symbol;

        unsigned hashval = hash(name);
        node->next = table[hashval];
        table[hashval] = node;
        return symbol;
    }
    else {
        fprintf(stderr, "symbol is already here %s, remove manually", name);
        exit(1);
    }
}


