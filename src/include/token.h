#pragma once

#include <unistd.h>

typedef enum state
{
    COMMAND_PROCESSED,
    COMMAND_NOT_PROCESSED,
} state;

enum token_type
{
    TOKEN_NONE,
    TOKEN_COMMAND,
    TOKEN_CLAUSE,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELIF,
    TOKEN_ELSE,
    TOKEN_FI,
    TOKEN_ARGS,
    TOKEN_NEWLINE,
    TOKEN_EOF,
    TOKEN_SEMICOL,
    TOKEN_IONUM,
    TOKEN_REDIR,
    TOKEN_PIPELINE,
    TOKEN_NEGATE,
    TOKEN_OR,
    TOKEN_AND
};

struct token
{
    enum token_type type;
    char **value;
};

struct token *token_new(enum token_type type);

void free_token(struct token *token);
