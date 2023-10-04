/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: item.cpp,v 1.4 2005/08/15 06:15:38 ecco Exp $           *
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
static unsigned int moneysprite;
static unsigned int healthsprite;

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////
void itemCollision(entities_entity_t *source, entities_entity_t *collision)
{
	if (collision->etype == ENTITY_PLAYER &&
		collision->landed == false)
	{
		// Run player proccessing for collision
		player_ItemCollision(collision, source);

		// Eliminate the current missile the has collided
		source->used = false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void item_Init()
{
	char filename[MAX_PATH];
	sprintf(filename, "%sdata/items/money.tga", sysAppPath);
	moneysprite = vid_LoadImage(filename);
	sprintf(filename, "%sdata/items/health.tga", sysAppPath);
	healthsprite = vid_LoadImage(filename);
}

void item_SpawnFromPlayer(entities_entity_t *player)
{
	if (sys_Rand(1, 5) != 1)
		return;

	entities_entity_t *newitem;
	newitem = entities_Create(ENTITY_ITEM);

	newitem->used = true;
	newitem->pos = player->pos;
	newitem->velocity = player->velocity;
	newitem->ttl = 15.0f;

	newitem->money = sys_Rand(1, 25);
	//player->money = 0;

	newitem->type = sys_Rand(1, 2);
}

void item_ProcessInGame(entities_entity_t *item)
{
	entities_EnumerateCollisions(item, itemCollision);

	if (item->used == true)
	{
		item->ttl -= tm_FrameTime;
		if (item->ttl <= 0.0f)
			item->used = false;
	}
}

void item_DrawItem(entities_entity_t *item)
{
	vgvec2 spos;
	int drawx, drawy;

	spos = view_WorldToScreenCoord(&item->pos);
	drawx = int(spos.x);
	drawy = int(spos.y);

	if (item->type == 1)
		vid_DrawImage(drawx, drawy, healthsprite, true);
	else if (item->type == 2)
		vid_DrawImage(drawx, drawy, moneysprite, true);
}
