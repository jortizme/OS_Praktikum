#ifndef LIST_H 
    #define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 struct Node
{
    int position;       //to numerate the number of commands (if pipelining)
    char *info[3];      //info[0]=>command, info[1]=>parameter,info[2]=>parameter
    struct Node *next;  //points to the next command line"
};

struct Variable_Node                //new list to store global variables
{
    char *var;                  //max. 32 variables
    struct Variable_Node *next;
};

struct Node* add_to_list( struct Node *begin, char **cmd );
struct Node* delete_list( struct Node *begin );
char** get_information ( struct Node *begin, int stelle);
struct Variable_Node* add_to_variable_list(struct Variable_Node *begin, char *variable);
struct Variable_Node* delete_variable_list(struct Variable_Node *begin);

#endif
