#include "globals.h"

object* True;
object* False;
object* EmptyList;

void globals_init() {
    True = make_boolean(true);
    False = make_boolean(false);
    EmptyList = make_empty_list();
}

