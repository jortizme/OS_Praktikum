
//Veranstaltung: Betriebssysteme 
//Name, Matrikel-Nr: Joaquin Alejandro Ortiz Meza - 835862
//                   Marcel Mazeewski - 867084 
//Datum: 14.04.2020
//joaquin.ortiz-meza@hs-osnabrueck.de - marcel.mazeewski@hs-osnabrueck.de

#include "config.h"

int main ()
{   
    char *main_global_variables[16] = {"DISPLAY","HOME","PATH",
                                        "LOGNAME","SHELL","USER",
                                        "USERNAME", NULL};
    struct Variable_Node *Variable_Head_List = NULL;
    char **aux = main_global_variables;
    int cnt = 0;

    //Initialization of global variable List
    while (aux[cnt] != NULL)
    {
        Variable_Head_List = add_to_variable_list(Variable_Head_List, aux[cnt]);
        cnt++;
    }
    
   //struct Variable_Node *Variable_Head_List = NULL;
    while(1)
    {
        type_prompt();
        read_command(Variable_Head_List);
    }
   return 0;
}
