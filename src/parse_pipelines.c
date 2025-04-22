#include "parse_pipelines.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "parse_command.h"
#include "utils.h"

parser_status parse_pipelines(struct ast **ast, struct lexer *lexer, state st)
{
    // create new node for pipelines with node to indicate negation
    struct ast *neg_node = *ast;

    // check if ! symbol exists and update node value as such
    struct token *token = lexer_peek(lexer, &st);

    if (token->type == TOKEN_NEGATE)
    {
        strcpy(neg_node->value[0], "TRUE");
        free_token(lexer_pop(lexer, &st));
    }

    free_token(token);
    // parse for first pipeline
    enum parser_status status =
        parse_command(&neg_node->children[0], lexer, st);

    if (status != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;

    // loop to parse the other pipelines
    while (true)
    {
        token = lexer_peek(lexer, &st);

        // skip pipe symbol
        if (token->type != TOKEN_PIPELINE)
        {
            free_token(token);
            return PARSER_OK;
        }

        neg_node->children =
            realloc(neg_node->children,
                    (neg_node->nb_children + 1) * sizeof(struct ast *));
        neg_node->children[neg_node->nb_children] = NULL;
        neg_node->nb_children += 1;

        struct ast *pipe_ast = new_ast(AST_PIPE);
        neg_node->children[neg_node->nb_children - 1] = pipe_ast;

        free_token(token);
        free_token(lexer_pop(lexer, &st));
        token = lexer_peek(lexer, &st);

        // skip newline
        while (token->type == TOKEN_NEWLINE)
        {
            free_token(lexer_pop(lexer, &st));
            token = lexer_peek(lexer, &st);
        }

        free_token(token);

        // checks if next token is command
        token = lexer_peek(lexer, &st);
        if (token->type != TOKEN_COMMAND && token->type != TOKEN_IF)
        {
            free_token(token);
            return PARSER_UNEXPECTED_TOKEN;
        }
        free_token(token);

        // realloc children of main node to accomodate for new commands in
        // pipeline
        neg_node->children =
            realloc(neg_node->children,
                    (neg_node->nb_children + 1) * sizeof(struct ast *));
        neg_node->children[neg_node->nb_children] = NULL;
        neg_node->nb_children += 1;

        // parse for commands
        status = parse_command(&neg_node->children[neg_node->nb_children - 1],
                               lexer, st);

        if (status != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;
    }
}
