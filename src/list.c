#include "list.h"

struct Node* AddLineToList( struct Node *begin, char **cmd )
{
    struct Node *item = ( struct Node*)malloc(sizeof(struct Node));
    item->info[0] = cmd[0];
    item->info[1] = cmd[1];
    item->info[2] = cmd[2];

    struct Node *aux1 = begin;
    struct Node *aux2 = NULL;

    if( begin == NULL )    //This means the list is empty
    {
        begin = item;
        begin->next = NULL;
        item->position = 1;
    }

    else
    {
        while( aux1 != 0  )
        {   
            aux2 = aux1;
            aux1 = aux1->next;    //go forward until the last item of the list
        }

        aux2->next = item;
        item->next = aux1;
        item->position = aux2->position + 1;
    }
    return begin;
}
//eine while Schleife machen und diese Funktion
//aufrufen solange begin != NULL ist
struct Node* DeleteLineList( struct Node *begin )                           
{
    struct Node *aux = begin;
    begin = begin->next;
    free(aux);
    return begin;
}

char** GetLineInfo(struct Node *begin, int stelle)
{
    struct Node* aux = begin;

    while ( aux->position != stelle )
    {
        aux = aux->next;
    }

    return aux->info;
}

struct Variable_Node* AddVariableToList(struct Variable_Node *begin, char *variable)
{

    struct Variable_Node *loof_for_variable = begin;

    while(loof_for_variable != NULL)
    {
        if(strcmp(loof_for_variable->var,variable) == 0)
        return begin;

        loof_for_variable = loof_for_variable->next;
    }

    struct Variable_Node *item = (struct Variable_Node*)malloc(sizeof(struct Variable_Node));
    item->var = (char*)malloc(16 * sizeof(char));
    strcpy(item->var,variable);
    struct Variable_Node *aux1 = begin;
    struct Variable_Node *aux2 = NULL;

    if( begin == NULL )    //This means the list is empty
    {
        begin = item;
        begin->next = NULL;     
    }
    else
    {
        while( aux1 != 0  )
        {   
            aux2 = aux1;
            aux1 = aux1->next;    //go forward until the last item of the list
        }

        aux2->next = item;
        item->next = aux1;
    }
    return begin;
}

struct Variable_Node* DeleteVariableList(struct Variable_Node *begin)
{
    struct Variable_Node *aux = begin;
    begin = begin->next;
    free(aux->var);
    free(aux);
    return begin;
}

