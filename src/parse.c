#include <stdbool.h>

#include "parse_list.h"

parser_status parse(struct ast **ast, struct lexer *lexer, state st)
{
    if (!lexer)
        return PARSER_UNEXPECTED_TOKEN;

    struct token *token = lexer_peek(lexer, &st);
    if (token->type == TOKEN_EOF || token->type == TOKEN_NEWLINE)
    {
        free_token(token);
        *ast = NULL;
        return PARSER_OK;
    }

    free_token(token);

    struct ast *root = new_ast(AST_ROOT);
    parser_status status = parse_list(&root, lexer, st);
    *ast = root;
    if (status != PARSER_OK)
        return handle_parse_error(status, ast);

    struct token *tok = lexer_pop(lexer, &st);
    if (tok->type == TOKEN_NEWLINE || tok->type == TOKEN_EOF)
    {
        free_token(tok);
        return PARSER_OK;
    }

    free_token(tok);

    return handle_parse_error(PARSER_UNEXPECTED_TOKEN, ast);
}
