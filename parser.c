// @author Ashwin Jagadeesh
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "parser.h"
#include "tree_node.h"

parse_error_t parse_error = PARSE_NONE;
eval_error_t eval_error = EVAL_NONE; 

void rep(char *exp){

    tree_node_t *parsed_tree = make_parse_tree(exp);

    // evaluate only if no errors are found from parsing tree
    if(parse_error == PARSE_NONE){     
        int evaluated_int = eval_tree(parsed_tree);

        // do not print infix if error was thrown during evaluation
        if(eval_error == EVAL_NONE){    
            print_infix(parsed_tree);
            printf(" = %d\n", evaluated_int);
        }
    }

    // clean up the parsed tree if not cleaned up during evaluation
    if(parsed_tree != NULL){ 
        cleanup_tree(parsed_tree);
    }
    
    // reset both eval and parse errors for the next expression
    parse_error = PARSE_NONE;
    eval_error = EVAL_NONE;
}

// sets any errors found during parse
// @param error during parsing 
// @param message to explain to user
void set_parse_error(parse_error_t error, const char *message) {
    parse_error = error;
    fprintf(stderr, "%s\n", message);
}

// sets any errors found during parse
// @param error during evaluating expression 
// @param message to explain to user
void set_eval_error(eval_error_t error, const char *message) {
    eval_error = error;
    fprintf(stderr, "%s\n", message);
}

// returns appropriate operation type according to token in parameter 
op_type_t get_op_type(char* token){
    if(strcmp(token, ADD_OP_STR) == 0){
        return ADD_OP;
    } else if(strcmp(token, SUB_OP_STR) == 0){
        return SUB_OP;
    } else if(strcmp(token, MUL_OP_STR) == 0){
        return MUL_OP;
    } else if(strcmp(token, DIV_OP_STR) == 0){
        return DIV_OP;
    } else if(strcmp(token, MOD_OP_STR) == 0){
        return MOD_OP;
    } else if(strcmp(token, ASSIGN_OP_STR) == 0){
        return ASSIGN_OP;
    } else if(strcmp(token, Q_OP_STR) == 0){
        return Q_OP;
    } else if(strcmp(token, ":") == 0){ // check for comparison operator during ternary
        return ALT_OP;
    } 
    return NO_OP;
}

// this method is called when to verify if token in parse is a valid integer
int is_integer(char *token){

    if(!isdigit((unsigned char)*token)){
        return 0;
    }

    for(const char *p= token + 1; *p != '\0'; ++p){
        if(!isdigit((unsigned char)*p)){
            return 0;
        }
    }
    return 1;
}

// this method is called when to verify if token in parse is a valid symbol
int is_symbol(char *token){
    if(!isalpha((unsigned char)*token)){
        return 0;
    }
    
    for(const char *p = token +1; *p != '\0'; ++p){
        if(!isalnum((unsigned char)*p)){
            return 0;
        }
    }
    return 1;
}

tree_node_t *parse(stack_t *stack){

    // creates a character token of based on the stack's topmost node's data 
    stack_node_t *current = top(stack);
    char* token = (char *)current->data;
    pop(stack);

    // stores the expression's operation type 
    op_type_t op_type = get_op_type(token);

    // ternary operation 
    if (op_type == Q_OP){
        char* token_alt_op = ":";
        op_type_t op_type_alt_op = get_op_type(token_alt_op);

        tree_node_t *right_alt_op = parse(stack);

        // stack shouldn't be empty after parsing right 
        if(empty_stack(stack)){
            set_parse_error(TOO_FEW_TOKENS, "Invalid expression, not enough tokens");
            cleanup_tree(right_alt_op);
            return NULL;  
        }

        tree_node_t *left_alt_op = parse(stack);
        // stack shouldn't be empty after parsing left 
        if(empty_stack(stack)){
            set_parse_error(TOO_FEW_TOKENS, "Invalid expression, not enough tokens");
            cleanup_tree(left_alt_op);
            cleanup_tree(right_alt_op);
            return NULL;  
        }

        tree_node_t *right_q_op = make_interior(op_type_alt_op, token_alt_op, left_alt_op, right_alt_op);

        if(empty_stack(stack)){
            set_parse_error(TOO_FEW_TOKENS, "Invalid expression, not enough tokens");
            cleanup_tree(right_q_op);
            return NULL;  
        }

        tree_node_t *left_q_op = parse(stack);
        return make_interior(op_type, token, left_q_op, right_q_op); 
    } 

    // creates interior node if token is an operation
    else if(op_type != NO_OP){
        tree_node_t *right = parse(stack);

        // stack shouldn't be empty after parsing right 
        if(empty_stack(stack)){
            set_parse_error(TOO_FEW_TOKENS, "Invalid expression, not enough tokens");
            cleanup_tree(right);
            return NULL;  
        }

        tree_node_t *left = parse(stack);

        //check for INVALID_ASSIGNMENT error
        if(op_type == ASSIGN_OP){
            leaf_node_t *left_leaf = left->node;
            leaf_node_t *right_leaf = right->node;

         if(left_leaf->exp_type != SYMBOL && right_leaf->exp_type != SYMBOL){
            set_parse_error(INVALID_ASSIGNMENT, "Invalid assignment");
            cleanup_tree(right);
            cleanup_tree(left);
            return NULL;
            }
        }
        return make_interior(op_type, token, left, right);
    } 

    if(is_integer(token) == 1){
        return make_leaf(INTEGER, token);
    } 
    else if(is_symbol(token) == 1){
        return make_leaf(SYMBOL, token);
        
    }
    set_parse_error(ILLEGAL_TOKEN, "Illegal token");
    return NULL; 
    
}


