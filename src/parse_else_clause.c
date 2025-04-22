#include <string.h>

#include "ast.h"
#include "parse.h"
#include "parse_compound_list.h"
#include "parse_rule_if.h"
#include "token.h"
#include "utils.h"

parser_status parse_else_clause(struct ast **ast, struct lexer *lexer, state st)
{
    // checking token for else
    struct token *token = lexer_peek(lexer, &st);

    if (token->type == TOKEN_ELSE)
    {
        free_token(token);
        free_token(lexer_pop(lexer, &st));

        // parse for compound list
        enum parser_status status = parse_compound_list(ast, lexer, st);

        return status;
    }
    else if (token->type == TOKEN_ELIF)
    {
        free_token(token);
        free_token(lexer_pop(lexer, &st));

        // if token is 'elif', create new tree with node as if
        struct ast *if_ast = new_ast(AST_IF);
        if_ast->children = realloc(if_ast->children, 4 * sizeof(struct ast *));
        if_ast->nb_children = 2;
        if_ast->children[2] = NULL;

        if_ast->value = xmalloc(2 * sizeof(char *));

        if_ast->value[0] = xcalloc(7 * sizeof(char));
        strcpy(if_ast->value[0], "FALSE");

        if_ast->value[1] = NULL;

        struct ast *tmp_if = if_ast;

        // parse a compound list for the first child of if
        parser_status status =
            parse_compound_list(&tmp_if->children[0], lexer, st);

        if (status != PARSER_OK)
        {
            ast_free(tmp_if);
            return PARSER_UNEXPECTED_TOKEN;
        }

        // checking token for then
        token = lexer_peek(lexer, &st);

        if (token->type != TOKEN_THEN)
        {
            free_token(token);
            ast_free(tmp_if);
            return PARSER_UNEXPECTED_TOKEN;
        }

        free_token(token);
        free_token(lexer_pop(lexer, &st));

        // parse a compound list for middle child of if, the 'then' statement
        status = parse_compound_list(&tmp_if->children[1], lexer, st);

        if (status != PARSER_OK)
        {
            ast_free(tmp_if);
            return PARSER_UNEXPECTED_TOKEN;
        }

        // parse an else clause for last child of elif, the 'else' statement
        // if its an else case, the last child becomes a compound list
        // if its an elif, the last child becomes an if node
        // if there is no else clause, last child remains NULL

        status = parse_else_clause(&tmp_if->children[2], lexer, st);

        if (status != PARSER_OK)
            tmp_if->children[2] = NULL;

        tmp_if->nb_children = 3;
        *ast = if_ast;

        return PARSER_OK;
    }

    free_token(token);
    return PARSER_UNEXPECTED_TOKEN;
}
