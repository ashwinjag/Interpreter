// @author: Ashwin Jagadeesh
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "stack.h"
#include "stack_node.h"



stack_t *make_stack(void){
    
    stack_t *new_stack = (stack_t *)malloc(sizeof(stack_t));

    if(new_stack == NULL){
        fprintf(stderr, "could not allocate memory for stack\n");
        return NULL;
    }

    new_stack->top = NULL;

    return new_stack;
}

void push(stack_t *stack, void *data){
    
    stack_node_t *new_node = (stack_node_t *)malloc(sizeof(stack_node_t));

    if(new_node == NULL){
        fprintf(stderr, "could not allocate memory for new node\n");
        exit(EXIT_FAILURE);
    }

    new_node->data = data;
    new_node->next = stack->top;

    stack->top = new_node;
}

void *top(stack_t *stack){
    
    if(stack->top == NULL){
        fprintf(stderr, "stack is empty\n");
        exit(EXIT_FAILURE);
    }

    return stack->top;
}

void pop(stack_t *stack){
    if(stack->top == NULL){
        fprintf(stderr, "stack is empty\n");
        exit(EXIT_FAILURE);
    }

    stack_node_t *tmp = stack->top;
    stack->top = tmp->next;
    free(tmp);
    
}


int empty_stack(stack_t *stack){
    if(stack->top != NULL){
        return 0;
    } else{
        return 1;
    }
}


void free_stack(stack_t *stack){

    stack_node_t *current = stack->top;
    while (current != NULL) {
        stack_node_t *temp = current;
        current = current->next;
        free(temp);
    }
    free(stack);
    
}
