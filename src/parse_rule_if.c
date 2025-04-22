#include <string.h>

#include "parse_command.h"
#include "parse_compound_list.h"
#include "parse_else_clause.h"
#include "token.h"
#include "utils.h"

parser_status parse_rule_if(struct ast **ast, struct lexer *lexer, state st)
{
    if (!lexer)
        return PARSER_UNEXPECTED_TOKEN;

    // checking token for if
    struct token *token = lexer_peek(lexer, &st);

    if (token->type != TOKEN_IF)
    {
        free_token(token);
        return PARSER_UNEXPECTED_TOKEN;
    }

    free_token(token);
    free_token(lexer_pop(lexer, &st));

    // if token is 'if', create new tree with node as if
    struct ast *if_ast = new_ast(AST_IF);
    if_ast->children = realloc(if_ast->children, 5 * sizeof(struct ast *));

    if_ast->value = xmalloc(2 * sizeof(char *));

    if_ast->value[0] = xcalloc(7 * sizeof(char));
    strcpy(if_ast->value[0], "FALSE");

    if_ast->value[1] = NULL;
    struct ast *tmp_if = if_ast;

    // parse a compound list for the first child of if
    parser_status status = parse_compound_list(&tmp_if->children[0], lexer, st);

    if (status != PARSER_OK)
    {
        ast_free(tmp_if);
        return PARSER_UNEXPECTED_TOKEN;
    }

    // checking token for then
    token = lexer_peek(lexer, &st);

    if (token->type != TOKEN_THEN)
    {
        ast_free(tmp_if);
        return PARSER_UNEXPECTED_TOKEN;
    }

    free_token(token);
    free_token(lexer_pop(lexer, &st));

    // parse a compound list for middle child of if, the 'then' statement
    status = parse_compound_list(&tmp_if->children[1], lexer, st);
    tmp_if->nb_children = 2;
    tmp_if->children[2] = NULL;

    if (status != PARSER_OK)
    {
        ast_free(tmp_if);
        return PARSER_UNEXPECTED_TOKEN;
    }

    // parse an else clause for last child of if, the 'else' statement
    // if its an else case, the last child becomes a compound list
    // if its an elif, the last child becomes an if node
    // if there is no else clause, last child remains NULL

    status = parse_else_clause(&tmp_if->children[2], lexer, st);

    // if an else clause if not found, the current token must be an FI
    // otherwise we check if the NEXT token is FI

    token = lexer_peek(lexer, &st);

    if (status == PARSER_OK)
    {
        tmp_if->nb_children = 3;
        tmp_if->children[3] = NULL;
    }

    if (token->type != TOKEN_FI)
    {
        ast_free(tmp_if);
        free_token(token);
        return PARSER_UNEXPECTED_TOKEN;
    }
    free_token(token);
    free_token(lexer_pop(lexer, &st));

    *ast = if_ast;

    return PARSER_OK;
}
