#ifndef FILES_FUNCS_H
#define FILES_FUNCS_H

#include "tree.h"

void startWriting(tree_t* tree);
node_t* startReading(tree_t* tree);
isError_t clearFile (const char* file_name);

#endif // FILES_FUNCS_H
