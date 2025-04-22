#include "exec.h"

#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ast.h"
#include "builtin.h"
#include "echo.h"
#include "string.h"

void free_args(char **args)
{
    if (!args)
        return;
    for (size_t i = 0; args[i]; i++)
        free(args[i]);
    free(args);
}

size_t len_arr(char **arr)
{
    size_t i;
    for (i = 0; arr[i]; i++)
        ;
    return i;
}

char **copy_all(char **argv, int length, char *command)
{
    char **ppDest = malloc(sizeof(char **) * (length + 2));
    if (!ppDest)
    {
        return NULL;
    }

    int i = 0;
    for (; i < length; i++)
    {
        char *pCurrent = argv[i];
        size_t currentLength = strlen(pCurrent);
        ppDest[i + 1] = malloc(currentLength + 1);
        if (!ppDest[i + 1])
        {
            printf("entered error\n");
            while (i > 0)
            {
                free(ppDest[i]);
                i--;
            }
            free(ppDest);
            return NULL;
        }

        strcpy(ppDest[i + 1], argv[i]);
    }
    ppDest[0] = malloc(strlen(command) + 1);
    if (!ppDest[0])
    {
        free(ppDest);
        return NULL;
    }
    strcpy(ppDest[0], command);
    ppDest[length + 1] = NULL;
    return ppDest;
}
char **make_args_like_argv(char **argument, char *command)
{
    size_t len = len_arr(argument);
    char **args = copy_all(argument, len, command);
    return args;
}

int exec(char **arguments, char *command, bool *pipe, int fd[], int *fd_in,
         bool *right_of_pipe)
{
    int pid;
    int saved_stdout = dup(STDOUT_FILENO);
    char **args = NULL;
    if (is_builtin(command))
        return call_builtin(command, arguments);
    args = make_args_like_argv(arguments, command);
    ssize_t error = 0;
    pid = fork();
    if (pid < 0)
    {
        free_args(args);
        err(1, "forking child process failed");
    }
    else if (pid == 0)
    {
        if (*pipe)
        {
            dup2(*fd_in, 0);
            if (*right_of_pipe == false)
                dup2(fd[1], 1);
            close(fd[0]);
        }
        error = execvp(command, args);
        if (error == -1)
        {
            dup2(saved_stdout, 1);
            err(1, "execvp failed");
        }
        return error;
    }
    else
    {
        int wstatus;
        waitpid(pid, &wstatus, 0);
        free_args(args);
        args = NULL;
        close(fd[1]);
        *fd_in = fd[0];
        return WEXITSTATUS(wstatus);
    }
}

bool check_pipes(struct ast *ast)
{
    if (!ast)
        return false;
    for (int i = 0; i < ast->nb_children; i++)
    {
        if (ast->children[i] && ast->children[i]->type == AST_PIPE)
            return true;
    }
    return false;
}

int exec_command(struct ast *ast, bool *pipe, int fd[], int *fd_in,
                 bool *right_of_pipe)
{
    char *arr[3] = { NULL };
    if (ast->nb_children == 2 && !(ast->children[0]))
    {
        exec(arr, (ast->value)[0], pipe, fd, fd_in, right_of_pipe);
        return _exec_ast(ast->children[1], pipe, fd, fd_in, right_of_pipe);
    }
    if (!(ast->children[0]))
        return exec(arr, (ast->value)[0], pipe, fd, fd_in, right_of_pipe);
    else if (ast->nb_children == 2)
    {
        exec((ast->children)[0]->value, (ast->value)[0], pipe, fd, fd_in,
             right_of_pipe);
        return _exec_ast(ast->children[1], pipe, fd, fd_in, right_of_pipe);
    }
    else
        return exec((ast->children)[0]->value, (ast->value)[0], pipe, fd, fd_in,
                    right_of_pipe);
}

int exec_if(struct ast *ast, bool *pipe, int fd[], int *fd_in,
            bool *right_of_pipe)
{
    if (_exec_ast(ast->children[0], pipe, fd, fd_in, right_of_pipe)
        == 0) // if exit status is 0
        return _exec_ast(ast->children[1], pipe, fd, fd_in, right_of_pipe);
    else
        return _exec_ast(ast->children[2], pipe, fd, fd_in, right_of_pipe);
}

int exec_pipe(struct ast *ast)
{
    int fd[2];
    int fd_in = 0;
    bool p = true;
    int i = 0;
    bool right_of_pipe;
    int ret_val = -1;
    while (i < ast->nb_children)
    {
        pipe(fd);
        right_of_pipe = true;
        if (i != ast->nb_children - 1)
            right_of_pipe = false;
        ret_val = _exec_ast(ast->children[i], &p, fd, &fd_in, &right_of_pipe);
        i++;
    }
    return ret_val;
}

int _exec_ast(struct ast *ast, bool *pipe, int fd[], int *fd_in,
              bool *right_of_pipe)
{
    int ret_val = -1;
    if (ast == NULL)
        return 0;
    bool if_pipe = check_pipes(ast);
    if (if_pipe)
        return exec_pipe(ast);
    if (ast->type == AST_COMMAND)
    {
        return exec_command(ast, pipe, fd, fd_in, right_of_pipe);
    }
    else if (ast->type == AST_IF)
    {
        return exec_if(ast, pipe, fd, fd_in, right_of_pipe);
    }
    else
    {
        for (int i = 0; i < ast->nb_children; i++)
            ret_val =
                _exec_ast(ast->children[i], pipe, fd, fd_in, right_of_pipe);
    }
    return ret_val;
}

void exec_ast(struct ast *ast)
{
    set_builtin();
    bool pipe = false;
    bool right_of_pipe = false;
    int fd[2];
    int fd_in = 0;
    int exit_code = _exec_ast(ast, &pipe, fd, &fd_in, &right_of_pipe);
    exit(exit_code);
}
