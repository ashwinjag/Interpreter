// @author Ashwin Jagadeesh
#include <stdio.h>
#include <stdlib.h>

#include "tree_node.h"

tree_node_t *make_leaf(exp_type_t exp_type, char *token){

    //create and attempt to allocate space for a tree node struct
    tree_node_t *new_tree_node = (tree_node_t *)malloc(sizeof(tree_node_t));

    if(new_tree_node == NULL){
        fprintf(stderr, "could not allocate memory for tree node");
        return NULL;
    }


    //create and attempt to allocate space for leaf node struct
    leaf_node_t *new_leaf_node = (leaf_node_t *)malloc(sizeof(leaf_node_t));

    if(new_leaf_node == NULL){
        fprintf(stderr, "could not allocate memory for leaf node");
        return NULL;
    }

    new_leaf_node->exp_type = exp_type;

    new_tree_node->type = LEAF;
    new_tree_node->token = token;
    new_tree_node->node = new_leaf_node;

    return new_tree_node;
}

tree_node_t *make_interior(op_type_t op, char *token, tree_node_t *left, tree_node_t *right){

    tree_node_t *new_tree_node = (tree_node_t *)malloc(sizeof(tree_node_t));
    if(new_tree_node == NULL){
        fprintf(stderr, "could not allocate memory for tree node");
        return NULL;
    }


    interior_node_t *interior_node = (interior_node_t *)malloc(sizeof(interior_node_t));
    if(new_tree_node == NULL){
        fprintf(stderr, "could not allocate memory for interior node");
        return NULL;
    }

    interior_node->op = op;
    interior_node->right = right;
    interior_node->left = left;

    new_tree_node->type = INTERIOR;
    new_tree_node->token = token;
    new_tree_node->node = interior_node;

    return new_tree_node;
}