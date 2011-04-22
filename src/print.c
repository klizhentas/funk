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

static bool print_empty_list(object* obj){
    if(is_empty_list(obj)){
        printf("()");
        return true;
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
    print_empty_list
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
