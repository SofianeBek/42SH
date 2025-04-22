#pragma once

#include <err.h>

#include "ast.h"

void exec_ast(struct ast *ast);
int _exec_ast(struct ast *ast, bool *pipe, int fd[], int *fd_in,
              bool *right_of_pipe);
