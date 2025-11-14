#include "akinator.h"
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
    startAkinator(tree);
    //nodeInit(tree, tree->root, RIGHT, "ноутбук",    "животное");
    //nodeInit(tree, tree->root, LEFT,  "полторашка", "животное");

    //guessObject(tree, tree->root, &matchOrNot);
    //treeDump(tree, &f_data, global_code_error, ++count_dumps, nullptr, "ERROR %d", global_code_error);
    //int n = 1;
    //printNode(tree->root, 1, POSTORDER);
    printErrors();
    nodeDestroy(tree->root, 1);

    return 0;
}
