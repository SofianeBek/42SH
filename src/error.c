#include "error.h"

parser_status handle_parse_error(parser_status status, struct ast **res)
{
    warnx("Unexpected token");
    ast_free(*res);
    *res = NULL;
    return status;
}
