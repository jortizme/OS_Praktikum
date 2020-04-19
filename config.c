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
    char space[2] = " ";
    char enter[2] = "\n";                         
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
        if ((HEAD = separate_lines(token, HEAD, space)) != NULL)
        {
            /*  TO DO */
        }
    }
    else
    {
        if((HEAD = separate_cmd_pmt(token, HEAD, space)) == NULL)
            return;
        else
        {
            //1 because if there is no Pipe we muss handle just one Node
            
            exec_normal(HEAD);
        }
    }

    while( HEAD != NULL )
    {
        HEAD = delete_list(HEAD);
    } 
}

void exec_normal(struct Node* HEAD)
{
    int status;
    pid_t pid,pid_child;
    char space[2] = " ";
    char *command[3];

     //words[0]=>command,words[1]=>parameter 
    char **words = get_information(HEAD,1); //1 beause there is just one line 

    if ( strcmp(words[1],space) == 0)
    {
        command[0] = words[0];
        command[1] = NULL;
    }
    else
    {
        command[0] = words[0];
        command[1] = words[1];
        command[2] = NULL;
    }

    int cntrl = build_in_cmd(command);
    if(cntrl == SUCCESS || cntrl == ERROR)  //Was it a built_in command or was there any error 
        return;                             //executing a built-in, then leave the function
    
    else
    {  
    //Extern commands

        pid = fork();

        if (pid < 0)
        {
            perror("Fork failed\n");
            exit(EXIT_FAILURE);
        }

        if(pid != 0)
        {
            pid_child = waitpid(-1, &status, 0);
            if (pid_child < 0)
            {
                perror("waitpid failed\n");
                exit(EXIT_FAILURE);
            }   
        }
        else
        {
            if((execvp(words[0],command) < 0))
            {
                perror("execv failed");
                exit(EXIT_FAILURE);
            }
                      
        } 
    }
}
void exec_pipe(struct Node* HEAD)
{
int status;
    pid_t pid,pid_child;
    char space[2] = " ";
    char *command[3];

     //words[0]=>command,words[1]=>parameter 
    char **words = get_information(HEAD,1); //1 beause there is just one line 

    if ( strcmp(words[1],space) == 0)
    {
        command[0] = words[0];
        command[1] = NULL;
    }
    else
    {
        command[0] = words[0];
        command[1] = words[1];
        command[2] = NULL;
    }

    int cntrl = build_in_cmd(command);
    if(cntrl == SUCCESS || cntrl == ERROR)  //Was it a built_in command or was there any error 
        return;                             //executing a built-in, then leave the function
    
    else
    {  
    //Extern commands

        pid = fork();

        if (pid < 0)
        {
            perror("Fork failed\n");
            exit(EXIT_FAILURE);
        }

        if(pid != 0)
        {
            pid_child = waitpid(-1, &status, 0);
            if (pid_child < 0)
            {
                perror("waitpid failed\n");
                exit(EXIT_FAILURE);
            }   
        }
        else
        {
            if((execvp(words[0],command) < 0))
            {
                perror("execv failed");
                exit(EXIT_FAILURE);
            }
                      
        } 
    }
}


int build_in_cmd(char **cmd)
{  
//Build-in commands
    if(strcmp(cmd[0],"exit") == 0)
    {
        printf("exiting ...\n");
        exit(EXIT_SUCCESS);

    }

    else if(strcmp(cmd[0],"cd") == 0)
    {
        if(chdir(cmd[1]) < 0)
        {
            perror("Directory could not be changed ");
            return ERROR;
        }
        else
        {
            return SUCCESS;
        }
        
    }

    else if(strcmp(cmd[0],"export") == 0)
    {
        char *envnam = strtok(cmd[1], "=");
        cmd[1] = strtok(NULL, "=");
        char *envvalue;

        if ((look_for_variable(cmd[1])) == TRUE)
        {
            if((get_var_value(cmd[1])) == NULL)
                return ERROR;   

            envvalue = cmd[1];
            setenv(envnam,envvalue,1);
            char *ausgabe = getenv(envnam);
            printf("%s was given the value: %s\n",envnam , ausgabe);
            return SUCCESS;
        }
        else
        {
            printf("No variable $... recognized...\n");
            return ERROR;
        }
    }
    return FAILURE;
}