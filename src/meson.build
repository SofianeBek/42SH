subdir('builtin')
incdir = include_directories('include')
sources = files(
        'lexer.c',
        'token.c',
        'parse.c',
        'parse_list.c',
        'parse_pipelines.c',
        'parse_and_or.c',
        'parse_command.c',
        'parse_simple_command.c',
        'parse_shell_command.c',
        'parse_rule_if.c',
        'parse_compound_list.c',
        'parse_else_clause.c',
        'parse_and_or.c',
        'ast.c',
        'print.c',
        'error.c',
        'utils.c',
        'error.c',
        'exec.c',
        'usage.c',)
sources += builtin
exec_main_src = files('exec.c', 'exec_main.c', 'ast.c', 'utils.c')
exec_main_src += builtin
