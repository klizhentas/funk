#include <stdlib.h>
#include <stdio.h>

#include "eval.h"
#include "globals.h"

bool is_self_evaluating(object *exp) {
    return is_boolean(exp)   ||
        is_integer(exp)    ||
        is_character(exp) ||
        is_string(exp);
}

bool is_tagged_list(object *expression, object *tag) {
    if (is_pair(expression)) {
        object* car_obj = car(expression);
        return is_symbol(car_obj) && (car_obj == tag);
    }
    else {
        return false;
    }
}

char is_quoted(object *expression) {
    return is_tagged_list(expression, QuoteSymbol);
}

object * text_of_quotation(object *expression) {
    return cadr(expression);
}

object *eval(object *expression) {
    if (is_self_evaluating(expression)) {
        return expression;
    }
    else if (is_quoted(expression)) {
        return text_of_quotation(expression);
    }
    else {
        fprintf(stderr, "cannot eval unknown expression type\n");
        exit(1);
    }
    fprintf(stderr, "eval illegal state\n");
    exit(1);
}
