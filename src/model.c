#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "model.h"

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

object* make_empty_list(){
    object* obj = alloc_object();
    obj->type = EMPTY_LIST;
    return obj;
}

object* cons(object* car, object* cdr){
    object* obj = alloc_object();
    obj->type = PAIR;
    obj->data.pair.car = car;
    obj->data.pair.cdr = cdr;
    return obj;
}

object *car(object *pair) {
    return pair->data.pair.car;
}

object *cdr(object *pair) {
    return pair->data.pair.cdr;
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
