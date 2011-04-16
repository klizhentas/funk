#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

//Model

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

object* True;
object* False;
object* EmptyList;


/* no GC so truely "unlimited extent" */
object* alloc_object(void) {
    object *obj;

    obj = malloc(sizeof(object));
    if (obj == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    return obj;
}

object* make_integer(long value) {
    object* obj = alloc_object();

    obj->type = INTEGER;
    obj->data.integer.value = value;
    
    return obj;
}

object* make_boolean(bool value) {
    object* obj  = alloc_object();

    obj->type = BOOLEAN;
    obj->data.boolean.value = value;

    return obj;
}

object* make_character(char value) {
    object* obj = alloc_object();
    
    obj->type = CHARACTER;
    obj->data.character.value = value;

    return obj;
}

object* make_string(char* value){
    object* obj = alloc_object();
    
    obj->type = STRING;
    obj->data.string.value = malloc(strlen(value) + 1);
    if(obj->data.string.value == NULL){
        fprintf(stderr, "out of memory");
        exit(1);
    }
    strcpy(obj->data.string.value, value);
    return obj;
}

object* make_empty_list(){
    object* obj = alloc_object();
    obj->type = EMPTY_LIST;
    return obj;
}

bool is_integer(object *obj) {
    return obj->type == INTEGER;
}

bool is_boolean(object *obj) {
    return obj->type == BOOLEAN;
}

bool is_true(object *obj){
    return is_boolean(obj) && obj->data.boolean.value == true;
}

bool is_false(object *obj){
    return is_boolean(obj) && ! is_true(obj);
}

bool is_character(object *obj) {
    return obj->type == CHARACTER;
}

bool is_string(object *obj){
    return obj->type == STRING;
}

bool is_empty_list(object* obj){
    return obj->type == EMPTY_LIST;
}


//READ

/***************************** READ ******************************/

char is_delimiter(int c) {
    return isspace(c) || c == EOF ||
           c == '('   || c == ')' ||
        c == '"'   || c == ';';
}

int peek(FILE *in) {
    int c;

    c = getc(in);
    ungetc(c, in);
    return c;
}

void eat_whitespace(FILE *in) {
    int c;
    
    while ((c = getc(in)) != EOF) {
        if (isspace(c)) {
            continue;
        }
        else if (c == ';') { /* comments are whitespace also */
            while (((c = getc(in)) != EOF) && (c != '\n'));
            continue;
        }
        ungetc(c, in);
        break;
    }
}

object* read_integer(FILE*in){
    char c;
    long value = 0;
    short sign = 1;

    c = getc(in);

    if(isdigit(c) || ((c == '-' || c == '+') && isdigit(peek(in)))){

        if(c == '-') {
            sign = -1;
        }
        else if (c == '+') {
            sign = 1;
        }    
        else{
            ungetc(c, in);
        }
    
        while(isdigit(c = getc(in))){
            value = value*10 + (c - '0');
        }

        value *= sign;

        ungetc(c, in);

        return make_integer(value);
    }
    else {
        ungetc(c, in);
        return NULL;
    }
}

bool read_literal(FILE* in, const char* literal){
    int len = strlen(literal);
    char chars[len];

    for (int i = 0; i < len; ++i){
        chars[i] = getc(in);
        if( chars[i] != literal[i]){
            for(int j = i; j >= 0; --j){
                ungetc(chars[j], in);
            }
            return false;
        }            
    }
    return true;
}

object* read_boolean(FILE* in){
    if(read_literal(in, "true")) {
        return True;
    }
    else if (read_literal(in, "false")) {
        return False;
    }
    else {
        return NULL;
    }
}

object* read_character(FILE* in){
    char c = getc(in);
    if(c == '\\') {
        if(read_literal(in, "newline")){
            return make_character('\n');
        }
        else if(read_literal(in, "space")){
            return make_character(' ');
        }
        else if(read_literal(in, "tab")){
            return make_character('\t');
        }

        c = getc(in);
        if(c == EOF){
            fprintf(stderr, "unexpected end of file");
            exit(1);
        }
        else {            
            return make_character(c);
        }
    }
    else{
        ungetc(c, in);
        return NULL;
    }
}

object* read_string(FILE* in){
    int buffer_max = 1024;
    char buffer[buffer_max];
    char read_c;
    int index = 0;

