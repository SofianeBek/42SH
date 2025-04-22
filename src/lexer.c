#include "lexer.h"

#include <err.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "utils.h"

struct lexer *lexer_new(char *input)
{
    struct lexer *new_lex = xmalloc(sizeof(struct lexer));
    new_lex->current_tok = NULL;
    new_lex->pos = 0;
    new_lex->input = input;

    return new_lex;
}

void lexer_free(struct lexer *lexer)
{
    if (lexer)
        free(lexer);
    lexer = NULL;
}

bool is_delim(char *tok, state *st)
{
    if (strcmp(tok, "&&") == 0)
    {
        *st = COMMAND_NOT_PROCESSED;
        return true;
    }
    if (strcmp(tok, "||") == 0)
    {
        *st = COMMAND_NOT_PROCESSED;
        return true;
    }
    if (strcmp(tok, ">") == 0 || strcmp(tok, "<") == 0 || strcmp(tok, ">&") == 0
        || strcmp(tok, "<&") == 0 || strcmp(tok, ">>") == 0
        || strcmp(tok, "<>") == 0 || strcmp(tok, ">|") == 0)
    {
        *st = COMMAND_NOT_PROCESSED;
        return true;
    }
    if (strcmp(tok, "|") == 0)
    {
        *st = COMMAND_NOT_PROCESSED;
        return true;
    }
    if (strcmp(tok, "if") == 0)
    {
        *st = COMMAND_NOT_PROCESSED;
        return true;
    }
    else if (strcmp(tok, "then") == 0)
    {
        *st = COMMAND_NOT_PROCESSED;
        return true;
    }
    else if (strcmp(tok, "elif") == 0)
    {
        *st = COMMAND_NOT_PROCESSED;
        return true;
    }
    else if (strcmp(tok, "else") == 0)
    {
        *st = COMMAND_NOT_PROCESSED;
        return true;
    }
    else if (strcmp(tok, "fi") == 0)
    {
        *st = COMMAND_NOT_PROCESSED;
        return true;
    }
    else if (strcmp(tok, "\n") == 0)
    {
        *st = COMMAND_NOT_PROCESSED;
        return true;
    }
    else if (strcmp(tok, ";") == 0)
    {
        *st = COMMAND_NOT_PROCESSED;
        return true;
    }
    else if (strcmp(tok, "!") == 0)
    {
        *st = COMMAND_NOT_PROCESSED;
        return true;
    }
    return false;
}

void set_lexer(struct token *token, char *tok, size_t *curr_index,
               size_t *capacity, state *state)
{
    if (strcmp(tok, "&&") == 0)
    {
        token->type = TOKEN_AND;
        *state = COMMAND_NOT_PROCESSED;
    }
    else if (strcmp(tok, "||") == 0)
    {
        token->type = TOKEN_OR;
        *state = COMMAND_NOT_PROCESSED;
    }
    else if (strcmp(tok, ">") == 0 || strcmp(tok, "<") == 0
             || strcmp(tok, ">&") == 0 || strcmp(tok, "<&") == 0
             || strcmp(tok, ">>") == 0 || strcmp(tok, "<>") == 0
             || strcmp(tok, ">|") == 0)
    {
        token->type = TOKEN_REDIR;
        *state = COMMAND_NOT_PROCESSED;
    }
    else if (strcmp(tok, "|") == 0)
    {
        token->type = TOKEN_PIPELINE;
        *state = COMMAND_NOT_PROCESSED;
    }
    else if (strcmp(tok, "if") == 0)
    {
        token->type = TOKEN_IF;
        *state = COMMAND_NOT_PROCESSED;
    }
    else if (strcmp(tok, "then") == 0)
    {
        token->type = TOKEN_THEN;
        *state = COMMAND_NOT_PROCESSED;
    }
    else if (strcmp(tok, "elif") == 0)
    {
        token->type = TOKEN_ELIF;

        *state = COMMAND_NOT_PROCESSED;
    }
    else if (strcmp(tok, "else") == 0)
    {
        token->type = TOKEN_ELSE;
        *state = COMMAND_NOT_PROCESSED;
    }
    else if (strcmp(tok, "fi") == 0)
    {
        token->type = TOKEN_FI;
        *state = COMMAND_NOT_PROCESSED;
    }
    else if (strcmp(tok, "\n") == 0)
    {
        token->type = TOKEN_NEWLINE;

        *state = COMMAND_NOT_PROCESSED;
    }
    else if (strcmp(tok, ";") == 0)
    {
        token->type = TOKEN_SEMICOL;
        *state = COMMAND_NOT_PROCESSED;
    }
    else if (strcmp(tok, "!") == 0)
    {
        token->type = TOKEN_NEGATE;
        *state = COMMAND_NOT_PROCESSED;
    }
    else
    {
        if (*state == COMMAND_PROCESSED)
        {
            token->type = TOKEN_ARGS;
            if (*curr_index + 2 > *capacity)
            {
                token->value =
                    xrealloc(token->value, *capacity * 2 * sizeof(char *));
                *capacity *= 2;
            }
            token->value[*curr_index] = strdup(tok);
            (*curr_index)++;
            token->value[*curr_index] = NULL;
        }
        else
        {
            token->type = TOKEN_COMMAND;
            token->value[0] = strdup(tok);
            token->value[1] = NULL;
            *state = COMMAND_PROCESSED;
        }
    }
}

