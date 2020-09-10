// DoomLib additions

#include "doomlib.h"

#include <stdio.h>
#include <setjmp.h>

jmp_buf exit_jmp;

void Run(int argc, char *argv[])
{
    // Make sure the I_Exit() calls jump here
    if (!setjmp(exit_jmp))
    {
        myargc = argc;
        myargv = argv;

        D_DoomMain();
    }
    else
    {
        printf("exit()\n");
    }
}
