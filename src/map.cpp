/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: map.cpp,v 1.18 2005/08/14 06:56:45 ecco Exp $                 *
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
/////////////////////////////////////////////////////////////////////////////
// Implementation Private Includes
/////////////////////////////////////////////////////////////////////////////
#include "defs.h"

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Definitions, Enumeration and Typedefs
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////
int mapWidth = 10000;
int mapHeight = 10000;

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////
static void drawEntity(entities_entity_t *entity, void *data)
{
	// Pull window data from the callback
	window *currentWin = (window *)data;
	if (currentWin == NULL)
		return;

	// Make meausurements
	int mapx, mapy;
	int mapwidth, mapheight;
	int px, py;
	float mapscalex, mapscaley;

	mapx = currentWin->x; mapy = currentWin->y;
	mapwidth = currentWin->width; mapheight = currentWin->height;

	mapscalex = mapWidth / mapwidth;
	mapscaley = mapHeight / mapheight;

	// Setup colors
	vid_color_t planetcolor = vid_Color(128, 128, 128);
	vid_color_t targetcolor = vid_Color(255, 0, 0);
	vid_color_t playercolor = vid_Color(255, 255, 255);
	vid_color_t shipcolor;

	if (entity->etype != ENTITY_SPAWN)
	{
 		switch (entity->team)
		{
			case 1:
				shipcolor = vid_Color(255, 0, 0);
				break;
			case 2:
				shipcolor = vid_Color(0, 255, 0);
				break;
			case 3:
				shipcolor = vid_Color(0, 0, 255);
				break;
			default:
				shipcolor = vid_Color(96, 96, 96);
				break;
		}
		
		px = mapx + int(entity->pos.x / mapscalex);
		py = mapy - int(entity->pos.y / mapscaley) + mapheight;
	
		if (entity == viewPlayer->target)
		{
			vid_Rect(px, py, 2, 2, targetcolor, 255);
		}
		else if (entity->etype == ENTITY_PLANET)
		{
			vid_Rect(px, py, 3, 3, planetcolor, 255);
		}
		else if (entity == gameMainPlayer)
		{
			vid_Rect(px, py, 3, 3, playercolor, 255);
		}
		else
		{
			vid_Rect(px, py, 2, 2, shipcolor, 255);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void map_Create(int width, int height)
{
	mapWidth = width;
	mapHeight = height;
}

void map_DrawMap(window *win)
{
	entities_Enumerate(drawEntity, (void *)win);
}
