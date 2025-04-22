#pragma once

#include "ast.h"
#include "error.h"
#include "lexer.h"

parser_status parse_else_clause(struct ast **ast, struct lexer *lexer,
                                state st);
