#include "errors.h"
#include "graphDebug.h"
#include "safetyTree.h"
#include "tree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define NUMBER_ERRORS 6

storageListErrors errors[NUMBER_ERRORS] = { {HAVE_NO_ERRORS,          code_HAVE_NO_ERRORS,          "you have no any error!!!!",     true },
                                            {ERR_FAIL_INIT_TREE,      code_ERR_FAIL_INIT_TREE,      "failed to init tree!",          false},
                                            {ERR_INV_SIZE,            code_ERR_INV_SIZE,            "invalid size of tree",          false},
                                            {ERR_CYCLE,               code_ERR_CYCLE,               "there is cycle in tree!",       false},
                                            {ERR_FAIL_CLEAR_LOG_FILE, code_ERR_FAIL_CLEAR_LOG_FILE, "failed to clear html log file", false},
                                            {ERR_INV_REASON_FOR_DUMP, code_ERR_INV_REASON_FOR_DUMP, "invalid reason for dump tree",  false} };

int count_log_files = 0;

void setError (error_t error)
{

    errors[HAVE_NO_ERRORS].isError = false;
    for (int index = 1; index < NUMBER_ERRORS; index++)
    {
        if (errors[index].error == error)
        {
            errors[index].isError = true;
        }
    }

    return;
}

void printErrors()
{
    fprintf(stderr, "LIST OF ERROR: \n");
    for (int index = 1; index < NUMBER_ERRORS; index++)
    {
        if (errors[index].isError)
        {
            fprintf(stderr, "ERROR: %s\n", errors[index].description);
        }
    }

    return;
}

isError_t treeVerify (tree_t* tree, const char* file_name, const char* func_name, int line)
{
    int global_code_error = 0;
    func_data f_data = {file_name, func_name, line};
    if (tree == nullptr)
    {
        count_log_files++;
        errors[HAVE_NO_ERRORS].isError      = false;
        errors[ERR_FAIL_INIT_TREE].isError  = true;
        global_code_error |= code_ERR_FAIL_INIT_TREE;
        treeDump(tree, &f_data, global_code_error, count_log_files, nullptr, "ERROR %d", global_code_error);
        return HAVE_ERROR;
    }
    if (tree->size < 0)
    {
        count_log_files++;
        errors[HAVE_NO_ERRORS].isError = false;
        errors[ERR_INV_SIZE].isError   = true;
        global_code_error |= code_ERR_INV_SIZE;
        treeDump(tree, &f_data, global_code_error, count_log_files, nullptr, "ERROR %d", global_code_error);
        return HAVE_ERROR;
    }

    node_t** visited_nodes = (node_t**)calloc(tree->size + 3, sizeof(node_t*));
    int counter = 0;

    if (nodeVerify(tree->root, visited_nodes, &counter))
    {
        count_log_files++;
        setError(ERR_CYCLE);
        global_code_error |= code_ERR_CYCLE;
        treeDump(tree, &f_data, global_code_error, count_log_files, nullptr, "ERROR %d", global_code_error);
        return HAVE_ERROR;
    }

    if (errors[HAVE_NO_ERRORS].isError == false)
    {
        treeDump(tree, &f_data, global_code_error, count_log_files, nullptr, "ERROR %d", global_code_error);
        return HAVE_ERROR;
    }

    return NO_ERRORS;
}

isError_t nodeVerify (node_t* node, node_t** visited_nodes, int* counter)
{
    if (node == nullptr) return NO_ERRORS;

    for (index_t index = 0; index < *counter; index++)
    {
        if (visited_nodes[index] == node)
        {
            return HAVE_ERROR;
        }
    }

    visited_nodes[*counter] = node;
    (*counter)++;

    isError_t verifyLeft  = nodeVerify(node->left,  visited_nodes, counter);
    isError_t verifyRight = nodeVerify(node->right, visited_nodes, counter);

    if (verifyLeft == NO_ERRORS) return verifyRight;
    else                         return verifyLeft;


}

