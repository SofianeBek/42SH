#include "parse_and_or.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "parse_pipelines.h"
#include "utils.h"

parser_status parse_and_or(struct ast **ast, struct lexer *lexer, state st)
{
    // link node to current root
    struct ast *and_or = *ast;

    // create first child as a pipeline
    struct ast *neg_node = new_ast(AST_PIPE);
    neg_node->value = xcalloc(2 * sizeof(char *));
    neg_node->value[0] = xcalloc(7 * sizeof(char));
    strcpy(neg_node->value[0], "FALSE");
    neg_node->value[1] = NULL;

    and_or->children[0] = neg_node;

    // parse first pipeline
    parser_status status = parse_pipelines(&and_or->children[0], lexer, st);

    if (status != PARSER_OK)
        return status;

    // loop to parse the other pipeline
    while (true)
    {
        // check what symbol the logical operator is. If its not, break out of
        // the loop
        struct token *token = lexer_peek(lexer, &st);
        if (token->type != TOKEN_AND && token->type != TOKEN_OR)
        {
            free_token(token);
            return PARSER_OK;
        }

        // declare new logic node as child for and_or root
        struct ast *logic_node;

        // initialise logic node depending on token type
        if (token->type == TOKEN_AND)
            logic_node = new_ast(AST_AND);
        else
            logic_node = new_ast(AST_OR);

        free_token(token);
        free_token(lexer_pop(lexer, &st));

        // create node for subsequent child of logic_node
        struct ast *tmp_neg_node = new_ast(AST_PIPE);
        tmp_neg_node->value = xcalloc(2 * sizeof(char *));
        tmp_neg_node->value[0] = xcalloc(7 * sizeof(char));
        strcpy(tmp_neg_node->value[0], "TRUE");
        tmp_neg_node->value[1] = NULL;
        logic_node->children[0] = neg_node;

        // realloc and_or node chidlren to parse remaining pipelines
        and_or->children = realloc(
            and_or->children, (and_or->nb_children + 1) * sizeof(struct ast *));
        and_or->children[and_or->nb_children] = NULL;
        and_or->nb_children += 1;

        // parse child of logic node as pipeline
        status = parse_pipelines(&logic_node->children[0], lexer, st);

        // link logic_node and and_or root
        and_or->children[and_or->nb_children - 1] = logic_node;

        if (status != PARSER_OK)
            return status;
    }
}
