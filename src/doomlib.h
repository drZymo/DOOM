#ifndef __DOOMLIB_H
#define __DOOMLIB_H

#include "m_argv.h"
#include "d_main.h"

typedef void (*framehandler_t)(int32_t width, int32_t height, uint8_t* framebuffer);

extern void Run(int argc, char* argv[], framehandler_t framehandler);

#endif
