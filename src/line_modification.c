#include "include/line_modification.h"
#include "include/config.h"

int IsVariable(const char *pmt)
{
    if ( pmt[0] == '$' )
        return true;
    else
        return false;
}

int IsExecutableProgramm(const char *cmd)
{
    if(cmd[0] =='.')
        return true;
    else
        return false;
}


void GetStringAssignment(char* name, char* pmt)
{
    char* AssignmentPos = strchr(pmt,'=');

    *AssignmentPos = '\0';

    name = pmt;
    pmt = ++AssignmentPos;

}

char* GetVariableValue(char *pmt)
{
    char* varvalue;

    if((varvalue = getenv(++pmt)) != NULL)
    {
        --pmt;
        strcpy(pmt, varvalue);
        return pmt;
    }
    else
    {
        printf("Variable doesn't exist...\n");
        return NULL;
    }
}

Node* SeparateCmdAndPmt(char* token, Node *HEAD)
{
    char *words[MAX_LINE_AMOUNT] = {NULL, NULL, NULL, NULL, NULL, NULL};

    token = strtok(token, " ");
    
    for(int i = 0; i < MAX_LINE_AMOUNT && token != NULL; i++ )
    {
        words[i]  = token;
        token = strtok(NULL, " ");
    }

    //check if the parameter is a variable, if it is get the value
    //and overwrite the initial parameter value 
    if(IsVariable(words[0]) == true)
    {
        if ((GetVariableValue(words[0]) == NULL))
            return NULL;
    }
    else if (IsExecutableProgramm(words[0]) == true)
    {
        strcpy(words[0],realpath(words[0], NULL));
    }
    
    for( int i = 1; i < MAX_LINE_AMOUNT && words[i] != NULL; i++)
    {
        if( (IsVariable(words[i])) == true)                                           
        {
            if ((GetVariableValue(words[i]) == NULL))
                return NULL;
        }
    }

    HEAD = AddLineToList(HEAD, words);

    return HEAD;
}

Node * SeparateLines( char* zeile, Node *HEAD)
{    
    char* PipePos = strchr(zeile, '|');
    char* AnotherPipe = strrchr(zeile, '|');

    if( AnotherPipe != NULL && AnotherPipe != PipePos)
    {
        printf("Only one pipe is allowed\n");
        return NULL;
    }

    *(--PipePos) = '\0';

    for(int i = 0; i < MAX_NR_PIPES; i++)
    {
        if ((HEAD = SeparateCmdAndPmt(zeile,HEAD)) == NULL)
            return NULL;

        zeile = PipePos + 3;
    }

    return HEAD;
}

int AssignWords(char **words, char **command)
{
    int i;
    int NrArguments = 0;

    for(i = 0; i < MAX_LINE_AMOUNT && words[i] != NULL; i++)
    {
        command[i] = words[i];
            NrArguments++;
    }
    command[i] = NULL;

    return NrArguments;
}