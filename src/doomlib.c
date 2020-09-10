// DoomLib additions

#include "doomlib.h"

#include <stdio.h>
#include <setjmp.h>
#include "doomdef.h"

jmp_buf exit_jmp;

framehandler_t g_framehandler;

void Run(int argc, char *argv[], framehandler_t framehandler)
{
    // Make sure the I_Exit() calls jump here
    if (!setjmp(exit_jmp))
    {
        myargc = argc;
        myargv = argv;
        g_framehandler = framehandler;

        D_DoomMain();
    }
    else
    {
        printf("exit()\n");
    }
}


// TODO: Convert to float32
uint8_t framebuffer[SCREENWIDTH * SCREENHEIGHT * 3];

extern uint8_t g_palette[768];
extern uint8_t *screens[5];

void D_ProcessFrame(void)
{
    printf(".");

    for (int32_t y = 0; y < SCREENHEIGHT; y++)
    {
        for (int32_t x = 0; x < SCREENWIDTH; x++)
        {
            int32_t i = SCREENWIDTH * y + x;
            uint8_t p = screens[0][i];
            framebuffer[i * 3 + 0] = g_palette[p * 3 + 0];
            framebuffer[i * 3 + 1] = g_palette[p * 3 + 1];
            framebuffer[i * 3 + 2] = g_palette[p * 3 + 2];
        }
    }

    if (g_framehandler != NULL)
    {
        g_framehandler(SCREENWIDTH, SCREENHEIGHT, framebuffer);
    }
}
