#ifndef TREE_H
#define TREE_H


#include "errors.h"

#include <stdio.h>

typedef char* data_t;
typedef int index_t;

typedef enum side_t
{
    LEFT  = 1,
    RIGHT = 2
} side_t;

typedef enum mode_print
{
    PREORDER  = 1,
    POSTORDER = 2,
    INORDER   = 3
} mode_print;

typedef enum mode_dump
{
    ERROR         = 0,
    BEFORE_INSERT = 1,
    AFTER_INSERT  = 2,
    BEFORE_DELETE = 3,
    AFTER_DELETE  = 4
} mode_dump;

typedef enum answer_t
{
    ANS_NULL  = 0,
    ANS_YES   = 1,
    ANS_NO    = 2,
    OBJECT    = 3
} answer_t;

typedef struct node_t
{
    node_t*  node;
    data_t   object;
    node_t*  left;
    node_t*  right;
    node_t*  father;
    answer_t status;
} node_t;

typedef struct tree_t
{
    node_t* root;
    size_t  size;
} tree_t;


tree_t*   treeInit ();
isError_t nodeInit (tree_t* tree, node_t* node, side_t side, data_t object, data_t question);
isError_t nodeDestroy (node_t* node, int rank);

void printNode          (const node_t* node, int rank, mode_print mode);
void printNodePREorder  (const node_t* node, int rank);
void printNodePOSTorder (const node_t* node, int rank);
void printNodeINorder   (const node_t* node, int rank);

#endif // TREE_H
