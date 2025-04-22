#include <stdbool.h>

#include "parse_command.h"
#include "token.h"
#include "utils.h"

parser_status parse_compound_list(struct ast **ast, struct lexer *lexer,
                                  state st)
{
    if (!lexer)
        return PARSER_UNEXPECTED_TOKEN;

    struct token *token = lexer_peek(lexer, &st);

    // skip all newlines
    while (token->type == TOKEN_NEWLINE)
    {
        free_token(lexer_pop(lexer, &st));
        token = lexer_peek(lexer, &st);
    }
    free_token(token);

    // parse for commands
    parser_status status = parse_command(ast, lexer, st);

    if (status != PARSER_OK)
    {
        ast_free(*ast);
        return PARSER_UNEXPECTED_TOKEN;
    }

    struct ast *current = *ast;

    while (true)
    {
        token = lexer_peek(lexer, &st);

        // skip semi colon
        if (token->type == TOKEN_SEMICOL)
            free_token(lexer_pop(lexer, &st));

        free_token(token);
        token = lexer_peek(lexer, &st);

        // skip newline
        while (token->type == TOKEN_NEWLINE)
        {
            free_token(token);
            free_token(lexer_pop(lexer, &st));
            token = lexer_peek(lexer, &st);
        }

        free_token(token);
        token = lexer_peek(lexer, &st);

        if (token->type != TOKEN_COMMAND)
        {
            free_token(token);
            return PARSER_OK;
        }

        free_token(token);

        current->children =
            realloc(current->children,
                    (current->nb_children + 1) * sizeof(struct ast *));
        current->children[current->nb_children] = NULL;
        current->nb_children += 1;
        // parse for commands
        status = parse_command(&current->children[1], lexer, st);

        if (status != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;

        current = current->children[1];
    }
}
