#ifndef CONFIG_H
    #define CONFIG_H 

#define _POSIX_C_SOURCE 200112L
#define  _DEFAULT_SOURCE

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
#include <dirent.h>
#include <time.h>
#include <stdbool.h>
#include <grp.h>

#include "list.h"
#include "line_modification.h"
#include "ls.h"

#define INPUT_SIZE 512

#define ErrorSeveral(x) fprintf(stderr,x); exit(EXIT_FAILURE) 

#define ControlReturnNegative(y,x) if(x < 0){\
                        fprintf(stderr,"%s failed at line %d of file %s (function %s)\n",y,__LINE__,__FILE__,__func__);\
                        exit(EXIT_FAILURE);}

#define ControlReturnNULL(y,x) if(x == NULL){ \
                        fprintf(stderr,"%s failed at line %d of file %s (function %s)\n",y,__LINE__,__FILE__,__func__);\
                        exit(EXIT_FAILURE);}

enum test    {FAILURE, SUCCESS};

#ifdef MAIN
void TypePrompt();
void DefGlobVar();
void ReadCommandLine();
#endif

#ifdef CONFIG_CPP
void ExecuteNormalLine();
void ExecutePipe();
#endif

#endif // of CONFIG_H