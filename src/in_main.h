/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker   *
 *   jbussdieker@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifdef LINUX
	#include "in_linux.h"
#else
	#include "in_win.h"
#endif

enum in_event_type
{
	IN_KEYPRESS = 1,
	IN_KEYRELEASE = 2
};

struct in_event_t
{
	in_event_type type; // Key press, Key down, Key up
	unsigned int key; // Key code or mouse button
	bool handled;
	bool remove;

	int x, y; // Mouse x and y
};

void in_Init();
dword in_CreateHandler(void (*callback)(in_event_t*));
void in_Frame();
char in_ASCII();
void in_Shutdown();
