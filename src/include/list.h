#ifndef LIST_H 
    #define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_AMOUNT 6

 struct Node
{
    int position;       //to numerate the number of commands (if pipelining)
    char* info[MAX_LINE_AMOUNT];      //info[0]=>command, info[1]=>parameter,info[2]=>parameter
    struct Node *next;  //points to the next command line"
};

struct Variable_Node                //new list to store global variables
{
    char *var;                  //max. 32 variables
    struct Variable_Node *next;
};

struct Node* AddLineToList( struct Node *begin, char **cmd );
struct Node* DeleteLineList( struct Node *begin );
char** GetLineInfo( struct Node *begin, int stelle);
struct Variable_Node* AddVariableToList(struct Variable_Node *begin, char *variable);
struct Variable_Node* DeleteVariableList(struct Variable_Node *begin);

#endif
