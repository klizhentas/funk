#include <stdio.h>
#include <stdlib.h>

#include "print.h"

static bool print_integer(object* obj){
    if(is_integer(obj)){
        printf("%ld", obj->data.integer.value);
        return true;
    }
    return false;
}

static bool print_boolean(object* obj){
    if(is_boolean(obj)){
        if(is_true(obj)){
            printf("true");
        }
        else{
            printf("false");
        }        
        return true;
    }
    else{
        return false;
    }
}

static bool print_character(object* obj){
    if(is_character(obj)){
        if(obj->data.character.value == '\n'){
            printf("\\newline");
        }
        else if(obj->data.character.value == '\t'){
            printf("\\tab");
        }
        else if(obj->data.character.value == ' '){
            printf("\\space");
        }
        else{
            printf("\\%c", obj->data.character.value);
        }
    }
}

static bool print_string(object* obj){
    if(is_string(obj)){
        char* str = obj->data.string.value;
        putchar('"');
        while (*str != '\0') {
            switch (*str) {
            case '\n':
                printf("\\n");
                break;
            case '\\':
                printf("\\\\");
                break;
            case '"':
                printf("\\\"");
                break;
            default:
                putchar(*str);
            }
            str++;
        }
        putchar('"');
        return true;
    }
    else{
        return false;
    }
}

void print(object *obj);

static void print_pair_contents(object* obj){
    print(car(obj));

    object* cdr_obj = cdr(obj);
    if(is_empty_list(cdr_obj)){
        //do nothing
    }
    else if (is_pair(cdr_obj)){
        printf(" ");
        print_pair_contents(cdr_obj);
    }
    else {
        printf(" . ");
        print(cdr_obj);
    }
}

static bool print_pair(object* obj){
    if(is_empty_list(obj)){
        printf("()");
        return true;
    }
    else if(is_pair(obj)){
        printf("(");
        print_pair_contents(obj);
        printf(")");
        return true;
    }
    else{
        return false;
    }
}

static bool print_symbol(object* obj){
    if(is_symbol(obj)){
        printf("%s", obj->data.symbol.value);
    }
    else{
        return false;
    }
}

static bool print_fn(object* obj){
    if(is_fn(obj)){
        printf("<#function>");
    }
    else{
        return false;
    }
}

static bool (*PRINTERS[]) (object* obj) = {
    print_integer, 
    print_boolean, 
    print_character, 
    print_string,
    print_pair,
    print_symbol,
    print_fn
};


void print(object *obj){

    for(int i = 0; i< sizeof(PRINTERS)/sizeof(PRINTERS[0]); ++i){
        if( (*PRINTERS[i])(obj) ) {
            return;
        }
    }
    
    fprintf(stderr, "could not print unknown object");
    exit(1);
}
