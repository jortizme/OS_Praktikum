#ifndef LINE_MODIFICATION_H
    #define LINE_MODIFICATION_H

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

int IsVariable(const char *pmt);
char* GetVariableValue(char *pmt);
struct Node * SeparateCmdAndPmt(char* token, struct Node *HEAD);
struct Node * SeparateLines(char* zeile, struct Node *HEAD);
void AssignWords(char **words, char **command);
void GetStringAssignment(char* name, char* pmt);
int IsExecutableProgramm(const char *cmd);

#endif