struct token *lexer_peek(struct lexer *lexer, state *st)
{
    state tmp_st = *st;
    size_t tmp_pos = lexer->pos;
    struct token *tok = lexer_pop(lexer, st);
    lexer->pos = tmp_pos;
    *st = tmp_st;
    return tok;
}

char *single_quote(const char *str, size_t *i)
{
    char *new_str = calloc(1024, sizeof(char));
    if (!new_str)
        return NULL;
    size_t index = 0;
    for (; str[index] != '\'' && str[index] != '\0'; index++)
        new_str[index] = str[index];
    new_str[index] = '\0';
    if (str[index] == '\0')
        *i += strlen(new_str) + 1;
    else
        *i += strlen(new_str) + 2;
    return new_str;
}

struct token *token_init(void)
{
    struct token *token = xmalloc(sizeof(struct token));
    token->type = TOKEN_NONE;
    token->value = xmalloc(sizeof(char *) * 2);
    token->value[0] = NULL;
    return token;
}

char *create_tok(const char *str, size_t *i)
{
    char *new_str = xcalloc(1024 * sizeof(char));
    size_t index = 0;
    for (; str[index] != '\0' && str[index] != ' ' && str[index] != '\t';
         index++)
        new_str[index] = str[index];
    new_str[index] = '\0';
    *i += strlen(new_str);
    return new_str;
}

struct token *lexer_pop(struct lexer *lexer, state *state)
{
    if (!lexer)
        return NULL;

    size_t i = lexer->pos;
    size_t curr_index = 0;
    size_t capacity = 2;
    char *tok = NULL;
    struct token *token = token_init();
    if (!lexer->input[i] || lexer->input[i] == '\0')
    {
        token->type = TOKEN_EOF;
        free(token->value);
        token->value = NULL;
        return token;
    }
    while (lexer->input[i] != '\0')
    {
        if (lexer->input[i] == ' ' || lexer->input[i] == '\t')
            i++;
        else
        {
            if (tok)
            {
                free(tok);
                tok = NULL;
            }
            if (lexer->input[i] == '\'')
                tok = single_quote(lexer->input + i + 1, &i);
            else if (lexer->input[i] != ' ' && lexer->input[i] != '\t')
                tok = create_tok(lexer->input + i, &i);

            if (*state == COMMAND_NOT_PROCESSED)
            {
                set_lexer(token, tok, &curr_index, &capacity, state);
                break;
            }
            if (is_delim(tok, state) && token->type != TOKEN_NONE)
            {
                i -= strlen(tok);
                break;
            }
            set_lexer(token, tok, &curr_index, &capacity, state);
            if (*state == COMMAND_NOT_PROCESSED)
                break;
        }
    }
    lexer->pos = i;
    if (!tok)
    {
        token->type = TOKEN_EOF;
        free(token->value);
        token->value = NULL;
    }
    else
        free(tok);
    return token;

    /*    size_t curr_index = 0;
        size_t capacity = 2;
        char *save_ptr;
        struct token *token;
        char *tok;
        char *cpy = strdup(lexer->input);
        tok = strtok_r(cpy + lexer->pos, " \t\r", &save_ptr);
        lexer->pos = save_ptr - cpy;
        token = token_init();
        // token->value = xmalloc(sizeof(char *) * 2);
        // token->value[0] = NULL;
        if (!tok)
        {
            token->type = TOKEN_EOF;
            free(token->value);
            free(cpy);
            token->value = NULL;
            return token;
        }

        if (*state == COMMAND_PROCESSED)
        {
            while (true)
            {
                set_lexer(token, tok, &curr_index, &capacity, state);
                if (*state == COMMAND_NOT_PROCESSED)
                    break;

                tok = strtok_r(NULL, " \t\r", &save_ptr);
                if (!tok)
                {
                    free(cpy);
                    return token;
                }
                if (is_delim(tok, state))
                    break;
                lexer->pos = save_ptr - cpy;
            }
        }
        else
            set_lexer(token, tok, &curr_index, &capacity, state);
        free(cpy);
        return token;*/
}
