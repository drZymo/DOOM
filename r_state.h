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
//	Refresh/render internal state variables (global).
//
//-----------------------------------------------------------------------------


#ifndef __R_STATE__
#define __R_STATE__

// Need data structure definitions.
#include "d_player.h"
#include "r_data.h"



#ifdef __GNUG__
#pragma interface
#endif



//
// Refresh internal data structures,
//  for rendering.
//

// needed for texture pegging
extern fixed_t*		textureheight;

// needed for pre rendering (fracs)
extern fixed_t*		spritewidth;

extern fixed_t*		spriteoffset;
extern fixed_t*		spritetopoffset;

extern lighttable_t*	colormaps;

extern int32_t		viewwidth;
extern int32_t		scaledviewwidth;
extern int32_t		viewheight;

extern int32_t		firstflat;

// for global animation
extern int32_t*		flattranslation;	
extern int32_t*		texturetranslation;	


// Sprite....
extern int32_t		firstspritelump;
extern int32_t		lastspritelump;
extern int32_t		numspritelumps;



//
// Lookup tables for map data.
//
extern int32_t		numsprites;
extern spritedef_t*	sprites;

extern int32_t		numvertexes;
extern vertex_t*	vertexes;

extern int32_t		numsegs;
extern seg_t*		segs;

extern int32_t		numsectors;
extern sector_t*	sectors;

extern int32_t		numsubsectors;
extern subsector_t*	subsectors;

extern int32_t		numnodes;
extern node_t*		nodes;

extern int32_t		numlines;
extern line_t*		lines;

extern int32_t		numsides;
extern side_t*		sides;


//
// POV data.
//
extern fixed_t		viewx;
extern fixed_t		viewy;
extern fixed_t		viewz;

extern angle_t		viewangle;
extern player_t*	viewplayer;


// ?
extern angle_t		clipangle;

extern int32_t		viewangletox[FINEANGLES/2];
extern angle_t		xtoviewangle[SCREENWIDTH+1];
//extern fixed_t		finetangent[FINEANGLES/2];

extern fixed_t		rw_distance;
extern angle_t		rw_normalangle;



// angle to line origin
extern int32_t		rw_angle1;

// Segs count?
extern int32_t		sscount;

extern visplane_t*	floorplane;
extern visplane_t*	ceilingplane;


#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------
