#include "akinator.h"
#include "colors.h"
#include "graphDebug.h"
#include "tree.h"
#include "safetyTree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static FILE* openLogFile (int count_log_files);
static void creatStartGraph (FILE* log_file);
static void creatMainNodes (FILE* log_file, const tree_t* tree, node_t* node, node_t* deleted_node);
static void creatRibs (FILE* log_file, const tree_t* tree, node_t* node);
static bool checkDeleted (node_t* node, node_t* deleted_node);
static bool IsNodeInSubtree (node_t* root, node_t* node);

FILE* creatDotFile (const tree_t* tree, int count_log_files, node_t* deleted_node)
{
    FILE* log_file = openLogFile(count_log_files);

    treeVerify(tree, __FILE__, __func__, __LINE__);
    assert(log_file);

    fprintf(log_file, "digraph tree {\n");

    creatStartGraph(log_file);
    creatMainNodes(log_file, tree, tree->root, deleted_node);
    creatRibs(log_file, tree, tree->root);

    fprintf(log_file, "}\n");
    fclose(log_file);

    return log_file;
}

void creatLogPicture (FILE* log_file, const tree_t* tree, int count_log_files)
{
    assert(log_file && tree);

    char command[150] = {};

    snprintf(command, sizeof(command), "dot dots/logFile_%d.txt -T png -o images/logFile_%d.png", count_log_files, count_log_files);

    if (system(command) != 0)
    {
        fprintf(stderr, COLOR_BRED "failed to creat dot picture number %d!!!" COLOR_RESET "\n", count_log_files);
    }

    assert(log_file && tree);

    return;
}

FILE* openLogFile (int count_log_files)
{
    char filename[50] = {};
    snprintf(filename, sizeof(filename), "dots/logFile_%d.txt", count_log_files);
    FILE* log_file = fopen(filename, "w");

    assert(log_file);

    return log_file;
}

void creatStartGraph (FILE* log_file)
{
    assert(log_file);

    fprintf(log_file, "\trankdir = TB;\n");
    fprintf(log_file, "\tsplines = true;\n");
    fprintf(log_file, "\tnodesep = 0.25;\n");
    fprintf(log_file, "\tranksep = 0.6;\n");
    fprintf(log_file, "\tbgcolor = \"#cfa8ceff\"");
    fprintf(log_file, "\tnode [shape = Mrecord];\n");

    assert(log_file);

    return;
}

void creatMainNodes (FILE* log_file, const tree_t* tree, node_t* node, node_t* deleted_node)
{
    treeVerify(tree, __FILE__, __func__, __LINE__);
    assert(log_file);

    const char* fill_color = "";
    const char* color      = "";

    if (node == nullptr) return;

    if (checkDeleted(node, deleted_node))
    {
        fill_color = "#ec6871ff";
        color      = "#ffffffff";
    }
    else if (node == tree->root)
    {
        fill_color = "#c95748ff";
        color      = "#ffffffff";
    }
    else if (node->left != nullptr || node->right != nullptr)
    {
        fill_color = "#39a2acff";
        color      = "#ffffffff";
    }

    else
    {
        fill_color = "#C0DCC0";
        color      = "#ffffffff";
    }
    printf("node->object in creatMainNodes = %s\n", node->object);
    if (node->status != OBJECT)
    {
    fprintf(log_file,
                "node%p [shape=Mrecord; style = filled; fillcolor = \"%s\"; color = \"%s\"; label = \"{ %s | { YES | NO } }\"]\n",
                node, fill_color, color, node->object);
    }
    else
    {
        fprintf(log_file,
                "node%p [shape=Mrecord; style = filled; fillcolor = \"%s\"; color = \"%s\"; label = \"{ %s }\"]\n",
                node, fill_color, color,  node->object);
    }

    if (node->left  == nullptr && node->right == nullptr) return;

    creatMainNodes(log_file, tree, node->left,  deleted_node);
    creatMainNodes(log_file, tree, node->right, deleted_node);

    treeVerify(tree, __FILE__, __func__, __LINE__);
    assert(log_file && tree);

    return;
}

void creatRibs (FILE* log_file, const tree_t* tree, node_t* node)
{
    treeVerify(tree, __FILE__, __func__, __LINE__);
    assert(log_file && node);

    if (node->left == nullptr && node->right == nullptr) return;
    if (node->left && node == node->left->father)
    {
        fprintf(log_file, "node%p->node%p[color = \"blue\", dir = \"both\" weight = 50];\n", node, node->left);
        creatRibs(log_file, tree, node->left);
    }
    if (node->right && node == node->right->father)
    {
        fprintf(log_file, "node%p->node%p[color = \"red\", dir = \"both\" weight = 50];\n", node, node->right);
        creatRibs(log_file, tree, node->right);
    }

    treeVerify(tree, __FILE__, __func__, __LINE__);
    assert(log_file && node);

    return;
}

bool checkDeleted (node_t* node, node_t* deleted_node)
{
    assert(node);

    bool isDeleted = false;
    if (deleted_node != nullptr)
    {
        if (node == deleted_node)
        {
            isDeleted = true;
        }
        else
        {
            isDeleted = IsNodeInSubtree(deleted_node, node);
        }
    }

    assert(node);

    return isDeleted;
}

bool IsNodeInSubtree (node_t* root, node_t* node)
{
    if (root == nullptr) return false;
    if (root == node)    return true;

    return IsNodeInSubtree(root->left, node) || IsNodeInSubtree(root->right, node);
}
