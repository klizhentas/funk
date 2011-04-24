#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "model.h"
#include "read.h"
#include "globals.h"

static char is_delimiter(int c) {
    return isspace(c) || c == EOF ||
           c == '('   || c == ')' ||
           c == '"'   || c == ';';
}

static char is_initial(int c) {
    return isalpha(c) || c == '*' || c == '/' || c == '>' ||
        c == '<' || c == '=' || c == '?' || c == '!';
}

static int peek(FILE *in) {
    int c;

    c = getc(in);
    ungetc(c, in);
    return c;
}

static void eat_whitespace(FILE *in) {
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

static object* read_integer(FILE*in){
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

static bool read_literal(FILE* in, const char* literal){
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

static object* read_boolean(FILE* in){
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

static object* read_character(FILE* in){
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

static object* read_string(FILE* in){
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

object* read(FILE* in);

static object* read_pair_contents(FILE* in){

    eat_whitespace(in);
    char c = getc(in);
    if(c == ')'){
        return EmptyList;
    }

    ungetc(c, in);
    object* car_obj = read(in);

    eat_whitespace(in);

    c = getc(in);

    /*read classic pair*/
    if(c == '.'){
        c = peek(in);
        if(!is_delimiter(c)){
            fprintf(stderr, "hey, '.' is not followed by delimiter!");
            exit(1);
        }

        object* cdr_obj = read(in);                
        eat_whitespace(in);
            
        c = getc(in);

        if(c != ')'){
            fprintf(stderr, "Mismatched parenthesis! I'm looking for ')'");
            exit(1);
        }
                
        return cons(car_obj, cdr_obj);
    }
    /*read list*/
    else {
        ungetc(c, in);
        object* cdr_obj = read_pair_contents(in);
        return cons(car_obj, cdr_obj);
    }
}

static object* read_pair(FILE* in){    
    char c = getc(in);

    if(c == '('){
        return read_pair_contents(in);
    }
    else{
        ungetc(c, in);
        return NULL;
    }
}

static object* read_symbol(FILE* in){
    int buffer_max = 1024;
    char buffer[buffer_max];
    
    char c = getc(in);
    
    if(is_initial(c) || 
       ((c == '+' || c == '-') && 
        is_delimiter(peek(in)))){
        int i = 0;
        while(is_initial(c) || 
              isdigit(c) ||
              c == '+' || 
              c == '-' || 
              c == '_'){
            if(i < buffer_max - 1){
                buffer[i++] = c;
            }
            else{
                fprintf(stderr, "symbol is too long "
                        "Maximum length is %d\n", buffer_max - 1);
                exit(1);
            }
            c = getc(in);
        }

        if(is_delimiter(c)){
            ungetc(c, in);
            buffer[i] = '\0';
            return make_symbol(buffer);
        }
        else{
            fprintf(stderr, "symbol is not followed by delimiter, unexpected %c", c);
            exit(1);
        }
    }
    else{
        ungetc(c, in);
        return NULL;
    }
}

static object* read_quote(FILE* in){
    char c = getc(in);
    if(c == '\''){
        return cons(QuoteSymbol, cons(read(in), EmptyList));
    }
    else {
        ungetc(c, in);
        return NULL;
    }
}

static object* (*READERS[]) (FILE*in) = {
    read_integer, 
    read_boolean, 
    read_character,
    read_string,
    read_pair, 
    read_symbol,
    read_quote
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
