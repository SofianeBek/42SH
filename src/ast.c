#include "ast.h"

#include "utils.h"

struct ast *new_ast(enum ast_type type)
{
    struct ast *new = xcalloc(sizeof(struct ast));
    new->nb_children = 1;
    new->value = NULL;
    new->children = xcalloc(new->nb_children * sizeof(struct ast *));
    new->children[0] = NULL;
    new->type = type;
    return new;
}

void ast_free(struct ast *ast)
{
    if (ast == NULL)
    {
        free(ast);
        return;
    }

    for (int i = 0; i < ast->nb_children; i++)
    {
        ast_free(ast->children[i]);
    }
    free(ast->children);

    int i = 0;
    if (ast->value != NULL)
    {
        while (ast->value[i] != NULL)
        {
            free(ast->value[i]);
            i++;
        }
    }
    free(ast->value);
    free(ast);
}
