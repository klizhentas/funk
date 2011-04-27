#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "model.h"
#include "hashtable.h"
#include "globals.h"

object* alloc_object(void) {
    object *obj;

    obj = malloc(sizeof(object));
    if (obj == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    return obj;
}

object* make_integer(long value) {
    object* obj = alloc_object();

    obj->type = INTEGER;
    obj->data.integer.value = value;
    
    return obj;
}

object* make_boolean(bool value) {
    object* obj  = alloc_object();

    obj->type = BOOLEAN;
    obj->data.boolean.value = value;

    return obj;
}

object* make_character(char value) {
    object* obj = alloc_object();
    
    obj->type = CHARACTER;
    obj->data.character.value = value;

    return obj;
}

object* make_string(char* value){
    object* obj = alloc_object();
    
    obj->type = STRING;
    obj->data.string.value = malloc(strlen(value) + 1);
    if(obj->data.string.value == NULL){
        fprintf(stderr, "out of memory");
        exit(1);
    }
    strcpy(obj->data.string.value, value);
    return obj;
}

static object make_auto_string(char* value){
    object obj;
    obj.type = STRING;
    obj.data.string.value = value;
    return obj;
}

object* make_empty_list(){
    object* obj = alloc_object();
    obj->type = EMPTY_LIST;
    return obj;
}

object* cons(object* car_obj, object* cdr_obj){
    object* obj = alloc_object();
    obj->type = PAIR;
    obj->data.pair.car = car_obj;
    obj->data.pair.cdr = cdr_obj;
    return obj;
}

object *car(object *pair) {
    return pair->data.pair.car;
}

object *cdr(object *pair) {
    return pair->data.pair.cdr;
}

void set_car(object* obj, object* value) {
    obj->data.pair.car = value;
}

void set_cdr(object* obj, object* value) {
    obj->data.pair.cdr = value;
}

object* make_symbol(char* name){

    object name_string = make_auto_string(name);
    object* symbol = hashtable_lookup(SymbolTable, &name_string);
    if(symbol != NULL){
        return symbol;
    }

    symbol = alloc_object();
    symbol->type = SYMBOL;
    symbol->data.symbol.value = (malloc(strlen(name) + 1));
    if(symbol->data.symbol.value == NULL){
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    strcpy(symbol->data.symbol.value, name);
    hashtable_put(SymbolTable, symbol, symbol);
    return symbol;
}

object* make_builtin_fn(fn_pointer pfn){
    object* obj = alloc_object();
    obj->type = BUILTIN_FN;
    obj->data.builtin_fn.value = pfn;
    return obj;
}

object* make_compound_fn(object* parameters, object* body, environment* env){
    object* obj = alloc_object();

    obj->type = COMPOUND_FN;
    obj->data.compound_fn.parameters = parameters;
    obj->data.compound_fn.body = body;
    obj->data.compound_fn.env = env;

    return obj;
}

bool is_integer(object *obj) {
    return obj->type == INTEGER;
}

bool is_boolean(object *obj) {
    return obj->type == BOOLEAN;
}

bool is_true(object *obj){
    return is_boolean(obj) && obj->data.boolean.value == true;
}

bool is_false(object *obj){
    return is_boolean(obj) && ! is_true(obj);
}

bool is_character(object *obj) {
    return obj->type == CHARACTER;
}

bool is_string(object *obj){
    return obj->type == STRING;
}

bool is_empty_list(object* obj){
    return obj->type == EMPTY_LIST;
}

bool is_pair(object* obj){
    return obj->type == PAIR;
}

bool is_symbol(object *obj) {
    return obj->type == SYMBOL;
}

bool is_builtin_fn(object* obj) {
    return obj->type == BUILTIN_FN;
}

bool is_compound_fn(object *obj) {
    return obj->type == COMPOUND_FN;
}

bool is_fn(object* obj) {
    return is_builtin_fn(obj) || is_compound_fn(obj);
}
