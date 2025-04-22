#include <stdio.h>

#include "ast.h"
#include "parse.h"

void print_children(struct ast *tree)
{
    if (!tree)
        return;

    int j = 0;
    if (tree->type == AST_IF)
    {
        printf("If ");
        print_children(tree->children[0]);
        printf("Then ");
        print_children(tree->children[1]);
        printf("Else ");
        print_children(tree->children[2]);
    }
    else if (tree->type == AST_PIPE)
        printf(" | ");
    else
    {
        // printf("node type : %d\n", tree->type);
        while (tree->value[j])
        {
            printf("%s ", tree->value[j]);
            j++;
        }
        for (int i = 0; i < tree->nb_children; i++)
        {
            print_children(tree->children[i]);
        }
    }
}

void print_ast(struct ast *tree)
{
    for (int i = 0; i < tree->nb_children; i++)
    {
        print_children(tree->children[i]);
    }
}
