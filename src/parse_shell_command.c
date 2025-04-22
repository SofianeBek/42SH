#include "parse_shell_command.h"

#include "parse_rule_if.h"

parser_status parse_shell_command(struct ast **ast, struct lexer *lexer,
                                  state st)
{
    if (!lexer)
        return PARSER_UNEXPECTED_TOKEN;

    parser_status status = parse_rule_if(ast, lexer, st);
    return status;
}
