#ifndef GRAPH_DEBUG_H
#define GRAPH_DEBUG_H

#include "tree.h"

#include <stdio.h>

FILE* creatDotFile (tree_t* tree, int count_log_files, node_t* deleted_node = nullptr);
void creatLogPicture (FILE* log_file, tree_t* tree, int count_log_files);
FILE* openLogFile (int count_log_files);
void creatStartGraph (FILE* log_file);
void creatMainNodes (FILE* log_file, tree_t* tree, node_t* node, node_t* deleted_node);
void creatRibs (FILE* log_file, tree_t* tree, node_t* node);
isError_t clearFile (const char* file_name);
bool checkDeleted (node_t* node, node_t* deleted_node);
bool IsNodeInSubtree (node_t* root, node_t* node);

#endif // GRAPH_DEBUG_H
