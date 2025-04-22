#include "parse_command.h"

#include "parse_shell_command.h"
#include "parse_simple_command.h"

parser_status parse_command(struct ast **ast, struct lexer *lexer, state st)
{
    if (!lexer)
        return PARSER_UNEXPECTED_TOKEN;
    enum parser_status status = parse_simple_command(ast, lexer, st);

    if (status == PARSER_OK)
        return status;

    status = parse_shell_command(ast, lexer, st);
    return status;
}