tree_node_t *make_parse_tree(char *expr){
    char* token = strtok(expr, " \n");
    
    stack_t *myStack = make_stack();
    while(token != NULL){
        push(myStack, token);
        token = strtok(NULL, " \n");
    }

    tree_node_t *parsed_tree = parse(myStack);  

    if(parse_error == PARSE_NONE && empty_stack(myStack) == 0){
        set_parse_error(TOO_MANY_TOKENS, "Invalid expression, too many tokens");
    }

    free_stack(myStack);
    return parsed_tree;
}

int perform_op(op_type_t op, int left, int right){
    int result = 0;

    if(op == ADD_OP){
        result = left + right;
    } else if(op == SUB_OP){
        result = left - right;
    } else if(op == MUL_OP){
        result = left * right;
    } else if(op == DIV_OP){
        result = left / right;
    } else if(op == MOD_OP){
        result = left % right;
    } 

    return result;
}

int eval_tree(tree_node_t * node){
    if(eval_error != EVAL_NONE){
        return 0;
    }
    // leaf node operations
    if(node->type == LEAF){
        leaf_node_t *leaf_node = node->node;
        exp_type_t exp_type = leaf_node->exp_type;

        // checks if node is a symbol in symbol table
        if(exp_type == SYMBOL){
            symbol_t *symbol= lookup_table(node->token);

            if(symbol == NULL){
                set_eval_error(UNDEFINED_SYMBOL, "Undefined symbol");
                return 0;
            }
            return symbol->val;
        }

        //proceeds if node is an integer 
        char *endptr;
        int result_num = strtol(node->token, &endptr, 10);
        return result_num;

    } else{
        //interior node operations
        
        interior_node_t *interior_node = node->node;
        op_type_t op = interior_node->op;
        tree_node_t *left = interior_node->left;
        tree_node_t *right = interior_node->right;

        if(op == Q_OP){
            int left_val = eval_tree(left);
            interior_node_t *alt_op_node = right->node;
            
            if(left_val != 0){
                return eval_tree(alt_op_node->left);
            } else{
                return eval_tree(alt_op_node->right);
            }
        }


        if(op != ASSIGN_OP){
            int left_val = eval_tree(left);
            int right_val = eval_tree(right);
        
            if(op == DIV_OP && right_val == 0){
                set_eval_error(DIVISION_BY_ZERO, "Division by zero");
                return 0;
            }

            if(op == MOD_OP && right_val == 0){
                set_eval_error(INVALID_MODULUS, "Division by zero");
                return 0;
            }
            return perform_op(op, left_val, right_val);
        }
        
        //below is for = operator
        leaf_node_t *left_node = left->node;
        
        if(left_node->exp_type != SYMBOL){
            set_parse_error(INVALID_LVALUE, "Invalid left value");
            return 0;
        }

        symbol_t *symbol = lookup_table(left->token);
        int val = eval_tree(right);

        //create symbol it it deosn't exist in symbol table
        if(symbol == NULL){
            create_symbol(left->token, val);
        } 
        //create a new binding to existing symbol
        else{
            symbol->val = val;
        }
        return val;
    }    
}

void print_infix(tree_node_t * node){
    if(node->type == LEAF){
       printf("%s", node->token); 
    } else{
        interior_node_t *interior_node = node->node;
        printf("(");
        print_infix(interior_node->left);
        printf("%s",node->token);
        print_infix(interior_node->right);
        printf(")");
    }
}

void cleanup_tree(tree_node_t * node){

    if(node->type == INTERIOR){
        interior_node_t *interior_node = node->node;

        cleanup_tree(interior_node->right);

        cleanup_tree(interior_node->left);
        interior_node = NULL;
    }
    free(node->node);
    free(node);
    node = NULL;
}

