#pragma once

#include "ast.h"
#include "error.h"
#include "lexer.h"

parser_status parse_shell_command(struct ast **ast, struct lexer *lexer,
                                  state st);
