#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "echo.h"
#include "exec.h"
#include "lexer.h"
#include "parse.h"
#include "print.h"
#include "token.h"
#include "usage.h"

int main(int argc, char **argv)
{
    char *cpy = get_input(argc, argv);
    if (!cpy)
        return 1;
    state st = COMMAND_NOT_PROCESSED;
    struct lexer *lexer = lexer_new(cpy);
    struct ast *tree;
    enum parser_status status = parse(&tree, lexer, st);
    if (status != PARSER_OK)
    {
        free(cpy);
        ast_free(tree);
        lexer_free(lexer);
        return 1;
    }

    print_ast(tree);
    printf("\n");

    lexer_free(lexer);
    free(cpy);
    ast_free(tree);

    return 0;
}
