#pragma once

#include <stdlib.h>

#include "ast.h"
#include "token.h"

void *xmalloc(size_t size);
void *xrealloc(void *ptr, size_t size);
void *xcalloc(size_t size);
void get_args(char **dest, struct token *src);
int nb_args(char **src);
char *my_strdup(const char *lexer_inp);
// struct ast *ast_realloc (struct ast *tree);
