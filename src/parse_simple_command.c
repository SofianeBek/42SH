#include "parse_simple_command.h"

#include <string.h>

#include "utils.h"

parser_status parse_simple_command(struct ast **ast, struct lexer *lexer,
                                   state st)
{
    struct token *command = lexer_peek(lexer, &st);
    if (command->type == TOKEN_COMMAND)
    {
        struct ast *comm_node = new_ast(AST_COMMAND);
        comm_node->value = xcalloc(2 * sizeof(char *));
        comm_node->value[0] =
            calloc(strlen(command->value[0]) + 1, sizeof(char));
        strcpy(comm_node->value[0], command->value[0]);
        comm_node->value[1] = NULL;

        free_token(command);
        free_token(lexer_pop(lexer, &st));

        struct token *args = lexer_peek(lexer, &st);
        int nb = 0;
        struct ast *arg_node = NULL;

        if (args->type == TOKEN_ARGS)
        {
            nb = nb_args(args->value);
            arg_node = new_ast(AST_ARGS);

            arg_node->value = xmalloc((nb + 1) * sizeof(char *));
            arg_node->value[0] = NULL;
            int i = 0;
            while (args->value[i] != NULL)
            {
                arg_node->value[i] =
                    calloc(strlen(args->value[i]) + 1, sizeof(char));
                strcpy(arg_node->value[i], args->value[i]);
                i++;
            }
            free_token(lexer_pop(lexer, &st));
        }
        free_token(args);

        if (nb != 0)
            arg_node->value[nb] = NULL;

        comm_node->children[0] = arg_node;
        *ast = comm_node;

        return PARSER_OK;
    }
    else
    {
        free_token(command);
        return PARSER_UNEXPECTED_TOKEN;
    }

    return PARSER_UNEXPECTED_TOKEN;
}
