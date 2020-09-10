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
//
//-----------------------------------------------------------------------------


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdarg.h>
#include <sys/time.h>
#include <unistd.h>

#include "doomdef.h"
#include "m_misc.h"
#include "i_video.h"
#include "i_sound.h"

#include "d_net.h"
#include "g_game.h"

#ifdef __GNUG__
#pragma implementation "i_system.h"
#endif
#include "i_system.h"




int32_t	mb_used = 6;


void
I_Tactile
( int32_t	on,
  int32_t	off,
  int32_t	total )
{
  // UNUSED.
  on = off = total = 0;
}

ticcmd_t	emptycmd;
ticcmd_t*	I_BaseTiccmd(void)
{
    return &emptycmd;
}


int32_t  I_GetHeapSize (void)
{
    return mb_used*1024*1024;
}

uint8_t* I_ZoneBase (int32_t*	size)
{
    *size = mb_used*1024*1024;
    return (uint8_t *) malloc (*size);
}



//
// I_GetTime
// returns time in 1/70th second tics
//
int32_t  I_GetTime (void)
{
    struct timeval	tp;
    int32_t			newtics;
    static int32_t		basetime=0;
  
    gettimeofday(&tp, NULL);
    if (!basetime)
	basetime = tp.tv_sec;
    newtics = (tp.tv_sec-basetime)*TICRATE + tp.tv_usec*TICRATE/1000000;
    return newtics;
}



//
// I_Init
//
void I_Init (void)
{
    I_InitSound();
    //  I_InitGraphics();
}

//
// I_Quit
//
void I_Quit (void)
{
    D_QuitNetGame ();
    I_ShutdownSound();
    I_ShutdownMusic();
    M_SaveDefaults ();
    I_ShutdownGraphics();
    I_Exit(0);
}

void I_WaitVBL(int32_t count)
{
#ifdef SGI
    sginap(1);                                           
#else
#ifdef SUN
    sleep(0);
#else
    usleep (count * (1000000/70) );                                
#endif
#endif
}

void I_BeginRead(void)
{
}

void I_EndRead(void)
{
}

uint8_t*	I_AllocLow(int32_t length)
{
    uint8_t*	mem;
        
    mem = (uint8_t *)malloc (length);
    memset (mem,0,length);
    return mem;
}


//
// I_Error
//
extern boolean demorecording;

void I_Error (char *error, ...)
{
    va_list	argptr;

    // Message first.
    va_start (argptr,error);
    fprintf (stderr, "Error: ");
    vfprintf (stderr,error,argptr);
    fprintf (stderr, "\n");
    va_end (argptr);

    fflush( stderr );

    // Shutdown. Here might be other errors.
    if (demorecording)
	G_CheckDemoStatus();

    D_QuitNetGame ();
    I_ShutdownGraphics();
    
    I_Exit(-1);
}

/* -- DoomLib addition -- */

#include <setjmp.h>
extern jmp_buf exit_jmp;

void I_Exit(int code)
{
    // Instead of calling exit() we use longjmp to jump back to the entry function and gracefully exit.
    longjmp(exit_jmp, code);
}

/* --------------------- */