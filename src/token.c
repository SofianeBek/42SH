#include "token.h"

#include <err.h>
#include <stdlib.h>

#include "utils.h"

struct token *token_new(enum token_type type)
{
    struct token *new = xmalloc(sizeof(struct token));
    new->type = type;
    return new;
}

void free_token(struct token *token)
{
    if (token)
    {
        if (token->value)
        {
            size_t i = 0;
            while (token->value[i])
            {
                free(token->value[i]);
                i++;
            }
            free(token->value);
        }
        token->value = NULL;
        free(token);
    }
}
