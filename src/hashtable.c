#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashtable.h"

static unsigned hash(char *s, int size)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % size;
}

static char* string_from_object(object* obj){
    if(is_symbol(obj)){
        return obj->data.symbol.value;
    }
    else if(is_string(obj)){
        return obj->data.string.value;
    }
    else {
        fprintf(stderr, "unknown symbol type: %d %d\n", obj->type, COMPOUND_FN);
        exit(1);
    }
}

static unsigned object_hash(object* obj, hashtable* table){
    return hash(string_from_object(obj), table->size);
}

static unsigned object_eq(object* a, object* b){
    return strcmp(string_from_object(a), string_from_object(b)) == 0;
}

hashtable* make_hashtable(int size){
    hashtable* table = malloc(sizeof(hashtable));

    if(table == NULL){
        fprintf(stderr, "out of memory\n");
        exit(1);
    }

    table->entries = malloc(sizeof(list_node*)*size);
    if (table->entries == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }

    table->size = size;

    for(int i = 0; i< size; ++i){
        table->entries[i] = NULL;
    }
    return table;
}

static list_node* hashtable_lookup_node(hashtable* table, object *key)
{
    list_node* node;
    for (node = table->entries[object_hash(key, table)]; node != NULL;  node = node->next){
        if(object_eq(key, node->key)){
            return node;
        }
    }

    return NULL;
}


object* hashtable_lookup(hashtable* table, object* key){
    list_node* node = hashtable_lookup_node(table, key);
    if(node != NULL){
        return node->symbol;
    }
    else{
        return NULL;
    }
}

object* hashtable_put(hashtable* table, object* key, object* symbol)
{
    list_node* node = hashtable_lookup_node(table, key);

    if (node == NULL) {
        node = malloc(sizeof(list_node));
        if (node == NULL){
            fprintf(stderr, "out of memory\n");
            exit(1);
        }
        node->symbol = symbol;
        node->key = key;

        unsigned hashval = object_hash(key, table);
        node->next = table->entries[hashval];
        table->entries[hashval] = node;
        return symbol;
    }
    else {
        object* old_symbol = node->symbol;
        node->symbol = symbol;
        return old_symbol;
    }
}


