#include <stdlib.h>
#include <stdio.h>

#include "globals.h"
#include "environment.h"
#include "builtin.h"

object* True;
object* False;
object* EmptyList;
hashtable* SymbolTable;
object* QuoteSymbol;
environment* GlobalEnvironment;
object* DefineSymbol;
object* SetSymbol;
object* IfSymbol;


void globals_init() {
    True = make_boolean(true);
    False = make_boolean(false);
    EmptyList = make_empty_list();

    GlobalEnvironment = make_environment(NULL);
    SymbolTable = make_hashtable(1024);

    QuoteSymbol = make_symbol("quote");
    DefineSymbol = make_symbol("define");
    SetSymbol = make_symbol("set!");    
    IfSymbol = make_symbol("if");

    define_variable(make_symbol("+"),
                    make_builtin_fn(builtin_sum),
                    GlobalEnvironment);
}

