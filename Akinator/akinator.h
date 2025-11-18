#ifndef AKINATOR_H
#define AKINATOR_H

#include "errors.h"
#include "tree.h"

typedef enum mode_T
{
    GUESS_OBJ = '1',
    ADD_OBJ   = '2',
    DUMP_TREE = '3',
    GIVE_DEF  = '4',
    DELETE    = 'd',
    WRITE     = 'w',
    READ      = 'r',
    EXIT      = 'q'
} mode_T;


isError_t guessObject (tree_t* tree, node_t* node, int* count_dumps);
void checkAnswer (tree_t* tree, node_t* current_node);
void checkFinalAnswerFromPerson (tree_t* tree, node_t* answer_node, int* count_dumps);
isError_t addObject (tree_t* tree, node_t* node);
void skipString (FILE* input_file);
void startAkinator (tree_t* tree);
bool checkStartAnswer(int* answer);
void printStartOfAkinator ();
void giveDefinition (tree_t* tree);
node_t* findNode (tree_t* tree, node_t* current_node, const char* object);
void definitionInit (node_t* current_node);
void startDeletingElement (tree_t* tree);



#endif // AKINATOR_H
