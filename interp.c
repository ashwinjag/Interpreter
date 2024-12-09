/// @author Ashwin Jagadeesh

#include <stdio.h>
#include <stdlib.h>

#include "interp.h"
#include "symtab.h"
#include "parser.h"

int main(int argc, char* argv[]){

    //exits functions if too many arguements are passed 
    if(argc > 2){
        fprintf(stderr, "usage: interp [sym-table]");
        exit(EXIT_FAILURE);
    } 

    //builds and prints table from provided file 
    if(argc == 2){
        build_table(argv[1]);
        dump_table();
    }

    printf("Enter postfix expressions (CTRL-D to exit):\n");
    printf("> ");

    char expression[MAX_LINE];

    while(fgets(expression, sizeof(expression), stdin) != NULL){

        rep(expression);
        printf("> ");
    }
    printf("\n");
    dump_table();
    
    return 0;
}