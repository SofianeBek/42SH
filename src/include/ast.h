#pragma once
#include <unistd.h>

enum ast_type
{
    AST_ROOT,
    AST_AND_OR,
    AST_AND,
    AST_OR,
    AST_PIPE,
    AST_COMMAND,
    AST_REDIR,
    AST_CLAUSE,
    AST_IF,
    AST_THEN,
    AST_ELSE,
    AST_ARGS,
    AST_NEG,
};

struct ast
{
    enum ast_type type;
    char **value;
    struct ast **children;
    int nb_children;
};

struct ast *new_ast(enum ast_type type);
void ast_free(struct ast *node);
