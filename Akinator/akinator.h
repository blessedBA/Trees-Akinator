#ifndef AKINATOR_H
#define AKINATOR_H

#include "errors.h"
#include "tree.h"

typedef enum mode_T
{
    GUESS_OBJ = 49,
    ADD_OBJ   = 50,
    DUMP_TREE = 51,
    GIVE_DEF  = 52,
    EXIT      = 113
} mode_T;


isError_t guessObject (tree_t* tree, node_t* node, int* count_dumps);
isError_t checkAnswer (tree_t* tree, node_t* current_node, char* answer);
isError_t checkFinalAnswerFromPerson (tree_t* tree, node_t* answer_node, int* count_dumps);
isError_t addObject (tree_t* tree, node_t* node);
void skipString (FILE* input_file);
void startAkinator (tree_t* tree);
bool checkStartAnswer(int* answer);
void printStartOfAkinator ();
void giveDefinition (tree_t* tree);
node_t* findNode (tree_t* tree, node_t* current_node, const char* object);
void definitionInit (node_t* current_node);
#endif // AKINATOR_H
