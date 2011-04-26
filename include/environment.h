#ifndef H_FUNK_ENVIRONMENT
#define H_FUNK_ENVIRONMENT

#include "hashtable.h"

typedef struct environment_ {
    struct environment_* enclosing_environment;
    hashtable* variables;
} environment;

environment* make_environment(environment* enclosing_environment);

void define_variable(object* var, object* val, environment* env);

object* lookup_variable_value(object* var, environment* env);

void set_variable_value(object* var, object* val, environment* env);

#endif
