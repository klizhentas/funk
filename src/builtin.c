#include "builtin.h"

object* builtin_sum(object* arguments){
    long result = 0;
    object* arg;
    while(!is_empty_list(arguments)){
        result += car(arguments)->data.integer.value;
        arguments = cdr(arguments);
    }
    return result;
}

