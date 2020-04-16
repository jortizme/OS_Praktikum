#ifndef LIST_H 
    #define LIST_H

#include <stdio.h>
#include <stdlib.h>

 struct Node
{
    int position;       //to numerate the number of commands (if pipelining)
    char *info[2];      //info[0]=>command, info[1]=>parameter
    struct Node *next;  //points to the next command line"
};

struct Node* add_to_list( struct Node *begin, char **cmd );
struct Node* delete_list( struct Node *begin );
char** get_information ( struct Node *begin, int stelle);

#endif
