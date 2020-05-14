#include "include/list.h"


Node* AddLineToList( Node *begin, char **cmd )
{
    Node* item = (Node*)malloc(sizeof(Node));

    for(int i = 0; i < MAX_LINE_AMOUNT; i++)
    {
        item->info[i] = NULL;
    }

    for(int i = 0; i < MAX_LINE_AMOUNT && cmd[i] != NULL; i++ )
    {
        item->info[i] = cmd[i];
    }
    
    Node *aux1 = begin;
    Node *aux2 = NULL;

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
Node* DeleteLineList( Node *begin )                           
{
    Node *aux = begin;
    begin = begin->next;
    free(aux);
    return begin;
}

char** GetLineInfo(Node *begin, int stelle)
{
    Node* aux = begin;

    while ( aux->position != stelle )
    {
        aux = aux->next;
    }

    return aux->info;
}

Variable_Node* AddVariableToList(Variable_Node *begin, char *variable)
{

    Variable_Node *loof_for_variable = begin;

    while(loof_for_variable != NULL)
    {
        if(strcmp(loof_for_variable->var,variable) == 0)
        return begin;

        loof_for_variable = loof_for_variable->next;
    }

    Variable_Node *item = (Variable_Node*)malloc(sizeof(Variable_Node));
    item->var = (char*)malloc(16 * sizeof(char));
    strcpy(item->var,variable);
    Variable_Node *aux1 = begin;
    Variable_Node *aux2 = NULL;

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

Variable_Node* DeleteVariableList(Variable_Node *begin)
{
    Variable_Node *aux = begin;
    begin = begin->next;
    free(aux->var);
    free(aux);
    return begin;
}

