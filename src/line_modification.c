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

int LookForAssignment(const char *pmt)
{
    int cnt = 0;

    while(pmt[cnt] != '\0')
    {
        if(pmt[cnt] == '=')
            return TRUE;

        cnt++;
    }

    printf("There is no assignment '=' ...\n");
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
    char *words[3] = {NULL, NULL, NULL};
    char *exec = NULL;
   
    token = strtok(token, space[0]);
       
    while ( token != NULL && cnt < 3 )  //only 2 words are allowed 
    {
        if( cnt == 0)
            words[0] = token;
                
        else if ( cnt == 1 )
            words[1] = token;
        
        else if ( cnt == 2 )
            words[2] = token;
                       
        token = strtok(NULL, space[0]);
        ++cnt;
    } 

    if( words[1] == NULL )
        words[1] = space[0];

    if (words[2] == NULL)
        words[2] = space[1];
   
    

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
        
    if( (IsVariable(words[1])) == TRUE)                                           
    {
        if ((GetVariableValue(words[1]) == NULL))
            return NULL;
    }

    if( (IsVariable(words[2])) == TRUE)                                           
    {
        if ((GetVariableValue(words[2]) == NULL))
            return NULL;
    }


    HEAD = AddLineToList(HEAD, words);

    return HEAD;
}

int LookForPipe(const char *line)
{
    const char *aux_line = line;
    while (*aux_line != '|' && *aux_line != '\0')
    {
        aux_line++;
        if(*aux_line == '|')
            return TRUE; 
    }
   return FALSE;  
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
    if (strcmp(words[1],space) == 0)
    {
        command[0] = words[0];
        command[1] = NULL;
    }
    else if (strcmp(words[2],space) == 0)
    {
        command[0] = words[0];
        command[1] = words[1];
        command[2] = NULL;
    }
    else{
        command[0] = words[0];
        command[1] = words[1];
        command[2] = words[2];
        command[3] = NULL;
    }
}