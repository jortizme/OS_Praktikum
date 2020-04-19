#include "line_modification.h"

int look_for_variable(const char *pmt)
{
    if ( pmt[0] == '$' )
        return TRUE;
    else
        return FALSE;    
}

char* get_var_value(char *pmt)
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

struct Node * separate_cmd_pmt(char* token, struct Node *HEAD, char *space)
{
    int cnt = 0;
    char *words[2] = {NULL, NULL};
   
    token = strtok(token, space);
       
    while ( token != NULL && cnt < 2 )  //only 2 words are allowed 
    {
        if( cnt == 0)
            words[0] = token;
                
        else if ( cnt == 1 )
            words[1] = token;
                       
        token = strtok(NULL, space);
        ++cnt;
    } 

    if( words[1] == NULL )
        words[1] = space;

    //check if the parameter is a variable, if it is get the value
    //and overwrite the initial parameter value     
    if( (look_for_variable(words[1])) == TRUE)                                           
    {
        if ((get_var_value(words[1]) == NULL))
            return NULL;
    }

    HEAD = add_to_list(HEAD, words);

    return HEAD;
}

int loof_for_pipe(char *line)
{
    char *aux_line = line;
    while (*aux_line != '|' && *aux_line != '\0')
    {
        aux_line++;
        if(*aux_line == '|')
            return TRUE; 
    }
   return FALSE;  
}

struct Node * separate_lines( char* zeile, struct Node *HEAD, char *space)
{    
   char *aux = zeile;
   char *before_aux = aux - 1;
   char *after_aux = aux + 1;
   char *kopf_zeile = aux;

   while( *aux != '\0')
   {
       if( *aux == '|' )
       {
            *before_aux = '\0';
            if ((HEAD = separate_cmd_pmt(kopf_zeile,HEAD ,space)) == NULL)
                return NULL;
            aux = after_aux + 1;
            after_aux = aux + 1;
            before_aux = aux - 1;
            kopf_zeile = aux;
       }
       else if (*after_aux == '\0')
       {
            if ((HEAD = separate_cmd_pmt(kopf_zeile,HEAD, space)) == NULL)
                return NULL;    
       }
       
       after_aux++;
       aux++;
       before_aux++;
   }

    return HEAD;
}