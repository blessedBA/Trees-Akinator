#include "akinator.h"
#include "errors.h"
#include "filesFuncs.h"
#include "safetyTree.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BAD_VALUE -666

void startAkinator (tree_t* tree)
{
    treeVerify(tree, __FILE__, __func__, __LINE__);

    int global_code_error = 0;
    int count_dumps = 0;
    int answer = 0;

    printf("Hello, this is Akinator. It will guess the person whom you will make a wish)\n\n");
    usleep(3e5);

    do
    {
        printStartOfAkinator();
        while (!checkStartAnswer(&answer)) printf("please, enter just one symbol\n");

        switch (answer)
        {
            case GUESS_OBJ:
                guessObject(tree, tree->root, &count_dumps);
                break;
            case ADD_OBJ:
                printf("before adding new object we need to try to guess it :) \n");
                guessObject(tree, tree->root, &count_dumps);
                break;
            case DUMP_TREE:
                //showTree(tree);
                ++count_dumps;
                treeDump(tree, __FILE__, __func__, __LINE__, global_code_error, count_dumps, nullptr, "DUMP %d", count_dumps - 1);
                break;
            case GIVE_DEF:
                giveDefinition(tree);
                break;
            case DELETE:
                startDeletingElement(tree);
                break;
            case WRITE:
                startWriting(tree);
                break;
            case READ:
                startReading(tree);
                break;
            case EXIT:
                printf("GOODBYE MEOW\n");
                break;
            default:
                assert(0 && "fucking shit something went wrong...(invalid answer in function startAkinator");
        }

    }
    while (answer != 'q');


    treeVerify(tree, __FILE__, __func__, __LINE__);

    return;
}

isError_t guessObject (tree_t* tree, node_t* current_node, int* count_dumps)
{
    treeVerify(tree, __FILE__, __func__, __LINE__);

    if (current_node->left  == nullptr && current_node->right == nullptr)
    {
        checkFinalAnswerFromPerson(tree, current_node, count_dumps);
        return NO_ERRORS;
    }

    checkAnswer(current_node);

    switch (current_node->status)
    {
        case ANS_NULL:
            fprintf(stderr, "something went wrong in %s %s:%d ...\n", __FILE__, __func__, __LINE__);
            return HAVE_ERROR;
            break;
        case OBJECT:
            fprintf(stderr, "something went wrong in %s %s:%d ...\n", __FILE__, __func__, __LINE__);
            return HAVE_ERROR;
            break;
        case ANS_YES:
            if (guessObject(tree, current_node->left, count_dumps))  return HAVE_ERROR;
            break;
        case ANS_NO:
            if (guessObject(tree, current_node->right, count_dumps)) return HAVE_ERROR;
            break;
        default:
            assert(0 && "holy shit, your tree is definitely WRONG");
    }

    treeVerify(tree, __FILE__, __func__, __LINE__);

    return NO_ERRORS;
}

isError_t addObject (tree_t* tree, node_t* node)
{
    treeVerify(tree,  __FILE__, __func__, __LINE__);
    assert(node);

    printf("do you want to add a new object?\n");
    char answer[10] = "";
    bool good_answer = false;

    while (!good_answer && scanf("%*s", sizeof(answer), answer))
    {
        if (strcmp(answer, "no")  == 0 || strcmp(answer, "NO")  == 0) return NO_ERRORS;
        if (strcmp(answer, "yes") != 0 && strcmp(answer, "YES") != 0)
        {
            printf("please, answer \"no/NO\" or \"yes/YES\"\n");
            continue;
        }
        good_answer = true;
    }

    char* new_object = (char*)calloc(30, sizeof(char));
    //printf("node->object = %s\n", node->object);
    printf("print new object: ");

    // skipString(stdin);
    // fgets(new_object, sizeof(new_object), stdin);
    scanf("%s", new_object);
    printf("How is it different from %s? It ", node->object);

    char* new_question = (char*)calloc(100, sizeof(char));
    // skipString(stdin);
    // fgets(new_question, sizeof(new_question), stdin);
    scanf("%s", new_question);

    //printf("new_question = %s\n", new_question);
    if (node == tree->root)
    {
        char* new_right_object = (char*)calloc(30, sizeof(char));
        printf("also, I need the second object with non %s. It is ", new_question);
        scanf("%s", new_right_object);
        node->object = nullptr;
        nodeInit(tree, node, RIGHT, new_right_object, new_question);
    }
    else nodeInit(tree, node, RIGHT, node->object, new_question);

    nodeInit(tree, node, LEFT, new_object, new_question);
    node->status = ANS_YES;
    node->object = new_question;

    //printf("new_object = %s\n", new_object);
    //printf("node->object = %s\n", node->object);

    treeVerify(tree,  __FILE__, __func__, __LINE__);
    assert(node);

    return NO_ERRORS;
}

void giveDefinition (const tree_t* tree)
{
    treeVerify(tree, __FILE__, __func__, __LINE__);

    char object[30] = "";
    printf("\ndefinition of which object do you want to know?\n");
    scanf("%s", object);

    node_t* founded_node = (node_t*)calloc(1, sizeof(node_t));
    assert(founded_node);
    founded_node = findNode(tree, tree->root, object);

    if (founded_node == nullptr) printf("\nthere is no such object in akinator data(\n");

    printf("%s is ", object);
    definitionInit(founded_node);

    printf("thats all :)\n\n");

    treeVerify(tree, __FILE__, __func__, __LINE__);

    return;
}

