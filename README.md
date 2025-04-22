# 42sh

### Compilation

To compile use the following commands :
```
meson setup builddir
ninja -C builddir
```

### Usage

There are three ways 42sh can read its input Shell program :

- It can read its input from a string by prefacing with the flag -c
```
42sh$ ./42sh -c "echo Input as string"
Input as string
```
- It can read directly from a file if given as the first and only argument
```
42sh$ cat -e script.sh
echo Input as file$
42sh$ ./42sh script.sh
Input as file
```
- It can also read commands from standard input when no other source is provided
```
42sh$ cat -e script.sh
echo Input through stdin$
42sh$ ./42sh < script.sh
Input throught stdin
42sh$ cat script.sh | ./42sh
Input through stdin
```
