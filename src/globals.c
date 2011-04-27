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
object* LambdaSymbol;


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
    LambdaSymbol = make_symbol("lambda");

#define add_procedure(scheme_name, c_name)\
    define_variable(\
        make_symbol(scheme_name),\
        make_builtin_fn(c_name),\
        GlobalEnvironment);

    add_procedure("null?"     , builtin_is_null);
    add_procedure("boolean?"  , builtin_is_boolean);
    add_procedure("symbol?"   , builtin_is_symbol);
    add_procedure("integer?"  , builtin_is_integer);
    add_procedure("char?"     , builtin_is_char);
    add_procedure("string?"   , builtin_is_string);
    add_procedure("pair?"     , builtin_is_pair);
    add_procedure("function?" , builtin_is_function);
    
    add_procedure("char->integer" , builtin_char_to_integer);
    add_procedure("integer->char" , builtin_integer_to_char);
    add_procedure("number->string", builtin_number_to_string);
    add_procedure("string->number", builtin_string_to_number);
    add_procedure("symbol->string", builtin_symbol_to_string);
    add_procedure("string->symbol", builtin_string_to_symbol);
      
    add_procedure("+"        , builtin_sum);
    add_procedure("-"        , builtin_sub);
    add_procedure("*"        , builtin_mul);
    add_procedure("quotient" , builtin_quotient);
    add_procedure("remainder", builtin_remainder);
    add_procedure("="        , builtin_is_number_equal);
    add_procedure("<"        , builtin_is_less_than);
    add_procedure(">"        , builtin_is_greater_than);

    add_procedure("cons"    , builtin_cons);
    add_procedure("car"     , builtin_car);
    add_procedure("cdr"     , builtin_cdr);
    add_procedure("set-car!", builtin_set_car);
    add_procedure("set-cdr!", builtin_set_cdr);
    add_procedure("list"    , builtin_list);

    add_procedure("eq?", builtin_is_eq);
}

