#include "list.h"

struct Node* add_to_list( struct Node *begin, char **cmd )
{
    struct Node *item = ( struct Node*)malloc(sizeof(struct Node));
    item->info[0] = cmd[0];
    item->info[1] = cmd[1];
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
struct Node* delete_list( struct Node *begin )                           
{
    struct Node *aux = begin;
    begin = begin->next;
    free(aux);
    return begin;
}

char** get_information(struct Node *begin, int stelle)
{
    struct Node* aux = begin;

    while ( aux->position != stelle )
    {
        aux = aux->next;
    }

    return aux->info;
}