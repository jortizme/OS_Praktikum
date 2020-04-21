#ifndef CONFIG_H
    #define CONFIG_H 
#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>
#include <pwd.h>
#include "list.h"
#include "line_modification.h"

#define INPUT_SIZE 512

enum boolean {FALSE, TRUE};
enum test    {FAILURE = 3, SUCCESS};

void TypePrompt();
void ReadCommandLine(struct Variable_Node *Variable_Head_List);
void ExecuteNormalLine(struct Node* HEAD,  struct Variable_Node *Variable_Head_List);
void ExecutePipe(struct Node* HEAD);

#endif