void definitionInit (const node_t* current_node)
{

    if (current_node->father == nullptr) return;

    if (current_node->father->left  == current_node) printf("%s, ",     current_node->father->object);
    if (current_node->father->right == current_node) printf("not %s, ", current_node->father->object);

    definitionInit(current_node->father);

    return;
}

void startDeletingElement (tree_t* tree)
{
    assert(tree);

    char deleting_object[30] = "";
    printf("which element do you want to delete? ");
    scanf("%s", deleting_object);

    node_t* founded_node = (node_t*)calloc(1, sizeof(node_t));
    assert(founded_node);
    founded_node = findNode(tree, tree->root, deleting_object);

    if (founded_node == nullptr) printf("\nthere is no such object in akinator data(\n");

    printf("deleting.");
    usleep(5e5);
    printf(".");

    nodeDestroy(tree, founded_node, 1);

    usleep(5e5);
    printf(".");

    printf("deleting was done!\n");

    assert(tree);

    return;
}

bool checkStartAnswer(int* answer)
{
    assert(answer);

    char temp = 0;
    skipString(stdin);
    scanf("%c", &temp);

    switch (temp)
    {
        case GUESS_OBJ:
            *answer = GUESS_OBJ;
            break;
        case ADD_OBJ:
            *answer = ADD_OBJ;
            break;
        case DUMP_TREE:
            *answer = DUMP_TREE;
            break;
        case GIVE_DEF:
            *answer = GIVE_DEF;
            break;
        case DELETE:
            *answer = DELETE;
            break;
        case WRITE:
            *answer = WRITE;
            break;
        case READ:
            *answer = READ;
            break;
        case EXIT:
            *answer = 'q'; // 'q' = \113
            break;
        default:
            *answer = BAD_VALUE;
    }

    assert(answer);

    return (*answer == BAD_VALUE) ? false : true;
}

void checkAnswer (node_t* current_node)
{

    bool isAnswerRight = false;
    char answer[10] = "";

    printf("%s?\n", current_node->object);

    do
    {
        scanf("%s", answer);

        if (strcmp(answer, "YES") == 0 || strcmp(answer, "yes") == 0)
        {
            current_node->status = ANS_YES;
            isAnswerRight = true;
        }
        if (strcmp(answer, "NO") == 0 || strcmp(answer, "no") == 0)
        {
            current_node->status = ANS_NO;
            isAnswerRight = true;
        }
    }
    while(isAnswerRight == false && printf("please, answer \"no/NO\" or \"yes/YES\"\n"));


    return;
}

void checkFinalAnswerFromPerson (tree_t* tree, node_t* answer_node, int* count_dumps)
{
    treeVerify(tree, __FILE__, __func__, __LINE__);
    assert(answer_node);

    char answer[5] = "";
    bool isAnswerRight = false;

    printf("IS YOUR ANSWER %s?\n", answer_node->object);

    do
    {
        scanf("%s", answer);

        if (strcmp(answer, "YES") == 0 || strcmp(answer, "yes") == 0)
        {
            printf("thanks for work :)\n");
            isAnswerRight = true;
        }
        else if (strcmp(answer, "NO") == 0 || strcmp(answer, "no") == 0)
        {
            printf("((((\n\n");
            isAnswerRight = true;
            treeDump(tree, __FILE__, __func__, __LINE__, NULL, ++*count_dumps, nullptr, "BEFORE %dth ADDING", *count_dumps);
            addObject(tree, answer_node);
            treeDump(tree, __FILE__, __func__, __LINE__, NULL, ++*count_dumps, nullptr, "AFTER %dth ADDING ", *count_dumps - 1);
        }
    }
    while(isAnswerRight == false && printf("please, answer \"no/NO\" or \"yes/YES\"\n"));

    treeVerify(tree, __FILE__, __func__, __LINE__);

    return;
}

void skipString (FILE* input_file)
{
    assert(input_file);

    int ch = 0;
    if ((ch = fgetc(input_file)) == '\n')
    {
        //ungetc(ch, input_file);
        return;
    }
    while ((ch = fgetc(input_file)) != '\n' && ch != EOF)
        ;

    assert(input_file);

    return;
}

void printStartOfAkinator ()
{
    printf("Please, choose an action: \n");
    printf("1. guess a person\n");
    printf("2. add a person\n");
    printf("3. show tree of akinator\n");
    printf("4. get a definition of object\n");
    printf("d. delete object or node\n");
    printf("w. write tree of akinator in file\n");
    printf("r. read tree from file\n");
    printf("q. exit\n");

    return;
}

node_t* findNode (const tree_t* tree, node_t* current_node, const char* object)
{
    treeVerify(tree, __FILE__, __func__, __LINE__);
    assert(object);

    node_t* required_node = nullptr;

    if (current_node->left)
    {
        if ((required_node = findNode(tree, current_node->left,  object))) return required_node;
    }
    if (current_node->right)
    {
        if ((required_node = findNode(tree, current_node->right, object))) return required_node;
    }

    if (strcmp(current_node->object, object) == 0) return current_node;

    treeVerify(tree, __FILE__, __func__, __LINE__);
    assert(object);

    return nullptr;
}

// void showTree(tree_t* tree)
// {
//     treeVerify(tree, __FILE__, __func__, __LINE__);
//
//     FILE* log_file = creatDotFile(tree, count_log_files, deleted_node);
//     creatLogPicture(log_file, tree, count_log_files);
//
//     treeVerify(tree, __FILE__, __func__, __LINE__);
//
//     return;
// }
