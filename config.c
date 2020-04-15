#include "config.h"


void type_prompt()
{
    long size;
    char *buf;
    char *ptr;

    size = pathconf(".",_PC_PATH_MAX);

    if( (buf = (char *)malloc((size_t)size)) != NULL )
    {
        ptr = getcwd(buf, (size_t)size);
        printf("%s@%s> ",getenv("USER"),ptr);
    }

    free(buf);
}

void read_command()
{
    char buffer[INPUT_SIZE];            
    char path_command[COMMAND_PATH_SIZE];
    char *token = NULL;
    char space[64] = " ";
    char enter[2] = "\n";
    char *words[2] = {space, space};    //words[0] => command, words[1] => parameter
    int cnt = 0;

    strcpy(path_command,"/bin/");
    fgets(buffer, INPUT_SIZE, stdin);   
    token = strtok(buffer, enter);      //removes the "\n" at the end of the line

    if(token == NULL)
    {
        printf("Please enter a command\n");
        return;
    }

    else
    {   
        token = strtok(token, space);
       
        while ( token != NULL && cnt < 2 )  //only 2 words are allowed at the moment
        {
            if( cnt == 0)
                words[cnt] = token;
                
            else if ( cnt == 1 )
                words[cnt] = token;
                       
            token = strtok(NULL, space);
            ++cnt;
        }
    }

    strcat(path_command,words[0]);

//check if the parameter is a variable, if it is get the value
//and overwrite the initial parameter value

    if( (look_for_variable(words[1])) == TRUE)                                           
    {
        if ((get_var_value(words[1]) == NULL))
            return;
    }

    exec_cmd(path_command,words[0],words[1]);
}

void exec_cmd(const char *path_command, char *cmd,  char *pmt)
{
    int status;
    pid_t pid,pid_child;

    char *command[] = {cmd, pmt, (char*)NULL};

//Build-in commands

    if(strcmp(command[0],"exit") == 0)
    {
        printf("exiting ...\n");
        exit(EXIT_SUCCESS);

    }

    else if(strcmp(command[0],"cd") == 0)
    {
        if(chdir(command[1]) < 0)
        {
            perror("Directory could not be changed ");
            return;
        }

    }

    else if( (strcmp(command[0],"export") == 0) 
            || (strcmp(command[0],"set") == 0))
    {
        char *envnam = strtok(command[1], "=");
        command[1] = strtok(NULL, "=");
        char *envvalue;

        if ((look_for_variable(command[1])) == TRUE)
        {
            if((get_var_value(command[1])) == NULL)
                return;   
        }
        envvalue = command[1];
        setenv(envnam,envvalue,1);

        char *ausgabe = getenv(envnam);
        printf("%s was given the value: %s\n",envnam , ausgabe);
    }

//Extern commands

    else
    {

        pid = fork();

        if (pid < 0)
        {
            perror("Fork failed\n");
            exit(EXIT_FAILURE);
        }

        if(pid != 0)
        {
            pid_child = waitpid(pid, &status, 0);
            if (pid_child < 0)
            {
                perror("waitpid failed\n");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            if((execv(path_command,command) < 0))
            {
                perror("execv failed");
                exit(EXIT_FAILURE);
            }
        }   
    }
}

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
        perror("Variable doesn't exist...\n");
        return NULL;
    }
}