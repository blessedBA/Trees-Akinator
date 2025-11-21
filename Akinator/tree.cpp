#include "akinator.h"
#include "errors.h"
#include "tree.h"
#include "safetyTree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define ROOT 0
#define BAD_VALUE -666
#define BAD_QUESTION "there is no question!!!"
#define TREE_VERIFY treeVerify(tree, __FILE__, __func__, __LINE__)


tree_t* treeInit ()
{
    tree_t* tree = (tree_t*)calloc(1, sizeof(tree_t));
    if (tree == nullptr)
    {
        return nullptr;
    }
    node_t* root = (node_t*)calloc(1, sizeof(node_t));
    char* start_question = (char*)calloc(10, sizeof(char));
    start_question = "nothing";
    root->object = start_question;
    root->left   = nullptr;
    root->right  = nullptr;
    root->father = nullptr;
    tree->root   = root;
    tree->size   = 1;
    root->status = ANS_NULL;

    if (treeVerify(tree, __FILE__, __func__, __LINE__)) return nullptr;

    return tree;
}

node_t* creatNode() // for reading from file
{
    node_t* node = (node_t*)calloc(1, sizeof(node_t));
    assert(node);
    node->father = nullptr;
    node->left = nullptr;
    node->right = nullptr;
    node->status = ANS_NULL;

    return node;
}

isError_t nodeInit (tree_t* tree, node_t* node, side_t side, data_t object, data_t question)
{
    if (treeVerify(tree, __FILE__, __func__, __LINE__)) return HAVE_ERROR;
    assert(node);

    node_t* new_node = (node_t*)calloc(1, sizeof(node_t));
    assert(new_node);
    char* old_object = node->object;
    //printf("old_object in nodeInit = %s\n", old_object);
    if (node->object != nullptr) node->object = question;

    //printf("question in nodeInit = %s\n", question);
    //printf("\n\n");
    new_node->status = OBJECT;
    switch (side)
    {
        case LEFT:
            new_node->object = object;
            node->left = new_node;
            break;
        case RIGHT:
            if (node->object == nullptr) // for the first initialization in main
            {
                //printf("PENIS\n");
                new_node->object = object;
                node->right = new_node;
                break;
            }
            new_node->object = old_object;
            node->right = new_node;
            break;
        default:
            assert(0 && "invalid side for adding new node\n");
    }
    new_node->father = node;
    tree->size++;
    //printf("new_node->object = %s\n", new_node->object);
    //printf("node->object     = %s\n", node->object);
    //printf("1111111\n");
    //printf("\n\n");
    if (treeVerify(tree, __FILE__, __func__, __LINE__)) return HAVE_ERROR;
    assert(node);

    return NO_ERRORS;
}

isError_t nodeDestroy (tree_t* tree, node_t* node, int rank)
{
    treeVerify(tree, __FILE__, __func__, __LINE__);
    assert(node);

    tree->size--;
    if (node->left)  nodeDestroy(tree, node->left,  rank + 1);
    if (node->right) nodeDestroy(tree, node->right, rank + 1);

    if (node->father != nullptr)
    {
        if (node->father->left  == node) node->father->left  = nullptr;
        if (node->father->right == node) node->father->right = nullptr;
        node->father   = nullptr;
        free(node->object);
    }

    node->object   = nullptr;
    node->left     = nullptr;
    node->right    = nullptr;
    free(node);

    return NO_ERRORS;
}
void printNode (const node_t* node, int rank, mode_print mode)
{
    assert(node);

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

    assert(node);

    return;
}

void printNodePREorder (const node_t* node, int rank)
{
    assert(node);

    printf("(");
    printf("\"%s\"", node->object);
    if (node->left)  printNodePREorder(node->left,  rank + 1);
    if (node->right) printNodePREorder(node->right, rank + 1);
    printf("nil nil");
    printf(")");

    assert(node);

    return;
}

void printNodePOSTorder (const node_t* node, int rank)
{
    assert(node);

    printf("(");
    if (node->left)  printNodePOSTorder(node->left,  rank + 1);
    if (node->right) printNodePOSTorder(node->right, rank + 1);
    printf("%s", node->object);
    printf(")");

    assert(node);

    return;
}

void printNodeINorder (const node_t* node, int rank)
{
    assert(node);

    //printf("(");
    if (node->left)  printNodeINorder(node->left,  rank + 1);
    printf("%s ", node->object);
    if (node->right) printNodeINorder(node->right, rank + 1);
    //printf(")");

    assert(node);

    return;
}
