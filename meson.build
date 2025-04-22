project(
  '42sh',
  'c',
  version : '1.0',
  default_options: [
    'b_sanitize=address',
    'c_std=c99',
    'warning_level=3',
    'werror=true',
  ],
)

subdir('src')

cflags = ['-D_GNU_SOURCE', '-D_POSIX_C_SOURCE=200809L', '-g']
add_project_arguments(cflags, language: 'c')

executable('42sh',  sources + [ 'src/main.c' ], include_directories : incdir)
executable('token_print', sources + [ 'src/token_printer.c' ], include_directories : incdir)
executable('parse_print', sources + [ 'src/parse_print.c' ], include_directories : incdir)
