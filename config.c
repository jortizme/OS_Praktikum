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
    char *token = NULL;
    char enter[2] = "\n";   
    char **words;                       //words[0]=>path,words[1]=>command,words[2]=>parameter  
    static struct Node *HEAD = NULL;

    fgets(buffer, INPUT_SIZE, stdin);   
    token = strtok(buffer, enter);      //removes the "\n" at the end of the line

    if(token == NULL)
    {
        printf("Please enter a command\n");
        return;
    }

    else if (loof_for_pipe(token) == TRUE)
    {   
        if ((HEAD = separate_lines(token, HEAD)) != NULL)
             printf("ALLES GUT MIT DEN PIPES\n");
       
        /*   Algorith to execute multiple commands*/
        /*                      TO DO                       */
    }
    else
    {
        if((HEAD = separate_cmd_pmt(token, HEAD)) == NULL)
            return;
        else
        {
          words = get_information(HEAD,1);  //1 because if no Pipe there is one Node
          exec_cmd(words[0],words[1]);
        }
    }

    while( HEAD != NULL )
    {
        HEAD = delete_list(HEAD);
    } 
}

void exec_cmd(char *cmd,  char *pmt)
{
    int status;
    pid_t pid,pid_child;
    char prefix[16];
    strcpy(prefix,"/bin/");
    const char *path_command = strcat(prefix,cmd);

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


