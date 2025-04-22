#include <dirent.h>
#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

int my_atoi(char *s)
{
    int ans = 0;
    for (size_t i = 0; i < strlen(s); i++)
    {
        ans *= 8;
        ans += (s[i] - '0');
    }
    return ans;
}

int processOpts(char **buff, char *options[], bool op[])
{
    int i = 0;
    for (; buff[i]; i++)
    {
        bool f = false;
        for (int k = 0; k < 2; k++)
            if (strcmp(options[k], buff[i]) == 0)
                f = op[k] = true;
        if (!f)
            return i;
    }
    return i;
}

void process_str(char *src, char *dst, bool op[])
{
    if (!op[1])
    {
        strcpy(dst, src);
        return;
    }
    size_t j = 0;
    int curr = 0;
    int clen;
    char code[5];
    for (size_t i = 0; i < strlen(src); i++)
    {
        if (curr == 0)
        {
            if (src[i] != '\\')
                dst[j++] = src[i];
            else
                curr = 1;
        }
        else if (curr == 1)
        {
            switch (src[i])
            {
            case 'b':
                dst[j++] = '\b';
                break;
            case 't':
                dst[j++] = '\t';
                break;
            case 'v':
                dst[j++] = '\v';
                break;
            case 'a':
                dst[j++] = '\a';
                break;
            case '\\':
                dst[j++] = '\\';
                break;
            case 'n':
                dst[j++] = '\n';
                break;
            case '0':
                curr = 2;
                clen = 0;
                code[clen++] = '0';
                break;
            default:
                dst[j++] = '\\';
                dst[j++] = src[i];
            }
            if (curr == 1)
                curr = 0;
        }
        else if (curr == 2)
        {
            if (src[i] >= '0' && src[i] <= '9')
                code[clen++] = src[i];
            else
            {
                code[clen] = 0;
                dst[j++] = my_atoi(code);
                if (src[i] == '\\')
                    curr = 1;
                else
                {
                    dst[j++] = src[i];
                    curr = 0;
                }
            }
        }
    }
    if (curr == 2)
    {
        code[clen] = 0;
        dst[j++] = my_atoi(code);
    }
    dst[j] = 0;
}

int echo(char **buff)
{
    char *options[2] = { "-n", "-e" };
    bool op[2] = { false, false };
    int p = processOpts(buff, options, op);
    char *res = NULL;
    for (int i = p; buff[i]; i++)
    {
        res = xcalloc(256 * sizeof(char));
        process_str(buff[i], res, op);
        fprintf(stdout, "%s", res);
        if (buff[i + 1] != NULL)
            fprintf(stdout, " ");
        free(res);
    }
    if (!op[0])
        printf("\n");
    fflush(stdout);
    return 0;
}
