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
void treeDump (tree_t* tree, func_data* f_data, int global_code_error, int count_log_files, node_t* deleted_node, const char* reason, ...);
isError_t treeVerify (tree_t* tree, const char* file_name, const char* func_name, int line);
isError_t nodeVerify (node_t* node, node_t** visited_nodes, int* counter);
void printStartDump (FILE* log_file_html, func_data* f_data, int count_log_files);
void printErrorsInLog (FILE* log_file_html, int global_code_error);
void printMainInfoTree (FILE* log_file_html, tree_t* tree);
void printImage (FILE* log_file_html, int count_log_files);
void printReasonDump (FILE* log_file_html, mode_dump mode);

#endif // SAFETY_TREE_H
