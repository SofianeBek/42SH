#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "parse_command.h"
#include "parse_pipelines.h"
#include "token.h"
#include "utils.h"

parser_status parse_list(struct ast **ast, struct lexer *lexer, state st)
{
    if (!lexer)
        return PARSER_UNEXPECTED_TOKEN;

    // Save the root node in a temp variable
    struct ast *root = *ast;

    // Create AST_NEG node
    struct ast *neg_node = new_ast(AST_NEG);
    neg_node->value = xcalloc(2 * sizeof(char *));
    neg_node->value[0] = xcalloc(7 * sizeof(char));
    strcpy(neg_node->value[0], "FALSE");
    neg_node->value[1] = NULL;

    root->children[0] = neg_node;

    parser_status status = parse_pipelines(&root->children[0], lexer, st);

    if (status != PARSER_OK)
        return status;

    while (true)
    {
        struct token *token = lexer_peek(lexer, &st);
        if (token->type != TOKEN_SEMICOL)
        {
            free_token(token);
            return PARSER_OK;
        }

        free_token(token);
        free_token(lexer_pop(lexer, &st));

        token = lexer_peek(lexer, &st);
        if (token->type != TOKEN_COMMAND && token->type != TOKEN_IF)
        {
            free_token(token);
            return PARSER_OK;
        }

        free_token(token);

        neg_node = new_ast(AST_NEG);
        neg_node->value = xcalloc(2 * sizeof(char *));
        neg_node->value[0] = xcalloc(7 * sizeof(char));
        strcpy(neg_node->value[0], "FALSE");
        neg_node->value[1] = NULL;

        root->children = realloc(
            root->children, (root->nb_children + 1) * sizeof(struct ast *));
        root->children[root->nb_children] = NULL;
        root->nb_children += 1;
        root->children[root->nb_children - 1] = neg_node;
        status =
            parse_pipelines(&root->children[root->nb_children - 1], lexer, st);

        if (status != PARSER_OK)
            return status;
    }
}
