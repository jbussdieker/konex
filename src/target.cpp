/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: target.cpp,v 1.19 2005/08/14 03:54:31 ecco Exp $   *
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

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void target_DrawDisplay(void)
{
	if (viewPlayer->target != NULL)
	{
		int drawx;
		int drawy; 
		int imageid;
	
		imageid = ships[viewPlayer->target->shipid].sprites[0];
	
		vgvec2 spos;
		spos = view_WorldToScreenCoord(&viewPlayer->target->pos);
	
		drawx = (int)spos.x - vid_GetImageWidth(imageid) / 2;
		drawy = (int)spos.y - vid_GetImageHeight(imageid) / 2;
	
	
//		if (drawx > vidWidth || drawy > vidHeight ||
//			drawx + vid_GetImageWidth(imageid) < 0 || drawy + vid_GetImageHeight(imageid) < 0)
		{
			drawx += vid_GetImageWidth(imageid) / 2;
			drawy += vid_GetImageHeight(imageid) / 2;
			vid_Line(drawx, drawy, vidWidth / 2, vidHeight / 2, vid_Color(128, 0, 0));
			drawx -= vid_GetImageWidth(imageid) / 2;
			drawy -= vid_GetImageHeight(imageid) / 2;
		}
//		else
		{
			vid_color_t white = vid_Color(255, 255, 255);
	
			vid_Line(drawx, drawy, drawx + 10, drawy, white);
			vid_Line(drawx, drawy, drawx, drawy + 10, white);
	
			drawx += vid_GetImageWidth(imageid);
	
			vid_Line(drawx, drawy, drawx - 10, drawy, white);
			vid_Line(drawx, drawy, drawx, drawy + 10, white);
	
			drawy += vid_GetImageHeight(imageid);
	
			vid_Line(drawx, drawy, drawx - 10, drawy, white);
			vid_Line(drawx, drawy, drawx, drawy - 10, white);
	
			drawx -= vid_GetImageWidth(imageid);
	
			vid_Line(drawx, drawy, drawx + 10, drawy, white);
			vid_Line(drawx, drawy, drawx, drawy - 10, white);
		}	
	}
}

void target_DrawWindow(window *win)
{
	if (viewPlayer->target != NULL)
	{
		if (viewPlayer->target->used == false)
			viewPlayer->target = NULL;

		unsigned int shipid = viewPlayer->target->shipid;
	
		vid_DrawImage(win->x, win->y, ships[shipid].comm, false);

		vid_DrawString(win->x, win->y + 140, 255, ships[shipid].name);
		vid_DrawString(win->x, win->y + 160, 255, "Health: %d", viewPlayer->target->health);
		vid_DrawString(win->x, win->y + 180, 255, "Crew: %d", viewPlayer->target->crewcount);
		
		vid_DrawString(win->x, win->y + 200, 255, "Team: %d", viewPlayer->target->team);
	
		vid_DrawImage(win->x, win->y + 190, ships[shipid].target, false);
	}
}
