#ifndef LS_H
    #define LS_H

#include "config.h"

void exec_ls(int argc, char *argv[]);

#ifdef LS_CPP

#define MAX_ARG_AMOUNT 6
#define MAX_PATH_LENGTH 128
#define PERMISSION_FLAG_AMOUNT 10
#define TIME_FORMAT_LENGTH 15
#define DEBUG_MODE 0

typedef struct CommandInfo
{
    unsigned int NrArguments;
    bool PathIsThere;
    char PathName[MAX_PATH_LENGTH];
    char Arguments[MAX_ARG_AMOUNT];
    
}CommandInfo;

typedef struct VariablesforSearch
{
    DIR *DirStream;
    struct dirent* DirInfo; 
    struct stat buf;
    struct group* GrInfo;
    struct passwd* UsrInfo;
    struct tm* TimeInfo;
    char PathNameAux[MAX_PATH_LENGTH];

}VariablesforSearch;

typedef struct CheckForArguments
{
    bool a_isThere;
    bool o_isThere;
    bool l_isThere;
    bool g_isThere;

}CheckForArguments;

void parse_arguments(CommandInfo* Info,int argc,char** argv);
void getFilesInfo(VariablesforSearch* forSearch, CommandInfo* Info, CheckForArguments *Arguments);

#endif  //vom LS_CPP

#endif  //vom LS_H