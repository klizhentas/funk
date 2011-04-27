#include <stdlib.h>
#include <stdio.h>
#include "environment.h"


object* lookup_variable_value(object* var, environment* env) {
    object* value = NULL;
    while(env != NULL){
        value = hashtable_lookup(env->variables, var);
        if(value != NULL){
            return value;
        }
        else {
            env = env->enclosing_environment;
        }
    }

    fprintf(stderr, "lookup: unbound variable: %s\n", var->data.symbol.value);
    exit(1);
}

void set_variable_value(object* var, object* val, environment* env) {
    object* value = NULL;
    while(env != NULL){
        value = hashtable_lookup(env->variables, var);
        if(value != NULL){
            hashtable_put(env->variables, var, val);
            return;
        }
        else {
            env = env->enclosing_environment;
        }
    }

    fprintf(stderr, "set: unbound variable: %s\n", var->data.symbol.value);
    exit(1);
}

void define_variable(object* var, object* val, environment* env) {
    hashtable_put(env->variables, var, val);
}

environment* make_environment(environment* enclosing_environment){
    environment* env = malloc(sizeof(environment));
    if(env == NULL){
        fprintf(stderr, "out of memory\n");
        exit(1);
    }

    env->enclosing_environment = enclosing_environment;
    env->variables = make_hashtable(128);
    return env;
}

environment* extend_environment(object* vars, object* vals, environment* enclosing_environment) {
    environment* env = make_environment(enclosing_environment);
    object* var, *val;

    while (!is_empty_list(vars)) {

        if(is_empty_list(vals)){
            fprintf(stderr, "too much arguments\n");
            exit(1);
        }

        var = car(vars);
        val = car(vals);

        define_variable(var, val, env);
        
        vars = cdr(vars);
        vals = cdr(vals);
    }
    
    return env;
}

