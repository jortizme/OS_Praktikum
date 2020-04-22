#include "include/config.h"


void TypePrompt()
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

void ReadCommandLine(struct Variable_Node *Variable_Head_List)
{
    char buffer[INPUT_SIZE];            
    char *token = NULL;
    char *space[2] = { " ", " " };
    char enter[2] = "\n";                         
    static struct Node *HEAD = NULL;

    fgets(buffer, INPUT_SIZE, stdin);   
    token = strtok(buffer, enter);      //removes the "\n" at the end of the line

    if(token == NULL)
    {
        printf("Please enter a command\n");
        return;
    }

    else if (LookForPipe(token) == TRUE)
    {   
        if ((HEAD = SeparateLines(token, HEAD, space)) != NULL)
        {
            ExecutePipe(HEAD);
        }
    }
    else
    {
        if((HEAD = SeparateCmdAndPmt(token, HEAD, space)) == NULL)
            return;
        else
        {
            ExecuteNormalLine(HEAD, Variable_Head_List);
        }
    }

    while( HEAD != NULL )
    {
        HEAD = DeleteLineList(HEAD);
    } 
}

void ExecuteNormalLine(struct Node* HEAD, struct Variable_Node *Variable_Head_List)
{
    int status;
    pid_t pid,pid_child;
    char space[2] = " ";
    char *command[4];
   

     //words[0]=>command,words[1]=>parameter , words[2]=>parameter
    char **words = GetLineInfo(HEAD,1); //1 beause there is just one line 
    AssignWords(words, command, space);

   //Build-in commands
    if(strcmp(command[0],"exit") == 0)
    {
        while(Variable_Head_List != NULL)
        {
            Variable_Head_List = DeleteVariableList(Variable_Head_List);
        }
        printf("exiting ...\n");
        exit(EXIT_SUCCESS);

    }

    else if(strcmp(command[0],"cd") == 0)
    {
        if(command[1] == NULL)
        {
            if(chdir(getenv("HOME")) < 0)
            {
                perror("Failed finding /home\n");  
            }
        }
            
        else if(chdir(command[1]) < 0)
        {
            perror("Directory not found\n");
        }
    }

    else if(strcmp(command[0],"export") == 0)
    {
       if(LookForAssignment(command[1]) == TRUE)
       {    
            char *envnam = strtok(command[1], "=");
            command[1] = strtok(NULL, "=");

            if ((IsVariable(command[1])) == TRUE)
            {
                if((GetVariableValue(command[1])) == NULL)
                    return ;     
            }
            else
            {
                GetStringAssignment(envnam,command[1]);
            }
            setenv(envnam,command[1],1);
            AddVariableToList(Variable_Head_List,envnam);
            printf("%s was given the value: %s\n",envnam , getenv(envnam));         
       }
    }

    else if (strcmp(command[0],"set") == 0)
    {
        struct Variable_Node *aux1 = Variable_Head_List;
        char *variable = NULL;
        
        while( aux1 != NULL)
        {
            variable = aux1->var;
            if (strcmp(variable,"SHELL") == 0)
            {
                setenv(variable,"Praktikum-Shell",1);
                printf("%s=%s\n",variable,getenv(variable));
            }
            else
                printf("%s=%s\n",variable,getenv(variable));
            
            aux1= aux1->next;
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
void ExecutePipe(struct Node* HEAD)
{
    int status1, status2;
    pid_t child1,child2;
    char space[2] = " ";
    char *command1[3];
    char *command2[3];
    int fdes[2];

     //words[0]=>command,words[1]=>parameter 
    char **words1 = GetLineInfo(HEAD,1); //1 beause there is just one line
    char **words2 = GetLineInfo(HEAD,2);
    AssignWords(words1, command1, space);
    AssignWords(words2, command2, space);
   
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

