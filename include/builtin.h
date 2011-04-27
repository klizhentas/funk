#ifndef H_FUNK_BUILTIN
#define H_FUNK_BUILTIN

#include "model.h"


object* builtin_is_null(object* arguments);
object* builtin_is_boolean(object* arguments);
object* builtin_is_symbol(object* arguments);
object* builtin_is_integer(object* arguments);
object* builtin_is_char(object* arguments);
object* builtin_is_string(object *arguments);
object* builtin_is_pair(object* arguments);
object* builtin_is_function(object* arguments);

object* builtin_char_to_integer(object *arguments);
object* builtin_integer_to_char(object* arguments);
object* builtin_number_to_string(object *arguments);
object* builtin_string_to_number(object *arguments);
object* builtin_symbol_to_string(object* arguments);
object* builtin_string_to_symbol(object* arguments);

object* builtin_sum(object* arguments);
object* builtin_sub(object* arguments);
object* builtin_mul(object* arguments);

object* builtin_quotient(object* arguments);
object* builtin_remainder(object *arguments);

object* builtin_is_number_equal(object *arguments);
object* builtin_is_less_than(object *arguments);
object* builtin_is_greater_than(object* arguments);

object* builtin_cons(object* arguments);
object* builtin_car(object* arguments);
object* builtin_cdr(object *arguments);

object* builtin_set_car(object* arguments);
object* builtin_set_cdr(object *arguments);

object* builtin_list(object* arguments);
object* builtin_is_eq(object *arguments);


#endif
