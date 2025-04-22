#pragma once

#include <stdbool.h>

#include "ast.h"
#include "error.h"
#include "lexer.h"

parser_status parse(struct ast **ast, struct lexer *lexer, state st);
