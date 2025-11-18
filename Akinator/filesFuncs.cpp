#include "filesFuncs.h"
#include "Files.h"
#include "tree.h"
#include "safetyTree.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>


void startWriting(tree_t* tree)
{
    treeVerify(tree, __FILE__, __func__, __LINE__);

    printf("writing.");
    usleep(1e5);
    printf(".");
    usleep(1e5);
    printf(".\n");

    FILE* output_file = fopen("textDumpAkinator.txt", "w");
    assert(output_file);

    writeInFile(tree, tree->root, output_file);

    printf("writing have done :)\n");

    fclose(output_file);

    treeVerify(tree, __FILE__, __func__, __LINE__);

    return;
}

isError_t writeInFile (tree_t* tree, node_t* node, FILE* output_file)
{
    treeVerify(tree, __FILE__, __func__, __LINE__);
    assert(output_file);

    fprintf(output_file, "( ");

    fprintf(output_file, "\"%s\" ", node->object);
    if (node->left)  writeInFile(tree, node->left,  output_file);
    if (node->right) writeInFile(tree, node->right, output_file);

    if (node->left == nullptr && node->right == nullptr) fprintf(output_file, " nil nil ");
    fprintf(output_file, ") ");

    treeVerify(tree, __FILE__, __func__, __LINE__);
    assert(output_file);

    return NO_ERRORS;
}

node_t* startReading(tree_t* tree)
{
    treeVerify(tree, __FILE__, __func__, __LINE__);

    printf("reading.");
    usleep(5e5);
    printf(".");
    usleep(5e5);
    printf(".\n\n");

    const char* filename = TEXT_DUMP;

    char* buffer = creatBuffer(filename);
    printf("%s", buffer);
    size_t position = 0;
    tree->root = readTreeFromFile(buffer, &position, &tree->size, nullptr);

    printf("reading have done :)\n");

    treeVerify(tree, __FILE__, __func__, __LINE__);

    return tree->root;
}

char* creatBuffer (const char* filename)
{
    assert(filename);

    FILE* input_file = fopen(filename, "r");
    assert(input_file);

    struct stat st = {};
    const int get_info = (int)stat(filename, &st);
    const ssize_t size_file = st.st_size;
    if (get_info != 0)  assert(0 && "failed to get info about file");

    char* buffer = (char*)calloc(size_file + 2, sizeof(char));
    if (fread(buffer, sizeof(char), size_file, input_file) != size_file)
    {
        fprintf(stderr, "failed to read file to buffer\n");
        return nullptr;
    }

    assert(filename);

    return buffer;
}

node_t* readTreeFromFile(char* buffer, size_t* position, size_t* size, node_t* parent_node)
{
    assert(position && size);

    node_t* node = nullptr;
    skipSpacesInBuffer(buffer, position);
    printf("buffer[position] = %c\n", buffer[*position]);
    if (buffer[*position] == '(')
    {
        printf("if ACTIVATED\n");
        (*size)++;
        (*position)++;
        node = creatNode();
        node->father = parent_node;
    }
    printf("position [%p]\n", position);
    printf("position in %s = %zu\n", __func__, *position);

    usleep(1e5);

    writeInfoInNode(buffer, position, node);
    printf("\nnode [%p]\n", node);

    if (checkNil(buffer, position))
    {
        skipSpacesInBuffer(buffer, position);
        checkNil(buffer, position);
        skipSpacesInBuffer(buffer, position);
        if (buffer[*position] != ')') assert(0);
        printf("PENIS\n");
        (*position)++; // skip ')'
        node->status = OBJECT;
        return node;
    }
    node->left  = readTreeFromFile(buffer, position, size, node);
    node->right = readTreeFromFile(buffer, position, size, node);

    printf("\nnode->left = %p\n", node->left);
    printf("node->right = %p\n\n", node->right);

    skipSpacesInBuffer(buffer, position);
    (*position)++; // skip ')'

    assert(position && size);

    return node;
}

node_t* creatNode()
{
    node_t* node = (node_t*)calloc(1, sizeof(node_t));
    assert(node);
    node->father = nullptr;
    node->left = nullptr;
    node->right = nullptr;
    node->node = node;
    node->status = ANS_NULL;

    return node;
}

isError_t writeInfoInNode (char* buffer, size_t* position, node_t* node)
{
    assert(buffer && position);

    skipSpacesInBuffer(buffer, position);
    printf("position = %zu\n", *position);
    printf("buffer[*position] = %c\n", buffer[*position]);
    if (buffer[*position] != '"') return HAVE_ERROR;
    (*position)++;
    char* object = (char*)calloc(30, sizeof(char));
    sscanf(buffer + *position, "%29[^\"]", object);
    printf("object = %s\n", object);
    *position += strlen(object) + 1; // +1 because of the second "
    skipSpacesInBuffer(buffer, position);

    printf("node->object [%p] in %s\n", node->object, __func__);
    node->object = object;

    assert(buffer && position && node);

    return NO_ERRORS;
}


void skipSpacesInBuffer(char* buffer, size_t* position)
{
    assert(buffer && position);

    char ch = buffer[*position];
    if (ch != ' ') return;
    while ((ch = buffer[(*position)]) == ' ') (*position)++;

    printf("position in skipSpacesInBuffer = %c\n", buffer[(*position)]);
    assert(buffer && position);

    return;
}

bool checkNil (char* buffer, size_t* position)
{
    assert(buffer && position);

    char ch[4] = "";
    sscanf(buffer + *position, "%3s", ch);
    if (strcmp("nil", ch) == 0)
    {
        *position += 3;
        return true;
    }

    assert(buffer && position);

    return false;
}
