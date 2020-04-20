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
    char *global_variabels[32] = {"DISPLAY","HOME","PATH",
                                "LOGNAME","PPID","PWD",
                                "SHELL","USER","USERNAME",NULL};

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
            exec_pipe(HEAD);
        }
    }
    else
    {
        if((HEAD = separate_cmd_pmt(token, HEAD, space)) == NULL)
            return;
        else
        {
            exec_normal(HEAD, global_variabels);
        }
    }

    while( HEAD != NULL )
    {
        HEAD = delete_list(HEAD);
    } 
}

void exec_normal(struct Node* HEAD, char **global_variabels)
{
    int status;
    pid_t pid,pid_child;
    char space[2] = " ";
    char *command[3];

     //words[0]=>command,words[1]=>parameter 
    char **words = get_information(HEAD,1); //1 beause there is just one line 
    check_parameter(words, command, space);

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
            perror("Directory not found\n");
            exit(EXIT_FAILURE);
        }
        return;
    }

    else if(strcmp(command[0],"export") == 0)
    {
        char *envnam = strtok(command[1], "=");
        command[1] = strtok(NULL, "=");
        //char *envvalue;

        if ((look_for_variable(command[1])) == TRUE)
        {
            if((get_var_value(command[1])) == NULL)
                return ;   

            //envvalue = command[1];
            setenv(envnam,command[1],1);
           // char *ausgabe = getenv(envnam);
            printf("%s was given the value: %s\n",envnam , getenv(envnam));
        }
        else
        {
            printf("No variable $... recognized...\n");
            return;
        }
    }
    else if (strcmp(command[0],"set") == 0)
    {
        char **aux = global_variabels;
        while(*aux != NULL)
        {
            if(strcmp(*aux,"PPID") == 0)
                printf("%s=%d\n",*aux,getppid());
            else if (strcmp(*aux,"SHELL") == 0)
            {
                setenv(*aux,"Praktikum-Shell",1);
                printf("%s=%s\n",*aux,getenv(*aux));
            }
            else
                printf("%s=%s\n",*aux,getenv(*aux));
            aux++;
        }
        return;
    }
    
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
    int status1, status2;
    pid_t child1,child2;
    char space[2] = " ";
    char *command1[3];
    char *command2[3];
    int fdes[2];

     //words[0]=>command,words[1]=>parameter 
    char **words1 = get_information(HEAD,1); //1 beause there is just one line
    char **words2 = get_information(HEAD,2);
    check_parameter(words1, command1, space);
    check_parameter(words2, command2, space);
   
//Extern commands
    if (pipe(fdes) == -1)   //create the pipe and initialize fdes[0] and fdes[1]
    {
        perror("pipe\n");
        exit(EXIT_FAILURE);
    }
    if((child1 = fork()) < 0)   //create child1
    {
        perror("Fork 1 failed\n");
        exit(EXIT_FAILURE);
    }   
    if(child1 != 0)         
    {
        if((child2 = fork()) < 0)   //create child2 as parent of child1
        {
            perror("Fork 2 failed\n");
            exit(EXIT_FAILURE);
        }
    }
    if(child1 != 0 && child2 != 0)  //when parent from both
    {
        close(fdes[0]);
        close(fdes[1]);

        if(waitpid(child1, &status1, 0) < 0)    //wait for child1
        {
            perror("waitpid2 failed\n");
            exit(EXIT_FAILURE);
        }
        if(waitpid(child2, &status2, 0) < 0)    //wait for child2
        {
            perror("waitpid2 failed\n");
            exit(EXIT_FAILURE);
        }

    }
    else if(child1 == 0)        //when child1
    {
        
        //int fd1 = dup(fdes[1]);
        if(dup2(fdes[1], STDOUT_FILENO) < 0)   //fdes[1] will be copied into STDOUT and survive execvp
        {
            perror("dup() failed at fd[1]");
            exit(EXIT_FAILURE);
        }
        close(fdes[0]);                  //close the read part of the pipe
        close(fdes[1]);

        if((execvp(words1[0],command1) < 0))
        {
            perror("execv failed");
            exit(EXIT_FAILURE);
        }               
    }
    else if(child2 == 0)        //when child2
    {
       
        //int fd0 = dup(fdes[0]);
        if(dup2(fdes[0], STDIN_FILENO) < 0)    //fdes[0] will be copied into STDIN and survive execvp
        {
            perror("dup() failed at fd[0]");
            exit(EXIT_FAILURE);
        }
        close(fdes[1]);                 //close the write part of the pipe
        close(fdes[0]);
        if((execvp(words2[0],command2) < 0))
        {
            perror("execv failed");
            exit(EXIT_FAILURE);
        }
    }
}



