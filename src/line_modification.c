#include "include/line_modification.h"

int IsVariable(const char *pmt)
{
    if ( pmt[0] == '$' )
        return TRUE;
    else
        return FALSE;
}

int IsExecutableProgramm(const char *cmd)
{
    if(cmd[0] =='.')
        return TRUE;
    else
        return FALSE;
}


void GetStringAssignment(char* name, char* pmt)
{
    char* aux1 = pmt;
    char* aux2 = aux1;

    while(*aux2 != '\0')
    {
        if(*aux2 == '=')
        {
            *aux2 ='\0';
            name = aux1;
            pmt = aux2 + 1;
            return;
        
        }
        aux2++;
    }
}

char* GetVariableValue(char *pmt)
{
    char *varname = pmt;
    varname = varname + 1;
    char *varvalue = getenv(varname);
    if(varvalue != NULL)
    {
        strcpy(pmt, varvalue);
        return pmt;
    }
    else
    {
        printf("Variable doesn't exist...\n");
        return NULL;
    }
}

struct Node* SeparateCmdAndPmt(char* token, struct Node *HEAD, char **space)
{
    int cnt = 0;
    char *words[MAX_LINE_AMOUNT] = {NULL, NULL, NULL, NULL, NULL, NULL};
    char *exec = NULL;
   
    token = strtok(token, space[0]);
    
    for(int i = 0; i < MAX_LINE_AMOUNT && token != NULL; i++ )
    {
        words[i]  = token;
        token = strtok(NULL, space[0]);
    }

    //check if the parameter is a variable, if it is get the value
    //and overwrite the initial parameter value 
    if(IsVariable(words[0]) == TRUE)
    {
        if ((GetVariableValue(words[0]) == NULL))
            return NULL;
    }
    else if (IsExecutableProgramm(words[0]) == TRUE)
    {
        exec = realpath(words[0], NULL);
        strcpy(words[0],exec);
    }
    
    for( int i = 1; i < MAX_LINE_AMOUNT && words[i] != NULL; i++)
    {
        if( (IsVariable(words[i])) == TRUE)                                           
        {
            if ((GetVariableValue(words[i]) == NULL))
                return NULL;
        }
    }

    HEAD = AddLineToList(HEAD, words);

    return HEAD;
}

struct Node * SeparateLines( char* zeile, struct Node *HEAD, char **space)
{    
   char *aux = zeile;
   char *before_aux = aux - 1;
   char *after_aux = aux + 1;
   char *kopf_zeile = aux;
   int cnt = 0;

   while( *aux != '\0')
   {
       if( *aux == '|' )
       {    
           cnt++;
           if(cnt > 1)
           {
               printf("Only one pipe is allowed\n");
                while( HEAD != NULL )
                {
                    HEAD = DeleteLineList(HEAD);
                } 
               return NULL;
           }
            *before_aux = '\0';
            if ((HEAD = SeparateCmdAndPmt(kopf_zeile,HEAD ,space)) == NULL)
                return NULL;
            aux = after_aux + 1;
            after_aux = aux + 1;
            before_aux = aux - 1;
            kopf_zeile = aux;
       }
       else if (*after_aux == '\0')
       {
            if ((HEAD = SeparateCmdAndPmt(kopf_zeile,HEAD, space)) == NULL)
                return NULL;    
       }
       
       after_aux++;
       aux++;
       before_aux++;
   }

    return HEAD;
}

void AssignWords(char **words, char **command, char *space)
{
    int i;

    for(i = 0; i < MAX_LINE_AMOUNT && words[i] != NULL; i++)
    {
        command[i] = words[i];
    }

    command[i] = NULL;
}