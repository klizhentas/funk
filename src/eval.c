#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

bool is_quoted(object *expression) {
    return is_tagged_list(expression, QuoteSymbol);
}

object * text_of_quotation(object *expression) {
    return cadr(expression);
}

bool is_assignment(object *exp) {
    return is_tagged_list(exp, SetSymbol);
}

object* assignment_variable(object *exp) {
    return cadr(exp);
}

object* assignment_value(object *exp) {
    return caddr(exp);
}

bool is_definition(object *exp) {
    return is_tagged_list(exp, DefineSymbol);
}

object* definition_variable(object *exp) {
    return cadr(exp);
}

object* definition_value(object *exp) {
    return caddr(exp);
}

object* eval_assignment(object *exp, environment *env) {
    set_variable_value(
                    assignment_variable(exp),
                    eval(assignment_value(exp), env),
                    env );
    return assignment_variable(exp);    
}

object *eval_definition(object *exp, environment *env) {
    define_variable(
                    definition_variable(exp),
                    eval(definition_value(exp), env),
                    env );
    return definition_variable(exp);
}

bool is_variable(object *exp) {
    return is_symbol(exp);
}

bool is_if(object *expression) {
    return is_tagged_list(expression, IfSymbol);
}

object* if_predicate(object *exp) {
    return cadr(exp);
}

object* if_consequent(object *exp) {
    return caddr(exp);
}

object* if_alternative(object *exp) {
    if (is_empty_list(cdddr(exp))) {
        return False;
    }
    else {
        return cadddr(exp);
    }
}

bool is_true_expression(object* exp){
    if(is_true(exp)){
        if(is_empty_list(exp)){
            return false;
        }

        if(is_integer(exp) && exp->data.integer.value == 0){
            return false;
        }

        if(is_string(exp) && strlen(exp->data.string.value) == 0){
            return false;
        }

        return true;
    }
    else {
        return false;
    }
}



object *eval(object *exp, environment* env) {
    while(true){
        if (is_self_evaluating(exp)) {
            return exp;
        }
        else if (is_quoted(exp)) {
            return text_of_quotation(exp);
        }
        else if (is_variable(exp)){
            return lookup_variable_value(exp, env);
        }
        else if (is_assignment(exp)) {
            return eval_assignment(exp, env);
        }
        else if (is_definition(exp)) {
            return eval_definition(exp, env);
        }
        else if (is_if(exp)){
            if(is_true_expression(eval(if_predicate(exp), env))){
                exp = if_consequent(exp);
            }
            else {
                exp = if_alternative(exp);
            }
        }
        else {
            fprintf(stderr, "cannot eval unknown expression type\n");
            exit(1);
        }
    }

    fprintf(stderr, "eval illegal state\n");
    exit(1);
}
