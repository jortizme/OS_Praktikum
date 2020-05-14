#define LS_CPP
#include "include/ls.h"


void exec_ls(int argc, char *argv[])
{ 
    CommandInfo Info;
    VariablesforSearch forSearch;
    CheckForArguments Arguments = {.a_isThere = false, .o_isThere = false, .l_isThere = false, .g_isThere = false};

    for(int i = 0; i < MAX_ARG_AMOUNT; i++)
    {
        Info.Arguments[i] = 0;
    }

    parse_arguments(&Info,argc,argv);

    getFilesInfo(&forSearch, &Info, &Arguments);
    
}

void parse_arguments(CommandInfo* Info,int argc,char** argv)
{
    //if no arguments and no path given
    if(argc == 1)
    {
        Info->NrArguments = 0;
        Info->PathIsThere = false;
        strcpy(Info->PathName, ".");
    }
    //if there was something given
    else
    {
        unsigned int cnt;
        unsigned int loopLimit;

        //if there was no path given
        if (strchr(argv[argc-1],'/') == NULL && strchr(argv[argc-1],'.') == NULL)
        {  
            Info->PathIsThere = false;
            strcpy(Info->PathName, ".");
            loopLimit = argc - 1;
        }
        //if there is a path 
        else 
        {
            Info->PathIsThere = true;
            if(*argv[argc-1] == '~')
            {
                char PrefixAux[20] = "/home/";
                strcat(PrefixAux, getenv("$USER"));
                strcpy(Info->PathName, PrefixAux);
                strcat(Info->PathName, ++argv[argc-1]);
            }
            else
            {
                strcpy(Info->PathName, argv[argc-1]);
                strcat(Info->PathName,"/");
            }
            
            loopLimit = argc - 2;
            
            //This is the case, when only the path is available and no arguments
            if(argc == 2)
            {
                Info->NrArguments = 0;
                return;
            }
        }
        for(cnt = 1; cnt <= loopLimit; cnt++)
            {   
                //make sure the arguments start with the '-' symbol
                if(strchr(argv[cnt],'-') == NULL)
                {
                    ErrorSeveral("Arguments must be started by the '-' symbol\n");
                }
                
                //the ++ allows us to ignore the '-' symbol from the argument
                strcat(Info->Arguments,++argv[cnt]);
            }
        Info->NrArguments = strlen(Info->Arguments);

        if(Info->NrArguments > 4)
        {
            ErrorSeveral("Too much arguments given, maximal 4 are allowed\n");
        }   
    }

#if DEBUG_MODE == 1
    printf("Number Arguments : %u\n", Info->NrArguments);
    printf("Destination path there : %s\n", Info->PathIsThere ? "TRUE" : "FALSE");
    printf("Path name: %s\n", Info->PathName);
    printf("Arguments type: %s\n\n", Info->Arguments);
#endif

}

