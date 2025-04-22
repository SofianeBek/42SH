#include <stddef.h>
#include <stdlib.h>

#include "exec.h"
// test file for exec

int main(int argc, char **argv)
{
    if (argc == 1)
        abort();
    exec(argv);
    return 0;
}
