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
#include <string.h>
#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/ioctl.h>

#include "i_system.h"
#include "d_event.h"
#include "d_net.h"
#include "m_argv.h"

#include "doomstat.h"

#ifdef __GNUG__
#pragma implementation "i_net.h"
#endif
#include "i_net.h"





// For some odd reason...
#define ntohl(x) \
        ((uint32_t)((((uint32_t)(x) & 0x000000ffU) << 24) | \
                             (((uint32_t)(x) & 0x0000ff00U) <<  8) | \
                             (((uint32_t)(x) & 0x00ff0000U) >>  8) | \
                             (((uint32_t)(x) & 0xff000000U) >> 24)))

#define ntohs(x) \
        ((uint16_t)((((uint16_t)(x) & 0x00ff) << 8) | \
                              (((uint16_t)(x) & 0xff00) >> 8))) \
	  
#define htonl(x) ntohl(x)
#define htons(x) ntohs(x)

void	NetSend (void);
boolean NetListen (void);


//
// NETWORKING
//

int32_t	DOOMPORT =	(IPPORT_USERRESERVED +0x1d );

int32_t			sendsocket;
int32_t			insocket;

struct	sockaddr_in	sendaddress[MAXNETNODES];

void	(*netget) (void);
void	(*netsend) (void);


//
// UDPsocket
//
int32_t UDPsocket (void)
{
    int32_t	s;
	
    // allocate a socket
    s = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s<0)
	I_Error ("can't create socket: %s",strerror(errno));
		
    return s;
}

