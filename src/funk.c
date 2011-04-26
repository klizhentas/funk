#include <stdio.h>

#include "globals.h"
#include "read.h"
#include "eval.h"
#include "print.h"

int main(void) {

    printf("Welcome to FUNK "
           "Use ctrl-c to exit.\n");

    globals_init();

    while (1) {
        printf("f> ");
        print(eval(read(stdin), GlobalEnvironment));
        printf("\n");
    }

    return 0;
}
