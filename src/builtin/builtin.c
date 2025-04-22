#include "builtin.h"

#include <err.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "token.h"
// place all builtin functions here
#include "cd.h"
#include "echo.h"

/*
In the C99 standard:
an object that has static storage duration is not initialized explicitly, then:
    if it has pointer type, it is initialized to a null pointer

*/

void add_builtin(char *fct)
{
    size_t i;
    for (i = 0; builtin[i]; i++)
        ;
    if (i >= BUILTIN_SIZE)
        err(1, "Builtin array isn't large enough");
    builtin[i] = fct;
}

void set_builtin(void)
{
    add_builtin("echo");
    add_builtin("cd");
}

bool is_builtin(char *command)
{
    for (size_t i = 0; builtin[i]; i++)
        if (strcmp(command, builtin[i]) == 0)
            return true;
    return false;
}

int call_builtin(char *command, char **value)
{
    if (strcmp(command, "echo") == 0)
        return echo(value);
    else if (strcmp(command, "cd") == 0)
        return cd(value);
    else
        err(1, "call_builtin: error with is_builtin");
    return -1;
}

void print_builtin(void)
{
    for (size_t i = 0; builtin[i]; i++)
        printf("%s\n", builtin[i]);
    printf("\n");
}
