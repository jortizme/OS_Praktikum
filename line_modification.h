#ifndef LINE_MODIFICATION_H
    #define LINE_MODIFICATION_H

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

int look_for_variable(const char *pmt);
char* get_var_value(char *pmt);
int loof_for_pipe(char *line);
struct Node * separate_cmd_pmt(char* token, struct Node *HEAD, char **space);
struct Node * separate_lines(char* zeile, struct Node *HEAD, char** space);
void check_parameter(char **words, char **command, char *space);
int look_for_assignment(const char *pmt);
void get_string_assingment(char* name, char* pmt);

#endif