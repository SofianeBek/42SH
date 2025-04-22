#include "cd.h"

#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int cd(char **buff)
{
    int ret_val = -1;
    if (!buff[0] || strcmp(buff[0], "~") == 0)
        ret_val = chdir(getenv("HOME"));
    else if (strcmp(buff[0], "-") == 0)
        ret_val = chdir(getenv("OLDPWD"));
    else
    {
        ret_val = chdir(buff[0]);
    }
    if (ret_val == -1)
    {
        fprintf(stderr, "cd: %s No such file or directory\n", buff[0]);
        return 1;
    }
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("Current working Dir: %s\n", cwd);
    else
    {
        perror("getcwd() error");
        return 1;
    }
    setenv("OLDPWD", getenv("PWD"), 1);
    setenv("PWD", cwd, 1);
    return 0;
}