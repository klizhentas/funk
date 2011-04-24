/*
  We carefully store all globals
  inside the globals module h and c files

*/
#ifndef H_FUNK_GLOBALS
#define H_FUNK_GLOBALS

#include "model.h"
#include "symtable.h"

extern object* True;
extern object* False;
extern object* EmptyList;
extern symtable SymbolTable;
extern object* QuoteSymbol;

void globals_init();

#endif
