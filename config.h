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

#define INPUT_SIZE 512
#define COMMAND_PATH_SIZE 32

enum boolean {FALSE = 0, TRUE = 1};
void type_prompt();
void read_command();
void exec_cmd( const char* path_command,  char *command,  char* parameter);
int look_for_variable(const char *pmt);
char* get_var_value(char *pmt);

#endif