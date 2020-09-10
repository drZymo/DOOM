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
//
//
//-----------------------------------------------------------------------------


#ifndef __D_PLAYER__
#define __D_PLAYER__


// The player data structure depends on a number
// of other structs: items (internal inventory),
// animation states (closely tied to the sprites
// used to represent them, unfortunately).
#include "d_items.h"
#include "p_pspr.h"

// In addition, the player is just a special
// case of the generic moving object/actor.
#include "p_mobj.h"

// Finally, for odd reasons, the player input
// is buffered within the player data struct,
// as commands per game tick.
#include "d_ticcmd.h"

#ifdef __GNUG__
#pragma interface
#endif




//
// Player states.
//
typedef enum
{
    // Playing or camping.
    PST_LIVE,
    // Dead on the ground, view follows killer.
    PST_DEAD,
    // Ready to restart/respawn???
    PST_REBORN		

} playerstate_t;


//
// Player internal flags, for cheats and debug.
//
typedef enum
{
    // No clipping, walk through barriers.
    CF_NOCLIP		= 1,
    // No damage, no health loss.
    CF_GODMODE		= 2,
    // Not really a cheat, just a debug aid.
    CF_NOMOMENTUM	= 4

} cheat_t;


//
// Extended player object info: player_t
//
typedef struct player_s
{
    mobj_t*		mo;
    playerstate_t	playerstate;
    ticcmd_t		cmd;

    // Determine POV,
    //  including viewpoint bobbing during movement.
    // Focal origin above r.z
    fixed_t		viewz;
    // Base height above floor for viewz.
    fixed_t		viewheight;
    // Bob/squat speed.
    fixed_t         	deltaviewheight;
    // bounded/scaled total momentum.
    fixed_t         	bob;	

    // This is only used between levels,
    // mo->health is used during levels.
    int32_t			health;	
    int32_t			armorpoints;
    // Armor type is 0-2.
    int32_t			armortype;	

    // Power ups. invinc and invis are tic counters.
    int32_t			powers[NUMPOWERS];
    boolean		cards[NUMCARDS];
    boolean		backpack;
    
    // Frags, kills of other players.
    int32_t			frags[MAXPLAYERS];
    weapontype_t	readyweapon;
    
    // Is wp_nochange if not changing.
    weapontype_t	pendingweapon;

    boolean		weaponowned[NUMWEAPONS];
    int32_t			ammo[NUMAMMO];
    int32_t			maxammo[NUMAMMO];

    // True if button down last tic.
    int32_t			attackdown;
    int32_t			usedown;

    // Bit flags, for cheats and debug.
    // See cheat_t, above.
    int32_t			cheats;		

    // Refired shots are less accurate.
    int32_t			refire;		

     // For intermission stats.
    int32_t			killcount;
    int32_t			itemcount;
    int32_t			secretcount;

    // Hint messages.
    char*		message;	
    
    // For screen flashing (red or bright).
    int32_t			damagecount;
    int32_t			bonuscount;

    // Who did damage (NULL for floors/ceilings).
    mobj_t*		attacker;
    
    // So gun flashes light up areas.
    int32_t			extralight;

    // Current PLAYPAL, ???
    //  can be set to REDCOLORMAP for pain, etc.
    int32_t			fixedcolormap;

    // Player skin colorshift,
    //  0-3 for which color to draw player.
    int32_t			colormap;	

    // Overlay view sprites (gun, etc).
    pspdef_t		psprites[NUMPSPRITES];

    // True if secret level has been done.
    boolean		didsecret;	

} player_t;


//
// INTERMISSION
// Structure passed e.g. to WI_Start(wb)
//
typedef struct
{
    boolean	in;	// whether the player is in game
    
    // Player stats, kills, collected items etc.
    int32_t		skills;
    int32_t		sitems;
    int32_t		ssecret;
    int32_t		stime; 
    int32_t		frags[4];
    int32_t		score;	// current score on entry, modified on return
  
} wbplayerstruct_t;

typedef struct
{
    int32_t		epsd;	// episode # (0-2)

    // if true, splash the secret level
    boolean	didsecret;
    
    // previous and next levels, origin 0
    int32_t		last;
    int32_t		next;	
    
    int32_t		maxkills;
    int32_t		maxitems;
    int32_t		maxsecret;
    int32_t		maxfrags;

    // the par time
    int32_t		partime;
    
    // index of this player in game
    int32_t		pnum;	

    wbplayerstruct_t	plyr[MAXPLAYERS];

} wbstartstruct_t;


#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------
