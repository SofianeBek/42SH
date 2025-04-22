#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "alloc.h"
#include "echo.h"
#include "lexer.h"
#include "parse.h"
#include "print.h"
#include "token.h"
#include "usage.h"

char *tab[] = {
    [TOKEN_COMMAND] = "Command",
    [TOKEN_CLAUSE] = "Clause",
    [TOKEN_IF] = "If",
    [TOKEN_THEN] = "Then",
    [TOKEN_ELIF] = "Elif",
    [TOKEN_ELSE] = "Else",
    [TOKEN_FI] = "Fi",
    [TOKEN_ARGS] = "Args",
    [TOKEN_NEWLINE] = "Newline",
    [TOKEN_EOF] = "EOF",
    [TOKEN_SEMICOL] = "Semicolon",
};

void print_array(char **s)
{
    size_t i = 0;
    while (s[i])
    {
        if (s[i])
            printf("%s\n", s[i]);
        i++;
    }
    if (i == 0)
        printf("\n");
}

int main(int argc, char **argv)
{
    char *cpy = get_input(argc, argv);
    if (!cpy)
        return 1;
    state st = COMMAND_NOT_PROCESSED;
    struct lexer *lexer = lexer_new(cpy);
    struct token *token = lexer_peek(lexer, &st);

    while (token->type != TOKEN_EOF)
    {
        if (token->type == TOKEN_ARGS)
            echo(token->value);
        if (token->type == TOKEN_ARGS || token->type == TOKEN_COMMAND)
        {
            printf("%s : ", tab[token->type]);
            print_array(token->value);
        }
        else
            printf("Token type : %s\n", tab[token->type]);

        free_token(token);
        free_token(lexer_pop(lexer, &st));
        token = lexer_peek(lexer, &st);
    }
    if (token->type == TOKEN_EOF)
        printf("Token type : %s\n", tab[token->type]);
    free_token(token);
    lexer_free(lexer);
    free(cpy);
    return 0;
}