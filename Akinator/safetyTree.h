#ifndef SAFETY_TREE_H
#define SAFETY_TREE_H

#include "tree.h"

#include <stdio.h>

typedef struct
{
    const char* file_name;
    const char* func_name;
    int line;
} func_data;

void printErrors ();
void setError (error_t error);
void treeDump (const tree_t* tree, const char* file_name, const char* func_name, int line, int global_code_error, int count_log_files, node_t* deleted_node, const char* reason, ...);
isError_t treeVerify (const tree_t* tree, const char* file_name, const char* func_name, int line);
void printReasonDump (FILE* log_file_html, mode_dump mode);

#endif // SAFETY_TREE_H
