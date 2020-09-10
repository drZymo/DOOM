// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//	Main program, simply calls D_DoomMain high level loop.
//
//-----------------------------------------------------------------------------

#include "doomlib.h"
#include <stdio.h>

int file_index = 0;

void FrameHandler(int32_t width, int32_t height, uint8_t *framebuffer)
{
    printf("F");

    char filename[64];
    sprintf(filename, "doom%i.raw", file_index);
    FILE* fd = fopen(filename, "w");
    fwrite((void *)framebuffer, 3, width * height, fd);
    fclose(fd);

    file_index++;
}

int main(int argc,
         char **argv)
{
    char* args[] = {
        "doomtest",
        //"-skill", "4",
        //"-warp", "1", "1",
    };

    Run(sizeof(args)/sizeof(args[0]), args, FrameHandler);
    return 0;
}
