#ifndef LIST_H 
    #define LIST_H

#include "config.h"

#define MAX_LINE_AMOUNT 6
#define MAX_NR_PIPES    2

typedef struct Node
{
    int position;                      //to numerate the number of commands (if pipelining)
    char* info[MAX_LINE_AMOUNT];      //info[0]=>command, info[1]=>parameter,info[2]=>parameter
    struct Node *next;                //points to the next command line"
}Node;

typedef struct Variable_Node         //new list to store global variables
{
    char *var;                       
    struct Variable_Node *next;
}Variable_Node;

Node* AddLineToList( Node *begin, char **cmd );
Node* DeleteLineList( Node *begin );
char** GetLineInfo( Node *begin, int stelle);
Variable_Node* AddVariableToList(Variable_Node *begin, char *variable);
Variable_Node* DeleteVariableList(Variable_Node *begin);

#endif
