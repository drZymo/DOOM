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
// DESCRIPTION:
//	WAD I/O functions.
//
//-----------------------------------------------------------------------------


#ifndef __W_WAD__
#define __W_WAD__


#ifdef __GNUG__
#pragma interface
#endif


//
// TYPES
//
typedef struct
{
    // Should be "IWAD" or "PWAD".
    char		identification[4];		
    int32_t			numlumps;
    int32_t			infotableofs;
    
} wadinfo_t;


typedef struct
{
    int32_t			filepos;
    int32_t			size;
    char		name[8];
    
} filelump_t;

//
// WADFILE I/O related stuff.
//
typedef struct
{
    char	name[8];
    int32_t		handle;
    int32_t		position;
    int32_t		size;
} lumpinfo_t;


extern	void**		lumpcache;
extern	lumpinfo_t*	lumpinfo;
extern	int32_t		numlumps;

void    W_InitMultipleFiles (char** filenames);
void    W_Reload (void);

int32_t	W_CheckNumForName (char* name);
int32_t	W_GetNumForName (char* name);

int32_t	W_LumpLength (int32_t lump);
void    W_ReadLump (int32_t lump, void *dest);

void*	W_CacheLumpNum (int32_t lump, int32_t tag);
void*	W_CacheLumpName (char* name, int32_t tag);




#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------
