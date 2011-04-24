#include "globals.h"

object* True;
object* False;
object* EmptyList;
symtable SymbolTable;
object* QuoteSymbol;


void globals_init() {
    True = make_boolean(true);
    False = make_boolean(false);
    EmptyList = make_empty_list();
    SymbolTable = make_symtable();
    QuoteSymbol = make_symbol("quote");
}

