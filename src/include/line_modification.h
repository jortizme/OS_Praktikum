#ifndef LINE_MODIFICATION_H
    #define LINE_MODIFICATION_H


#include "list.h"

int IsVariable(const char *pmt);
char* GetVariableValue(char *pmt);
struct Node * SeparateCmdAndPmt(char* token, struct Node *HEAD);
struct Node * SeparateLines(char* zeile,  struct Node *HEAD);
int AssignWords(char **words, char **command);
void GetStringAssignment(char* name, char* pmt);
int IsExecutableProgramm(const char *cmd);

#endif