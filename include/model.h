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
    CHARACTER,
    STRING,
} object_type;

typedef struct {
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

    } data;

} object;

object* alloc_object(void);

object* make_integer(long value);
object* make_boolean(bool value);
object* make_character(char value);
object* make_string(char* value);
object* make_empty_list();

bool is_integer(object *obj);
bool is_boolean(object *obj);
bool is_true(object *obj);
bool is_false(object *obj);
bool is_character(object *obj);
bool is_string(object *obj);
bool is_empty_list(object* obj);


#endif