void getFilesInfo(VariablesforSearch* forSearch, CommandInfo* Info, CheckForArguments *Arguments)
{
    //Argaux created in order to let Info->Arguments unchanged for later use
    char* Argaux = Info->Arguments;

    for(int i = 0; i < Info->NrArguments; i++)
    {
        switch (*Argaux)
        {
        case 'a':  Arguments->a_isThere = true; break;
        case 'l':  Arguments->l_isThere = true; break;
        case 'g':  Arguments->g_isThere = true; break;
        case 'o':  Arguments->o_isThere = true; break;
        default:
            break;
        }
        Argaux++;
    }

    //open directory
    forSearch->DirStream = opendir(Info->PathName);
    ControlReturnNULL("opendir()",forSearch->DirStream);
        
    errno = 0;
    int iterator = 0;
    char fTypExecP[PERMISSION_FLAG_AMOUNT + 1];
    char TimeFormat[TIME_FORMAT_LENGTH];

    //every file/directory contained in the path will be shown
    while((forSearch->DirInfo = readdir(forSearch->DirStream)) != NULL)
    {
        iterator++;
        if(Info->PathIsThere == true)
        {
            strcpy(forSearch->PathNameAux,Info->PathName);
            strcat(forSearch->PathNameAux,forSearch->DirInfo->d_name);
            ControlReturnNegative("lstat()",lstat(forSearch->PathNameAux,&forSearch->buf));
        }
        //if there is no path, it means we look for infos in the current directory, this is 
        //why the d_name is given in lstat() directly and not with a followed concatenated path
        else
        {
            ControlReturnNegative("lstat()",lstat(forSearch->DirInfo->d_name,&forSearch->buf));
        }

        //ignore the .* files when -a argument not set
        if( *forSearch->DirInfo->d_name == '.' && Arguments->a_isThere == false )
        {
            iterator--;
            continue;
        }


        Argaux = Info->Arguments;

        if(Arguments->a_isThere)
        {
            //Argaux points to the l,o or g argument when a available
            while(*Argaux == 'a')
                Argaux++;
        }

        if(Arguments->l_isThere || Arguments->g_isThere || Arguments->o_isThere)
        {
            // ALLL DAS GANZE NURR WENN L ODER O ODER G GESETZT
            strcpy(fTypExecP, "----------");
            *TimeFormat = 0; 

            switch (forSearch->buf.st_mode & __S_IFMT) 
            {
            case __S_IFDIR:  fTypExecP[0] = 'd';  break;
            case __S_IFLNK:  fTypExecP[0] = 'l';  break;
            default:                              break;
            }

            unsigned int execFlags = 0;

            for(int i = 0; i < 3; i++)
            {
                execFlags = forSearch->buf.st_mode & (S_IRWXU >> (i*3));

                if(execFlags & (S_IRUSR >> (i*3)))
                    fTypExecP[1 + i*3] = 'r';
            
                if(execFlags & (S_IWUSR >> (i*3)))
                    fTypExecP[2 + i*3] = 'w';
            
                if(execFlags & (S_IXUSR >> (i*3)))
                    fTypExecP[3 + i*3] = 'x';
            }
            
            bool PrintIDOwner = true;
            bool PrintIDGroup = true;

            forSearch->GrInfo = getgrgid(forSearch->buf.st_gid);
            forSearch->UsrInfo = getpwuid(forSearch->buf.st_uid);
            forSearch->TimeInfo = localtime(&forSearch->buf.st_mtime);
            strftime(TimeFormat,TIME_FORMAT_LENGTH,"%b  %e  %H:%M",forSearch->TimeInfo);

            printf("%s\t",fTypExecP);                 //Type of file 
            printf("%ld\t",forSearch->buf.st_nlink); //Number of hard links : 
            printf("%ld\t", forSearch->buf.st_size);  //Total size, in bytes : 
            printf("%s\t", TimeFormat);    //Time of last modification : 

            switch (*Argaux)
            {
            case 'g':   PrintIDOwner = false; break;
            case 'o':   PrintIDGroup = false;  break;
            default:     break;
            }

            if(PrintIDGroup)
                printf("%s\t", forSearch->GrInfo->gr_name);   //User ID of owner : 

            if(PrintIDOwner)
                printf("%s\t", forSearch->UsrInfo->pw_name);   //Group ID of owner : 
                
        }

        if(!Arguments->l_isThere && !Arguments->g_isThere && !Arguments->o_isThere)
        {
        
            printf("%-16s\t\t",forSearch->DirInfo->d_name);   //Name der Datei

            if(iterator % 3 == 0)
                printf("\n");
        }
        else
        {
            if(Arguments->l_isThere)
            {
                char* SearchAux;

                if(strchr(fTypExecP,'x') != NULL  && *fTypExecP == '-')
                {
                    printf("\033[1;31m");
                    printf("%s\t",forSearch->DirInfo->d_name);   //Name der Datei in red when executable
                    printf("\033[0m");
                }
                else if ((SearchAux = strrchr(forSearch->DirInfo->d_name,'.')) != NULL && *(++SearchAux) == 'c' && *(++SearchAux) == 0)
                {
                        printf("\033[1;32m");
                        printf("%s\t",forSearch->DirInfo->d_name);   //Name der Datei in green when *.c
                        printf("\033[0m");
                }
                else
                {
                    printf("%s\t",forSearch->DirInfo->d_name);   //Name der Datei
                }
            }
            else
            {
                printf("%s\t",forSearch->DirInfo->d_name);   //Name der Datei
                
            }
            printf("\n");
        }
    }
    
    printf("\n");

    ControlReturnNegative("closedir()",closedir(forSearch->DirStream));

}
