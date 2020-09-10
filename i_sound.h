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
//
// DESCRIPTION:
//	System interface, sound.
//
//-----------------------------------------------------------------------------

#ifndef __I_SOUND__
#define __I_SOUND__

#include "doomdef.h"

// UNIX hack, to be removed.
#ifdef SNDSERV
#include <stdio.h>
extern FILE* sndserver;
extern char* sndserver_filename;
#endif

#include "doomstat.h"
#include "sounds.h"



// Init at program start...
void I_InitSound();

// ... update sound buffer and audio device at runtime...
void I_UpdateSound(void);
void I_SubmitSound(void);

// ... shut down and relase at program termination.
void I_ShutdownSound(void);


//
//  SFX I/O
//

// Initialize channels?
void I_SetChannels();

// Get raw data lump index for sound descriptor.
int32_t I_GetSfxLumpNum (sfxinfo_t* sfxinfo );


// Starts a sound in a particular sound channel.
int32_t
I_StartSound
( int32_t		id,
  int32_t		vol,
  int32_t		sep,
  int32_t		pitch,
  int32_t		priority );


// Stops a sound channel.
void I_StopSound(int32_t handle);

// Called by S_*() functions
//  to see if a channel is still playing.
// Returns 0 if no longer playing, 1 if playing.
int32_t I_SoundIsPlaying(int32_t handle);

// Updates the volume, separation,
//  and pitch of a sound channel.
void
I_UpdateSoundParams
( int32_t		handle,
  int32_t		vol,
  int32_t		sep,
  int32_t		pitch );


//
//  MUSIC I/O
//
void I_InitMusic(void);
void I_ShutdownMusic(void);
// Volume.
void I_SetMusicVolume(int32_t volume);
// PAUSE game handling.
void I_PauseSong(int32_t handle);
void I_ResumeSong(int32_t handle);
// Registers a song handle to song data.
int32_t I_RegisterSong(void *data);
// Called by anything that wishes to start music.
//  plays a song, and when the song is done,
//  starts playing it again in an endless loop.
// Horrible thing to do, considering.
void
I_PlaySong
( int32_t		handle,
  int32_t		looping );
// Stops a song over 3 seconds.
void I_StopSong(int32_t handle);
// See above (register), then think backwards
void I_UnRegisterSong(int32_t handle);



#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------