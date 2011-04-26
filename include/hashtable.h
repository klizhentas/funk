#ifndef H_FUNK_HASHTABLE
#define H_FUNK_HASHTABLE

#include "model.h"

typedef struct _list_node {
    struct _list_node* next;
    object* key;
    object* symbol;
} list_node;

typedef struct _hashtable {
    list_node** entries;
    int size;
} hashtable;


hashtable* make_hashtable(int size);
object* hashtable_lookup(hashtable* table, object* key);
object* hashtable_put(hashtable* table, object* key, object* symbol);

#endif

