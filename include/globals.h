/*
  We carefully store all globals
  inside the globals module h and c files

*/
#ifndef H_FUNK_GLOBALS
#define H_FUNK_GLOBALS

#include "model.h"
#include "hashtable.h"
#include "environment.h"

extern object* True;
extern object* False;
extern object* EmptyList;

extern hashtable* SymbolTable;
extern environment* GlobalEnvironment;

extern object* QuoteSymbol;
extern object* DefineSymbol;
extern object* SetSymbol;
extern object* IfSymbol;

void globals_init();

#endif