void treeDump (tree_t* tree, func_data* f_data, int global_code_error, int count_log_files, node_t* deleted_node, const char* reason, ...)
{
    FILE* log_file_html = fopen("graphDump.html", "a");
    #ifndef NDEBUG
    assert(log_file_html && f_data);
    #endif
    printStartDump(log_file_html, f_data, count_log_files);

    char formatted_reason[200] = {};
    va_list args = NULL;
    va_start(args, reason);
    vsnprintf(formatted_reason, sizeof(formatted_reason), reason, args);
    va_end(args);
    fprintf(log_file_html, "<p style=\"color: #0e450cff;\">%s</p>\n", formatted_reason);

    //printReasonDump(log_file_html, mode);
    if (tree == nullptr)
    {
        fprintf(log_file_html, "Tree [%p]\n", tree);
        fprintf(log_file_html, "FATAL ERROR\n\n");

        return;
    }
    FILE* log_file = creatDotFile(tree, count_log_files, deleted_node);
    creatLogPicture(log_file, tree, count_log_files);

    printErrorsInLog(log_file_html, global_code_error);
    printMainInfoTree(log_file_html, tree);
    printImage(log_file_html, count_log_files);

    fprintf(log_file_html, "-----------------------------------END-TreeDump------------------------\n\n");
    fprintf(log_file_html, "</pre>\n");

    fclose(log_file_html);

    return;
}

void printStartDump (FILE* log_file_html, func_data* f_data, int count_log_files)
{
    #ifndef NDEBUG
    assert(log_file_html && f_data);
    #endif
    fprintf(log_file_html, "<!DOCTYPE html>\n");
    fprintf(log_file_html, "<html lang = 'ru'>\n");
    fprintf(log_file_html, "<head>\n");
    fprintf(log_file_html, "<style>\n");
    fprintf(log_file_html, "body { background-color: #cfa8ceff }\n");
    fprintf(log_file_html, "</style>\n");
    fprintf(log_file_html, "</head>\n");
    fprintf(log_file_html, "<body>\n\n");
    fprintf(log_file_html, "<h3 align=\"center\"> TreeDump called from %s:%d from func %s</h3>\n", f_data->file_name, f_data->line, f_data->func_name);
    fprintf(log_file_html, "<pre>\n");
    fprintf(log_file_html, "DUMP NUMBER %d\n", count_log_files);
    fprintf(log_file_html, "-----------------------------------TreeDump------------------------\n\n");

    #ifndef NDEBUG
    assert(log_file_html && f_data);
    #endif

    return;
}

void printErrorsInLog (FILE* log_file_html, int global_code_error)
{
    #ifndef NDEBUG
    assert(log_file_html);
    #endif

    if (global_code_error)
    {
        fprintf(log_file_html, "error code:\t%d\n", global_code_error);
        fprintf(log_file_html, "ERRORS:\n");
        for (index_t index = 0; index < NUMBER_ERRORS; index++)
        {
            if (errors[index].isError)
            {
                fprintf(log_file_html, "error%d: %s\n", index, errors[index].description);
            }
        }
    }

    #ifndef NDEBUG
    assert(log_file_html);
    #endif

    return;
}

void printMainInfoTree (FILE* log_file_html, tree_t* tree)
{
    #ifndef NDEBUG
    assert(log_file_html && tree);
    #endif

    fprintf(log_file_html, "tree [%p]:\n", tree);
    fprintf(log_file_html, "size = %zu\n", tree->size);

    return;
}

void printImage (FILE* log_file_html, int count_log_files)
{
    #ifndef NDEBUG
    assert(log_file_html);
    #endif

    fprintf(log_file_html, "IMAGE\n");
    fprintf(log_file_html, "<img src=\"./images/logFile_%d.png\" width=\"%d\" alt=\"DUMP %d\"/>\n\n", count_log_files, 1000, count_log_files);

    #ifndef NDEBUG
    assert(log_file_html);
    #endif

    return;
}

void printReasonDump (FILE* log_file_html, mode_dump mode)
{
    #ifndef NDEBUG
    assert(log_file_html);
    #endif

    switch (mode)
    {
        case ERROR:
            break;
        case BEFORE_INSERT:
            fprintf(log_file_html, "<p style=\"color: red;\">BEFORE INSERT</p>\n");
            break;
        case AFTER_INSERT:
            fprintf(log_file_html, "<p style=\"color: green;\">AFTER INSERT</p>\n");
            break;
        case BEFORE_DELETE:
            fprintf(log_file_html, "<p style=\"color: red;\">BEFORE DELETE</p>\n");
            break;
        case AFTER_DELETE:
            fprintf(log_file_html, "<p style=\"color: green;\">AFTER DELETE</p>\n");
            break;
        default:
            setError(ERR_INV_REASON_FOR_DUMP);
    }

    #ifndef NDEBUG
    assert(log_file_html);
    #endif

    return;
}
