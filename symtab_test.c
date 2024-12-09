#include "symtab.h"
#include <stdio.h>

int main(int argc, char *argv[]){
    
    if(argc == 1){
        build_table(NULL);
    } else{
        build_table(argv[1]);
        dump_table();

        symbol_t *result = lookup_table("a");
        
        //testing for symbol a
        if (result != NULL){

            printf("found name: %s, value %d\n", result->var_name, result->val);
        } 
        
        //testing for zzz
        result = lookup_table("zzz");
        if (result != NULL){
            printf("found name: %s, value %d\n", result->var_name, result->val);
        } 

        //testing for symbol that doesn't exist
        result = lookup_table("d");
        if (result == NULL){
            printf("symbol does not exist\n");
        } 

        free_table();
    }
}

