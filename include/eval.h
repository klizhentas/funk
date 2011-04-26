/*
  Defines evaluator for all funk objects
  essentially the core of the interpreter
 */

#ifndef H_FUNK_EVAL
#define H_FUNK_EVAL

#include "model.h"
#include "environment.h"

object* eval(object *exp, environment* env);

#endif