    char c = getc(in);
    if(c == '"'){
        while(true){
            c = getc(in);
            if(c == EOF){
                fprintf(stderr, "unexpected EOF, expected \"");
                exit(1);
            }
            else if(c == '"'){
                break;
            }
            else if(c == '\\'){
                if(read_literal(in, "newline")){
                    read_c = '\n';
                }
                else if(read_literal(in, "tab")){
                    read_c = '\t';
                }
                else if(read_literal(in, "space")){
                    read_c = ' ';
                }
                else{
                    c = getc(in);
                    if(c == 'n'){
                        read_c = '\n';
                    }
                    else if(c == 't'){
                        read_c = '\t';
                    }
                    else if(c == 's'){
                        read_c = ' ';
                    }
                    else if(c == '"'){
                        read_c = '"';
                    }
                    else{
                        if(c == EOF){
                            fprintf(stderr, "unexpected character EOF expected escape char");
                        }
                        else{
                            fprintf(stderr, "unexpected character '%c', expected escape char", c);
                        }

                        exit(1);
                    }
                }
            }
            else {
                read_c = c;
            }
            
            if(index >= buffer_max - 1){
                fprintf(stderr, "this string is supa long, max is: %d", buffer_max-1);
                exit(1);
            }
            else {
                buffer[index++] = read_c;
            }
        }
        buffer[index] = '\0';
        return make_string(buffer);
    }
    else{
        ungetc(c, in);
        return NULL;
    }
}

object* read_empty_list(FILE* in){
    char c = getc(in);
    if(c == '('){
        eat_whitespace(in);
        c = getc(in);
        if(c == ')'){
            return EmptyList;
        }
        else {
            fprintf(stderr, "expected ')'");
            exit(1);
        }
    }
    else{
        ungetc(c, in);
        return NULL;
    }
}

object* (*READERS[]) (FILE*in) = {
    read_integer, 
    read_boolean, 
    read_character,
    read_string,
    read_empty_list
};

object* read(FILE* in){
    char c, p;
    object* obj = NULL;

    eat_whitespace(in);

    for(int i = 0; i< sizeof(READERS)/sizeof(READERS[0]); ++i){
        obj = (*READERS[i])(in);
        if(obj){
            return obj;
        }
    }

    if (obj == NULL) {
        fprintf(stderr, "could not recognize an object");
        exit(1);
    }

    c = getc(in);

    if(is_delimiter(c)){
        return obj;
    }
    else {
        fprintf(stderr, "object not followed by delimiter\n");
        exit(1);
    }
}

//EVAL

// until we have lists and symbols just echo
object* eval(object *exp) {
    return exp;
}


//PRINT

bool print_integer(object* obj){
    if(is_integer(obj)){
        printf("%ld", obj->data.integer.value);
        return true;
    }
    return false;
}

bool print_boolean(object* obj){
    if(is_boolean(obj)){
        if(is_true(obj)){
            printf("true");
        }
        else{
            printf("false");
        }        
        return true;
    }
    else{
        return false;
    }
}

bool print_character(object* obj){
    if(is_character(obj)){
        if(obj->data.character.value == '\n'){
            printf("\\newline");
        }
        else if(obj->data.character.value == '\t'){
            printf("\\tab");
        }
        else if(obj->data.character.value == ' '){
            printf("\\space");
        }
        else{
            printf("\\%c", obj->data.character.value);
        }
    }
}

bool print_string(object* obj){
    if(is_string(obj)){
        char* str = obj->data.string.value;
        putchar('"');
        while (*str != '\0') {
            switch (*str) {
            case '\n':
                printf("\\n");
                break;
            case '\\':
                printf("\\\\");
                break;
            case '"':
                printf("\\\"");
                break;
            default:
                putchar(*str);
            }
            str++;
        }
        putchar('"');
        return true;
    }
    else{
        return false;
    }
}

bool print_empty_list(object* obj){
    if(is_empty_list(obj)){
        printf("()");
        return true;
    }
    else{
        return false;
    }
}

bool (*PRINTERS[]) (object* obj) = {
    print_integer, 
    print_boolean, 
    print_character, 
    print_string,
    print_empty_list
};


void print(object *obj){

    for(int i = 0; i< sizeof(PRINTERS)/sizeof(PRINTERS[0]); ++i){
        if( (*PRINTERS[i])(obj) ) {
            return;
        }
    }
    
    fprintf(stderr, "could not print unknown object");
    exit(1);
}

void init(){
    True = make_boolean(true);
    False = make_boolean(false);
    EmptyList = make_empty_list();
}

//REPL

int main(void) {

    printf("Welcome to FUNK "
           "Use ctrl-c to exit.\n");

    init();

    while (1) {
        printf("f> ");
        print(eval(read(stdin)));
        printf("\n");
    }

    return 0;
}
