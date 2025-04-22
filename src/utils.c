#include "utils.h"

#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "token.h"

void *xmalloc(size_t size)
{
    void *res = malloc(size);
    if (res == NULL && size != 0)
        abort();
    return res;
}

void *xrealloc(void *ptr, size_t size)
{
    void *res = realloc(ptr, size);
    if (res == NULL && size != 0)
        abort();
    return res;
}

void *xcalloc(size_t size)
{
    void *res = xmalloc(size);
    memset(res, 0, size);
    return res;
}

void get_args(char **dest, struct token *src)
{
    int i = 0;
    while (src->value[i] != NULL)
    {
        dest[i] = malloc((strlen(src->value[i]) + 1));
        strcpy(dest[i], src->value[i]);
        i++;
    }
    dest[i] = NULL;
}

int nb_args(char **src)
{
    int i = 0;
    while (src[i] != NULL)
        i++;
    return i;
}

int is_op(char c)
{
    if ((c == ';' || c == '\n'))
        return 1;
    return 0;
}

char *my_strdup(const char *lexer_inp)
{
    size_t len = strlen(lexer_inp);
    size_t index = 0;
    char *new_str = xmalloc(len * 3 * sizeof(char) + 1);
    for (size_t i = 0; i < len; i++)
    {
        if (is_op(lexer_inp[i]))
        {
            new_str[index++] = ' ';
            new_str[index++] = lexer_inp[i];
            new_str[index++] = ' ';
            continue;
        }
        new_str[index++] = lexer_inp[i];
    }
    new_str[index] = '\0';
    return new_str;
}