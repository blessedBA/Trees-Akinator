#include "akinator.h"
#include "errors.h"
#include "filesFuncs.h"
#include "graphDebug.h"
#include "tree.h"
#include "safetyTree.h"

#include <stdio.h>
#include <stdlib.h>

void creatFirstAkinator (tree_t* tree);

int main()
{
    tree_t* tree = treeInit();
    if (clearFile("graphDump.html"))
    {
        setError(ERR_FAIL_CLEAR_LOG_FILE);
        printErrors();
    }

    startAkinator(tree);

    printErrors();
    nodeDestroy(tree->root, 1);

    return 0;
}

void creatFirstAkinator (tree_t* tree)
{
    treeVerify(tree, __FILE__, __func__, __LINE__);

    // nodeInit(tree, tree->root, RIGHT, "car", "live");
    // nodeInit(tree, tree->root, LEFT,  "cat", "live");
    // nodeInit(tree, tree->root->left, RIGHT, "cat", "funny");
    // nodeInit(tree, tree->root->left, LEFT, "dog", "funny");

    treeVerify(tree, __FILE__, __func__, __LINE__);

    return;
}
