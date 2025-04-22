#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int find_c(int argc, char **argv, bool *opt_c)
{
    int i = 1;
    for (; i < argc; i++)
    {
        if (strcmp("-c", argv[i]) == 0)
        {
            *opt_c = true;
            break;
        }
        if (argv[i][0] == '-' && argv[i][1] != 'c')
        {
            fprintf(stderr, "Invalid option\n");
            return -1;
        }
    }
    return i;
}

char *get_input_from_fd(FILE *fp)
{
    int len = 1024;
    char *buf = calloc(len, sizeof(char));
    if (!buf)
    {
        fprintf(stderr,
                "Could not calloc while reading from file descriptor\n");
        return NULL;
    }
    char c;
    int counter = 0;
    while ((c = fgetc(fp)) != EOF)
    {
        if (counter == len)
        {
            len *= 2;
            buf = realloc(buf, len);
            if (!buf)
            {
                fprintf(
                    stderr,
                    "Could not calloc while reading from file descriptor\n");
                return NULL;
            }
        }
        buf[counter++] = c;
    }
    buf[counter] = '\0';
    return buf;
}

char *get_input(int argc, char **argv)
{
    bool opt_c = false;
    char *cpy = NULL;
    if (argc < 2)
    {
        char *buf = get_input_from_fd(stdin);
        if (!buf)
            return NULL;
        cpy = my_strdup(buf);
        free(buf);
    }
    else
    {
        int i = find_c(argc, argv, &opt_c);
        if (i != -1)
        {
            if (opt_c)
            {
                i++;
                cpy = my_strdup(argv[i]);
            }
            else
            {
                FILE *fp = fopen(argv[1], "r");
                if (!fp)
                    return NULL;
                char *f_stream = get_input_from_fd(fp);
                if (!f_stream)
                {
                    fclose(fp);
                    return NULL;
                }
                cpy = my_strdup(f_stream);
                free(f_stream);
                fclose(fp);
            }
        }
        else
            return NULL;
    }
    return cpy;
}
