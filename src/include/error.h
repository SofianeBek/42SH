#pragma once

#include <err.h>

#include "ast.h"

typedef enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN
} parser_status;

parser_status handle_parse_error(parser_status status, struct ast **res);
