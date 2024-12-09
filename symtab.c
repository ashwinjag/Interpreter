// @author Ashwin Jagadeeesh

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtab.h"


symbol_t *symbol_table = NULL;


void build_table(char *filename){
        if(filename == NULL){
            symbol_table = NULL;
            return;
        }

        FILE *fp = fopen(filename, "r");
        
        //tries to open file and throws error if can't be opened
        if(fp == NULL){
            perror(filename);
            exit(EXIT_FAILURE);
        }

    char buffer[BUFLEN];
    char name[BUFLEN];
    int value;


    while (fgets(buffer, BUFLEN, fp)){
        
        if (buffer[0] == '#' || buffer[0] == '\n') {
        continue; // Ignore the line
    }
        if(sscanf(buffer,"%s %d", name, &value) == 2){

        
            if(!create_symbol(name, value)){
                fprintf(stderr, "could not add symbol to symbol table");
                fclose(fp);
                exit(EXIT_FAILURE);
            }
        } 
    }
    fclose(fp);
    
}
void dump_table(void){

    if(symbol_table != NULL){
        printf("SYMBOL TABLE:\n");
    }
    
    for (symbol_t *current = symbol_table; current != NULL; current = current->next) {
        printf("\tName: %s, Value: %d\n", current->var_name, current->val);
    }    
    
}

symbol_t *lookup_table(char *variable){

    symbol_t *current = symbol_table;

    while(current != NULL){
        if(strcmp(current->var_name, variable) == 0){
            return current;
        }
        current = current->next;
    }

    return NULL;
}


symbol_t *create_symbol(char *name, int val){
	 symbol_t *new_symbol = (symbol_t*)malloc(sizeof(symbol_t));
        if(new_symbol == NULL){
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
      
        size_t len = strlen(name) + 1; 
        char *copy = malloc(len);
        if (copy) {
            memcpy(copy, name, len);
        }
        new_symbol->var_name = copy;

        if (new_symbol->var_name == NULL) {
            fprintf(stderr, "Memory allocation for var_name failed\n");
            free(new_symbol); 
            exit(EXIT_FAILURE);
        } 

        new_symbol->val = val;
        new_symbol->next = symbol_table;
        symbol_table = new_symbol;

        return new_symbol;
}

void print_symbol(symbol_t *symbol) {
    if (symbol) {
        printf("Symbol: %s, Value: %d\n", symbol->var_name, symbol->val);
    } else {
        printf("Symbol is NULL\n");
    }
}

void free_table(void) {
    symbol_t *current = symbol_table;
    while (current != NULL) {
        symbol_t *next = current->next;
        free(current->var_name);     
        free(current);                
        current = next;                
    }
    symbol_table = NULL; 
}
