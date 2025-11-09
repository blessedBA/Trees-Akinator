#include "errors.h"
#include "graphDebug.h"
#include "tree.h"
#include "safetyTree.h"

#include <stdio.h>
#include <stdlib.h>


int main()
{
    tree_t* tree = treeInit();
    if (clearFile("graphDump.html"))
    {
        setError(ERR_FAIL_CLEAR_LOG_FILE);
    }
    func_data f_data = {__FILE__, __func__, __LINE__};
    int global_code_error = 0;
    int count_dumps = 0;
    //printf("\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");
    int huy[] = {5, 3, 7, 12, 54, -12, -5, 0, 3, -123, 32, 1235, 654, -432, -1, 1, 5, 5};
    int n = (int)(sizeof(huy) / sizeof(huy[0]));
    nodeInit(tree, tree->root, LEFT,  10);
    nodeInit(tree, tree->root, RIGHT, 20);
    nodeInit(tree, tree->root->left,  RIGHT, 30);
    nodeInit(tree, tree->root->right, RIGHT, 40);
    nodeInit(tree, tree->root->left,  LEFT,  50);
    nodeInit(tree, tree->root->right, LEFT,  60);

    treeDump(tree, &f_data, 0, ++count_dumps, nullptr, "BEFORE INSERT %d", 123);
    treeInsert(tree, tree->root, 123);
    treeDump(tree, &f_data, 0, ++count_dumps, nullptr, "AFTER INSERT %d", 123);

    treeDump(tree, &f_data, 0, ++count_dumps, nullptr, "BEFORE INSERT %d", 30);
    treeInsert(tree, tree->root, 30);
    treeDump(tree, &f_data, 0, ++count_dumps, nullptr, "AFTER INSERT %d", 30);

    treeDump(tree, &f_data, 0, ++count_dumps, nullptr, "BEFORE INSERT %d", -1);
    treeInsert(tree, tree->root, -1);
    treeDump(tree, &f_data, 0, ++count_dumps, nullptr, "AFTER INSERT %d", -1);
    //sortArray(tree, huy, n);
    printNode(tree->root, 1, INORDER);
    treeDump(tree, &f_data, 0, ++count_dumps, tree->root->left, "BEFORE DELETE %d", 10);
    nodeDestroy(tree->root->left, 0);
    treeDump(tree, &f_data, 0, ++count_dumps, tree->root->left, "AFTER DELETE %d",  10);
    treeDump(tree, &f_data, global_code_error, ++count_dumps, nullptr, "ERROR %d", global_code_error);
    printErrors();
    free(tree);

    return 0;
}
