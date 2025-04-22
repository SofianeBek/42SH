#pragma once
#include <stdbool.h>

#include "token.h"
#define BUILTIN_SIZE 256
char *builtin[BUILTIN_SIZE];

void add_builtin(char *fct);
void set_builtin(void);
bool is_builtin(char *command);
int call_builtin(char *command, char **args);
