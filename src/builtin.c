#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin.h"
#include "globals.h"

object* builtin_sum(object* arguments){
    long result = 0;
    object* arg;
    while(!is_empty_list(arguments)){
        result += car(arguments)->data.integer.value;
        arguments = cdr(arguments);
    }
    return make_integer(result);
}

object* builtin_is_null(object* arguments) {
    return is_empty_list(car(arguments)) ? True : False;
}

object* builtin_is_boolean(object* arguments) {
    return is_boolean(car(arguments)) ? True : False;
}

object* builtin_is_symbol(object* arguments) {
    return is_symbol(car(arguments)) ? True : False;
}

object* builtin_is_integer(object* arguments) {
    return is_integer(car(arguments)) ? True : False;
}

object* builtin_is_char(object* arguments) {
    return is_character(car(arguments)) ? True : False;
}

object* builtin_is_string(object *arguments) {
    return is_string(car(arguments)) ? True : False;
}

object* builtin_is_pair(object* arguments) {
    return is_pair(car(arguments)) ? True : False;
}

object* builtin_is_function(object* arguments) {
    return is_fn(car(arguments)) ? True : False;
}

object* builtin_char_to_integer(object *arguments) {
    return make_integer((car(arguments))->data.character.value);
}

object* builtin_integer_to_char(object* arguments) {
    return make_character((car(arguments))->data.integer.value);
}

object* builtin_number_to_string(object *arguments) {
    char buffer[100];

    sprintf(buffer, "%ld", (car(arguments))->data.integer.value);
    return make_string(buffer);
}

object* builtin_string_to_number(object *arguments) {
    return make_integer(atoi((car(arguments))->data.string.value));
}

object* builtin_symbol_to_string(object* arguments) {
    return make_string((car(arguments))->data.symbol.value);
}

object* builtin_string_to_symbol(object* arguments) {
    return make_symbol((car(arguments))->data.string.value);
}

object* builtin_sub(object* arguments) {
    long result;
    
    result = (car(arguments))->data.integer.value;
    while (!is_empty_list(arguments = cdr(arguments))) {
        result -= (car(arguments))->data.integer.value;
    }
    return make_integer(result);
}

object* builtin_mul(object* arguments) {
    long result = 1;
    
    while (!is_empty_list(arguments)) {
        result *= (car(arguments))->data.integer.value;
        arguments = cdr(arguments);
    }
    return make_integer(result);
}

object* builtin_quotient(object* arguments) {
    return make_integer(
                       ((car(arguments) )->data.integer.value)/
                       ((cadr(arguments))->data.integer.value));
}

object* builtin_remainder(object *arguments) {
    return make_integer(
                       ((car(arguments) )->data.integer.value)%
                       ((cadr(arguments))->data.integer.value));
}

object* builtin_is_number_equal(object *arguments) {
    long value;
    
    value = (car(arguments))->data.integer.value;
    while (!is_empty_list(arguments = cdr(arguments))) {
        if (value != ((car(arguments))->data.integer.value)) {
            return False;
        }
    }
    return True;
}

object* builtin_is_less_than(object *arguments) {
    long previous;
    long next;
    
    previous = (car(arguments))->data.integer.value;
    while (!is_empty_list(arguments = cdr(arguments))) {
        next = (car(arguments))->data.integer.value;
        if (previous < next) {
            previous = next;
        }
        else {
            return False;
        }
    }
    return True;
}

object* builtin_is_greater_than(object* arguments) {
    long previous;
    long next;
    
    previous = (car(arguments))->data.integer.value;
    while (!is_empty_list(arguments = cdr(arguments))) {
        next = (car(arguments))->data.integer.value;
        if (previous > next) {
            previous = next;
        }
        else {
            return False;
        }
    }
    return True;
}

object* builtin_cons(object* arguments) {
    return cons(car(arguments), cadr(arguments));
}

object* builtin_car(object* arguments) {
    return caar(arguments);
}

object* builtin_cdr(object *arguments) {
    return cdar(arguments);
}

object* builtin_set_car(object* arguments) {
    set_car(car(arguments), cadr(arguments));
    return cadr(arguments);
}

object* builtin_set_cdr(object *arguments) {
    set_cdr(car(arguments), cadr(arguments));
    return cadr(arguments);
}

object* builtin_list(object* arguments) {
    return arguments;
}

object* builtin_is_eq(object *arguments) {
    object *obj1;
    object *obj2;
    
    obj1 = car(arguments);
    obj2 = cadr(arguments);
    
    if (obj1->type != obj2->type) {
        return False;
    }

    switch (obj1->type) {
    case INTEGER:
        return (obj1->data.integer.value == 
                obj2->data.integer.value) ?
            True : False;
        break;
    case CHARACTER:
        return (obj1->data.character.value == 
                obj2->data.character.value) ?
            True : False;
        break;
    case STRING:
        return (strcmp(obj1->data.string.value, 
                       obj2->data.string.value) == 0) ?
            True : False;
        break;
    default:
        return (obj1 == obj2) ? True : False;
    }
}

