/*
  Defines an object - the generic data structure
  for all funk objects and allocation functions
 */

#ifndef H_FUNK_OBJECT
#define H_FUNK_OBJECT

#include <stdbool.h>

typedef enum {
    EMPTY_LIST,
    INTEGER,
    BOOLEAN,
    SYMBOL,
    CHARACTER,
    STRING,
    PAIR,
    BUILTIN_FN
} object_type;


typedef struct _object {
    object_type type;

    union {
        struct {
            long value;
        } integer;

        struct {
            bool value;
        } boolean;

        struct {
            char value;
        } character;

        struct {
            char* value;
        } string;

        struct {
            struct _object* car;
            struct _object* cdr;
        } pair;

        struct {
            char* value;
        } symbol;

        struct {
            struct _object* (*value)(struct _object* arguments);
        } builtin_fn;

    } data;

} object;

typedef object* (*fn_pointer)(object *arguments);

object* alloc_object(void);

object* make_integer(long value);
object* make_boolean(bool value);
object* make_character(char value);
object* make_string(char* value);
object* make_empty_list();
object* make_symbol(char* value);
object* make_builtin_fn(fn_pointer pfn);

object* cons(object* car, object* cdr);
object* car(object *pair);
object* cdr(object *pair);


bool is_integer(object *obj);
bool is_boolean(object *obj);
bool is_true(object *obj);
bool is_false(object *obj);
bool is_character(object *obj);
bool is_string(object *obj);
bool is_empty_list(object* obj);
bool is_pair(object* obj);
bool is_symbol(object *obj);
bool is_builtin_fn(object* obj);


#define caar(obj)   car(car(obj))
#define cadr(obj)   car(cdr(obj))
#define cdar(obj)   cdr(car(obj))
#define cddr(obj)   cdr(cdr(obj))
#define caaar(obj)  car(car(car(obj)))
#define caadr(obj)  car(car(cdr(obj)))
#define cadar(obj)  car(cdr(car(obj)))
#define caddr(obj)  car(cdr(cdr(obj)))
#define cdaar(obj)  cdr(car(car(obj)))
#define cdadr(obj)  cdr(car(cdr(obj)))
#define cddar(obj)  cdr(cdr(car(obj)))
#define cdddr(obj)  cdr(cdr(cdr(obj)))
#define caaaar(obj) car(car(car(car(obj))))
#define caaadr(obj) car(car(car(cdr(obj))))
#define caadar(obj) car(car(cdr(car(obj))))
#define caaddr(obj) car(car(cdr(cdr(obj))))
#define cadaar(obj) car(cdr(car(car(obj))))
#define cadadr(obj) car(cdr(car(cdr(obj))))
#define caddar(obj) car(cdr(cdr(car(obj))))
#define cadddr(obj) car(cdr(cdr(cdr(obj))))
#define cdaaar(obj) cdr(car(car(car(obj))))
#define cdaadr(obj) cdr(car(car(cdr(obj))))
#define cdadar(obj) cdr(car(cdr(car(obj))))
#define cdaddr(obj) cdr(car(cdr(cdr(obj))))
#define cddaar(obj) cdr(cdr(car(car(obj))))
#define cddadr(obj) cdr(cdr(car(cdr(obj))))
#define cdddar(obj) cdr(cdr(cdr(car(obj))))
#define cddddr(obj) cdr(cdr(cdr(cdr(obj))))


#endif

