
//Veranstaltung: Betriebssysteme 
//Name, Matrikel-Nr: Joaquin Alejandro Ortiz Meza - 835862
//                   Marcel Mazeewski - 867084 
//Datum: 14.04.2020
//joaquin.ortiz-meza@hs-osnabrueck.de 

#define MAIN
#include "include/config.h"

int main ()
{   
    DefGlobVar();

    while(1)
    {
        TypePrompt();
        ReadCommandLine();
    }
   return 0;
}