//
// BindToLocalPort
//
void
BindToLocalPort
( int32_t	s,
  int32_t	port )
{
    int32_t			v;
    struct sockaddr_in	address;
	
    memset (&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = port;
			
    v = bind (s, (void *)&address, sizeof(address));
    if (v == -1)
	I_Error ("BindToPort: bind: %s", strerror(errno));
}


//
// PacketSend
//
void PacketSend (void)
{
    int32_t		c;
    doomdata_t	sw;
				
    // uint8_t swap
    sw.checksum = htonl(netbuffer->checksum);
    sw.player = netbuffer->player;
    sw.retransmitfrom = netbuffer->retransmitfrom;
    sw.starttic = netbuffer->starttic;
    sw.numtics = netbuffer->numtics;
    for (c=0 ; c< netbuffer->numtics ; c++)
    {
	sw.cmds[c].forwardmove = netbuffer->cmds[c].forwardmove;
	sw.cmds[c].sidemove = netbuffer->cmds[c].sidemove;
	sw.cmds[c].angleturn = htons(netbuffer->cmds[c].angleturn);
	sw.cmds[c].consistancy = htons(netbuffer->cmds[c].consistancy);
	sw.cmds[c].chatchar = netbuffer->cmds[c].chatchar;
	sw.cmds[c].buttons = netbuffer->cmds[c].buttons;
    }
		
    //printf ("sending %i\n",gametic);		
    c = sendto (sendsocket , &sw, doomcom->datalength
		,0,(void *)&sendaddress[doomcom->remotenode]
		,sizeof(sendaddress[doomcom->remotenode]));
	
    //	if (c == -1)
    //		I_Error ("SendPacket error: %s",strerror(errno));
}


//
// PacketGet
//
void PacketGet (void)
{
    int32_t			i;
    int32_t			c;
    struct sockaddr_in	fromaddress;
    socklen_t		fromlen;
    doomdata_t		sw;
				
    fromlen = sizeof(fromaddress);
    c = recvfrom (insocket, &sw, sizeof(sw), 0
		  , (struct sockaddr *)&fromaddress, &fromlen );
    if (c == -1 )
    {
	if (errno != EWOULDBLOCK)
	    I_Error ("GetPacket: %s",strerror(errno));
	doomcom->remotenode = -1;		// no packet
	return;
    }

    {
	static int32_t first=1;
	if (first)
	    printf("len=%d:p=[0x%x 0x%x] \n", c, *(int32_t*)&sw, *((int32_t*)&sw+1));
	first = 0;
    }

    // find remote node number
    for (i=0 ; i<doomcom->numnodes ; i++)
	if ( fromaddress.sin_addr.s_addr == sendaddress[i].sin_addr.s_addr )
	    break;

    if (i == doomcom->numnodes)
    {
	// packet is not from one of the players (new game broadcast)
	doomcom->remotenode = -1;		// no packet
	return;
    }
	
    doomcom->remotenode = i;			// good packet from a game player
    doomcom->datalength = c;
	
    // uint8_t swap
    netbuffer->checksum = ntohl(sw.checksum);
    netbuffer->player = sw.player;
    netbuffer->retransmitfrom = sw.retransmitfrom;
    netbuffer->starttic = sw.starttic;
    netbuffer->numtics = sw.numtics;

    for (c=0 ; c< netbuffer->numtics ; c++)
    {
	netbuffer->cmds[c].forwardmove = sw.cmds[c].forwardmove;
	netbuffer->cmds[c].sidemove = sw.cmds[c].sidemove;
	netbuffer->cmds[c].angleturn = ntohs(sw.cmds[c].angleturn);
	netbuffer->cmds[c].consistancy = ntohs(sw.cmds[c].consistancy);
	netbuffer->cmds[c].chatchar = sw.cmds[c].chatchar;
	netbuffer->cmds[c].buttons = sw.cmds[c].buttons;
    }
}



int32_t GetLocalAddress (void)
{
    char		hostname[1024];
    struct hostent*	hostentry;	// host information entry
    int32_t			v;

    // get local address
    v = gethostname (hostname, sizeof(hostname));
    if (v == -1)
	I_Error ("GetLocalAddress : gethostname: errno %d",errno);
	
    hostentry = gethostbyname (hostname);
    if (!hostentry)
	I_Error ("GetLocalAddress : gethostbyname: couldn't get local host");
		
    return *(int32_t *)hostentry->h_addr_list[0];
}


//
// I_InitNetwork
//
void I_InitNetwork (void)
{
    boolean		trueval = true;
    int32_t			i;
    int32_t			p;
    struct hostent*	hostentry;	// host information entry
	
    doomcom = malloc (sizeof (*doomcom) );
    memset (doomcom, 0, sizeof(*doomcom) );
    
    // set up for network
    i = M_CheckParm ("-dup");
    if (i && i< myargc-1)
    {
	doomcom->ticdup = myargv[i+1][0]-'0';
	if (doomcom->ticdup < 1)
	    doomcom->ticdup = 1;
	if (doomcom->ticdup > 9)
	    doomcom->ticdup = 9;
    }
    else
	doomcom-> ticdup = 1;
	
    if (M_CheckParm ("-extratic"))
	doomcom-> extratics = 1;
    else
	doomcom-> extratics = 0;
		
    p = M_CheckParm ("-port");
    if (p && p<myargc-1)
    {
	DOOMPORT = atoi (myargv[p+1]);
	printf ("using alternate port %i\n",DOOMPORT);
    }
    
    // parse network game options,
    //  -net <consoleplayer> <host> <host> ...
    i = M_CheckParm ("-net");
    if (!i)
    {
	// single player game
	netgame = false;
	doomcom->id = DOOMCOM_ID;
	doomcom->numplayers = doomcom->numnodes = 1;
	doomcom->deathmatch = false;
	doomcom->consoleplayer = 0;
	return;
    }

    netsend = PacketSend;
    netget = PacketGet;
    netgame = true;

    // parse player number and host list
    doomcom->consoleplayer = myargv[i+1][0]-'1';

    doomcom->numnodes = 1;	// this node for sure
	
    i++;
    while (++i < myargc && myargv[i][0] != '-')
    {
	sendaddress[doomcom->numnodes].sin_family = AF_INET;
	sendaddress[doomcom->numnodes].sin_port = htons(DOOMPORT);
	if (myargv[i][0] == '.')
	{
	    sendaddress[doomcom->numnodes].sin_addr.s_addr 
		= inet_addr (myargv[i]+1);
	}
	else
	{
	    hostentry = gethostbyname (myargv[i]);
	    if (!hostentry)
		I_Error ("gethostbyname: couldn't find %s", myargv[i]);
	    sendaddress[doomcom->numnodes].sin_addr.s_addr 
		= *(int32_t *)hostentry->h_addr_list[0];
	}
	doomcom->numnodes++;
    }
	
    doomcom->id = DOOMCOM_ID;
    doomcom->numplayers = doomcom->numnodes;
    
    // build message to receive
    insocket = UDPsocket ();
    BindToLocalPort (insocket,htons(DOOMPORT));
    ioctl (insocket, FIONBIO, &trueval);

    sendsocket = UDPsocket ();
}


void I_NetCmd (void)
{
    if (doomcom->command == CMD_SEND)
    {
	netsend ();
    }
    else if (doomcom->command == CMD_GET)
    {
	netget ();
    }
    else
	I_Error ("Bad net cmd: %i\n",doomcom->command);
}

