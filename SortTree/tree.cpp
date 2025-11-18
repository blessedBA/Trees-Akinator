#include "errors.h"
#include "tree.h"
#include "safetyTree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define ROOT 0
#define BAD_VALUE -666

tree_t* treeInit ()
{
    tree_t* tree = (tree_t*)calloc(1, sizeof(tree_t));
    if (tree == nullptr)
    {
        return nullptr;
    }
    node_t* root = (node_t*)calloc(1, sizeof(node_t));
    root->node = root;
    root->value = 0;
    root->left = nullptr;
    root->right = nullptr;
    root->father = nullptr;
    tree->root = root;
    tree->size = 1;

    #ifndef NDEBUG
    if (treeVerify(tree, __FILE__, __func__, __LINE__)) return nullptr;
    #endif

    return tree;
}

isError_t nodeInit (tree_t* tree, node_t* node, side_t side, data_t value)
{
    #ifndef NDEBUG
    if (treeVerify(tree, __FILE__, __func__, __LINE__)) return HAVE_ERROR;
    assert(node);
    #endif

    node_t* new_node = (node_t*)calloc(1, sizeof(node_t));
    assert(new_node);
    new_node->value = value;
    switch (side)
    {
        case LEFT:
            node->left = new_node;
            break;
        case RIGHT:
            node->right = new_node;
            break;
        default:
            assert(0 && "invalid side for adding new node\n");
    }
    new_node->father = node;
    tree->size++;

    #ifndef NDEBUG
    if (treeVerify(tree, __FILE__, __func__, __LINE__)) return HAVE_ERROR;
    assert(node);
    #endif

    return NO_ERRORS;
}

isError_t treeInsert (tree_t* tree, node_t* node, data_t number)
{
    #ifndef NDEBUG
    if (treeVerify(tree, __FILE__, __func__, __LINE__)) return HAVE_ERROR;
    assert(node);
    #endif
    //func_data f_data = {__FILE__, __func__, __LINE__};

    if (number >= node->value)
    {
        if   (node->right == nullptr) return (nodeInit(tree, node, RIGHT, number)) ? HAVE_ERROR : NO_ERRORS;
        else treeInsert(tree, node->right, number);
    }
    else
    {
        if   (node->left  == nullptr) return (nodeInit(tree, node, LEFT,  number)) ? HAVE_ERROR : NO_ERRORS;
        else treeInsert(tree, node->left, number);
    }

    #ifndef NDEBUG
    if (treeVerify(tree, __FILE__, __func__, __LINE__)) return HAVE_ERROR;
    assert(node);
    #endif

    return NO_ERRORS;
}

isError_t nodeDestroy (node_t* node, int rank)
{
    #ifndef NDEBUG
    assert(node);
    #endif

    if (node->left)  nodeDestroy(node->left,  rank + 1);
    if (node->right) nodeDestroy(node->right, rank + 1);

    if (node->father->left  == node) node->father->left  = nullptr;
    if (node->father->right == node) node->father->right = nullptr;
    node->father  = nullptr;
    node->value   = BAD_VALUE;
    node->left    = nullptr;
    node->right   = nullptr;
    free(node);

    return NO_ERRORS;
}

void printNode (const node_t* node, int rank, mode_print mode)
{
    #ifndef NDEBUG
    assert(node);
    #endif

    switch (mode)
    {
        case PREORDER:
            printNodePREorder(node, rank);
            break;
        case POSTORDER:
            printNodePOSTorder(node, rank);
            break;
        case INORDER:
            printNodeINorder(node, rank);
            break;
        default:
            fprintf(stderr, "invalid mode of printTree!!\n");
    }

    #ifndef NDEBUG
    assert(node);
    #endif

    return;
}

void printNodePREorder (const node_t* node, int rank)
{
    #ifndef NDEBUG
    assert(node);
    #endif

    printf("(");
    printf("%d", node->value);
    if (node->left)  printNodePREorder(node->left,  rank + 1);
    if (node->right) printNodePREorder(node->right, rank + 1);
    printf(")");

    #ifndef NDEBUG
    assert(node);
    #endif

    return;
}

void printNodePOSTorder (const node_t* node, int rank)
{
    #ifndef NDEBUG
    assert(node);
    #endif

    printf("(");
    if (node->left)  printNodePOSTorder(node->left,  rank + 1);
    if (node->right) printNodePOSTorder(node->right, rank + 1);
    printf("%d", node->value);
    printf(")");

    #ifndef NDEBUG
    assert(node);
    #endif

    return;
}

void printNodeINorder (const node_t* node, int rank)
{
    #ifndef NDEBUG
    assert(node);
    #endif

    //printf("(");
    if (node->left)  printNodeINorder(node->left,  rank + 1);
    printf("%d ", node->value);
    if (node->right) printNodeINorder(node->right, rank + 1);
    //printf(")");

    #ifndef NDEBUG
    assert(node);
    #endif

    return;
}

isError_t sortArray(tree_t* tree, int* array, int n)
{
    #ifndef NDEBUG
    if (treeVerify(tree, __FILE__, __func__, __LINE__)) return HAVE_ERROR;
    #endif

    for (index_t index = 0; index < n; index++)
    {
        treeInsert(tree, tree->root, array[index]);
    }

    #ifndef NDEBUG
    if (treeVerify(tree, __FILE__, __func__, __LINE__)) return HAVE_ERROR;
    #endif

    return NO_ERRORS;
}
