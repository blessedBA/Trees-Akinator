#ifndef FILES_FUNCS_H
#define FILES_FUNCS_H

#include "tree.h"

void startWriting(tree_t* tree);
isError_t writeInFile (tree_t* tree, node_t* node, FILE* output_file);
node_t* startReading(tree_t* tree);
char* creatBuffer (const char* filename);
node_t* creatNode();
void skipSpacesInBuffer(char* buffer, size_t* position);
bool checkNil (char* buffer, size_t* position);
isError_t writeInfoInNode (char* buffer, size_t* position, node_t* node);
node_t* readTreeFromFile(char* buffer, size_t* position, size_t* size, node_t* node);


#endif // FILES_FUNCS_H
