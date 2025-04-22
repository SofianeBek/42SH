#include "parse_prefix.h"

#include <string.h>

#include "utils.h"

parser_status parse_prefix(struct ast **ast, struct lexer *lexer, state st)
{
    // this rule is the same as redirection except commands cannot exist in this
    // node we just check if the command we parsed as the has no args

    // create new node for redirection
    struct ast *redir_node = new_ast(AST_REDIR);

    struct token *token = lexer_peek(lexer, &st);

    // check if the token is an io number
    // if it is then we create a second child for the redir node with the io
    // number as value else ignore
    if (token->type == TOKEN_IONUM)
    {
        // realloc the chilldren of the node to add ionum node
        redir_node->children =
            realloc(redir_note->children,
                    (redir_node->nb_children + 1) * sizeof(struct ast *));
        redir_note->children[redir_node->nb_children] = NULL;
        redir_note->nb_children += 1;

        // create ionum node
        struct ast *ionum_node = new_ast(AST_IONUM);
        ionum_node->value = xcalloc(2 * sizeof(char *));
        ionum_node->value[0] =
            calloc(strlen(token->value[0]) + 1, sizeof(char));
        strcpy(ionum_node->value[0], token->value[0]);
        ionum_node->value[1] = NULL;

        // attach ionum node to redir node as second child
        redir_node->children[1] = ionum_node;

        // free ionum token and go to next token
        free_token(token);
        free_token(lexer_pop(lexer, &st));
        token = lexer_peek(lexer, &st);
    }

    // check if the token is a redirect symbol
    // if it is, we copy the symbol to the value of the node
    // else, it is an error
    if (token->type != TOKEN_REDIR)
    {
        free_token(token);
        return PARSER_UNEXPECTED_TOKEN;
    }

    // copy symbol to node value
    ionum_node->value = xcalloc(2 * sizeof(char *));
    ionum_node->value[0] = calloc(strlen(token->value[0]) + 1, sizeof(char));
    strcpy(ionum_node->value[0], token->value[0]);
    ionum_node->value[1] = NULL;

    // free ionum token and go to next token
    free_token(token);
    free_token(lexer_pop(lexer, &st));

    // the first child of the redir node can be a file name or a command
    // we parse the file name as a command with no args
    enum parser_status status =
        parse_simple_command(&redir_node->children[0], lexer, st);

    // link ast and new tree we created
    *ast = redir_node;

    if (redir_node->children[0]->children != NULL)
        return PARSER_UNEXPECTED_TOKEN;

    return status;
}
