#include "stack.h"
#include <stdio.h>


//void try_to_push(stack_t stack, int n){
    
        

//}

int main(void){
    stack_t *myStack = make_stack();

    if(myStack == NULL){
        
        fputs("cannot create stack\n", stderr);
        return (1);
    } else{
        printf("stack created\n");
    }

    printf("stack is %s\n", empty_stack(myStack) ? "empty" : "not empty");


    push(myStack, "one");
    printf("stack is %s\n", empty_stack(myStack) ? "empty" : "not empty");
    
    stack_node_t top_node = *myStack->top;
    printf("top node after adding one: %s\n", (char *)top_node.data);

    push(myStack, "two");
    stack_node_t *new_top = top(myStack);
    printf("top node after adding two: %s\n", (char *)new_top->data);

    
    pop(myStack);
    stack_node_t *new_top1 = top(myStack);
    printf("top node after popping: %s\n", (char *)new_top1->data);


    free_stack(myStack);
    
    return 0;
}
