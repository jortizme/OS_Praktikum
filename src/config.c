#define CONFIG_CPP
#include "include/config.h"


 static Node* HEAD;
 static Variable_Node* Variable_List;

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

void DefGlobVar()
{

    char *main_global_variables[16] = {"DISPLAY","HOME","PATH",
                                        "LOGNAME","SHELL","USER",
                                        "USERNAME", NULL};
    Variable_List = NULL;
    char **aux = main_global_variables;
    int cnt = 0;

    //Initialization of global variable List
    while (aux[cnt] != NULL)
    {
        Variable_List = AddVariableToList(Variable_List, aux[cnt]);
        cnt++;
    }
    
}

void ReadCommandLine()
{
    char buffer[INPUT_SIZE];            
    char* token = NULL;
    HEAD = NULL;

    fgets(buffer, INPUT_SIZE, stdin);   
    token = strtok(buffer, "\n");      //removes the "\n" at the end of the line

    if(token == NULL)
    {
        printf("Please enter a command\n");
        return;
    }
    else if (strchr(token,'|') != NULL)
    {   
        if ((HEAD = SeparateLines(token, HEAD)) != NULL)
        {
            ExecutePipe();
        }
    }
    else
    {
        if((HEAD = SeparateCmdAndPmt(token, HEAD)) == NULL)
            return;
        else
        {
            ExecuteNormalLine();
        }
    }
    while( HEAD != NULL )
    {
        HEAD = DeleteLineList(HEAD);
    } 
}

void ExecuteNormalLine()
{
    int status;
    pid_t pid,pid_child;
    char* command[MAX_LINE_AMOUNT];
    
     //words[0]=>command,words[1]=>parameter , words[2]=>parameter
    char **words = GetLineInfo(HEAD,1); //1 beause there is just one line 
    
    int NrArguments = AssignWords(words, command);

   //Build-in commands
    if(strcmp(command[0],"exit") == 0)
    {
        while(Variable_List != NULL)
        {
            Variable_List = DeleteVariableList(Variable_List);
        }
        printf("exiting ...\n");
        exit(EXIT_SUCCESS);
    }

    else if (strcmp(command[0],"ls") == 0)
    {   
        exec_ls(NrArguments,command);
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
        if(strchr(command[1],'=') != NULL)
        {    
            char *envnam = strtok(command[1], "=");
            command[1] = strtok(NULL, "=");

            if ((IsVariable(command[1])) == true)
            {
                if((GetVariableValue(command[1])) == NULL)
                    return ;     
            }
            else
            {
                GetStringAssignment(envnam,command[1]);
            }
            setenv(envnam,command[1],1);
            AddVariableToList(Variable_List,envnam);
            printf("%s was given the value: %s\n",envnam , getenv(envnam));         
        }
    }

    else if (strcmp(command[0],"set") == 0)
    {
        Variable_Node *aux1 = Variable_List;
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
    //Extern commands
    else
    {  
        pid = fork();
        ControlReturnNegative("Fork failed\n", pid);

        if(pid != 0)
        {
            pid_child = waitpid(-1, &status, 0);
            ControlReturnNegative("waitpid failed\n", pid_child);
        }
        else
        {
            ControlReturnNegative("command not found (execv failed)\n", execvp(words[0],command));
        } 
    }
}

void ExecutePipe()
{
    int status1, status2;
    pid_t child1,child2;
    char *command1[MAX_LINE_AMOUNT];
    char *command2[MAX_LINE_AMOUNT];
    int fdes[2];

     //words[0]=>command,words[1]=>parameter 
    char **words1 = GetLineInfo(HEAD,1); //1 beause there is just one line
    char **words2 = GetLineInfo(HEAD,2);
    AssignWords(words1, command1);
    AssignWords(words2, command2);

//Extern commands

    //create the pipe and initialize fdes[0] and fdes[1]
    ControlReturnNegative("Pipe failed\n", pipe(fdes));

    //create child1
    child1 = fork();
    ControlReturnNegative("Fork 1 failed\n", child1);

    if(child1 != 0)         
    {
         //create child2 as parent of child1
        child2 = fork();
        ControlReturnNegative("Fork 2 failed\n", child2)
    }
    //when parent from both
    if(child1 != 0 && child2 != 0)  
    {
        close(fdes[0]);
        close(fdes[1]);

        //wait for child1
        ControlReturnNegative("waitpid1 failed\n", waitpid(child1, &status1, 0));

       //wait for child2
        ControlReturnNegative("waitpid2 failed\n", waitpid(child2, &status2, 0));
    }
    else if(child1 == 0)        //when child1
    {
        //fdes[1] will be copied into STDOUT and survive execvp
        ControlReturnNegative("dup() failed at fd[1]", dup2(fdes[1], STDOUT_FILENO));

        close(fdes[0]);                  //close the read part of the pipe
        close(fdes[1]);

        ControlReturnNegative("command not found (execv child 1 failed)", execvp(words1[0],command1));        
    }
     //when child2
    else if(child2 == 0)       
    {
        //fdes[0] will be copied into STDIN and survive execvp
        ControlReturnNegative("dup() failed at fd[0]", dup2(fdes[0], STDIN_FILENO));

        close(fdes[1]);                 //close the write part of the pipe
        close(fdes[0]);

        ControlReturnNegative("command not found (execv child 2 failed)", execvp(words2[0],command2));
    }
}

