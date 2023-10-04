/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: explosion.cpp,v 1.11 2005/08/14 08:39:35 ecco Exp $           *
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
static dword sprites[18];

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void explosion_Init()
{
	char filename[128];
	dword i;
	for (i = 0; i < 17; i++)
	{
		// Load planet sprite
		sprintf(filename, "%sdata/explosions/%02d.tga", sysAppPath, i);
		sprites[i] = vid_LoadImage(filename);
		if (sprites[i] == 0)
			break;
	}

	console_Print("EXPLOSIONS: Loaded %d explosions...", i);

	explosion_Clear();
}

void explosion_Clear()
{
//	memset(&explosions, 0, sizeof(explosions));
}

void explosion_SpawnFromPlayer(entities_entity_t *player)
{
	entities_entity_t *newexplosion;
	newexplosion = entities_CreateEntity();

	newexplosion->etype = ENTITY_EXPLOSION;

	newexplosion->used = true;
	newexplosion->pos = player->pos;
	//newexplosion->pos.x += vid_GetImageWidth(ships[player->shipid].sprites[0]) / 2;
	//newexplosion->pos.y -= vid_GetImageWidth(ships[player->shipid].sprites[0]) / 2;

	newexplosion->size.x = vid_GetImageWidth(ships[player->shipid].sprites[0]);
	newexplosion->size.y = vid_GetImageWidth(ships[player->shipid].sprites[0]);
	newexplosion->velocity = player->velocity;
	newexplosion->ttl = 1.0f;
}

void explosion_ProcessInGame(entities_entity_t *explosion)
{
	if (explosion->used == true)
	{
		explosion->ttl -= tm_FrameTime;
		if (explosion->ttl <= 0.0f)
			explosion->used = false;
	}
}

void explosion_DrawExplosion(entities_entity_t *explosion)
{
	vgvec2 spos;
	int drawx, drawy;

	spos = view_WorldToScreenCoord(&explosion->pos);
	drawx = int(spos.x);
	drawy = int(spos.y);

	drawx -= int(explosion->size.x) / 2;
	drawy -= int(explosion->size.y) / 2;

	
	int spriteinex = int(-explosion->ttl * 16.0f) + 16;

	if (spriteinex < 0 || spriteinex > 16)
		console_Print("%d", spriteinex);

	vid_DrawImage(drawx, drawy, int(explosion->size.x), int(explosion->size.y), 128, sprites[spriteinex], true);
	vid_DrawImage(drawx + spriteinex, drawy + spriteinex, int(explosion->size.x), int(explosion->size.y), 32, sprites[spriteinex], true);
	vid_DrawImage(drawx - spriteinex, drawy - spriteinex, int(explosion->size.x), int(explosion->size.y), 32, sprites[spriteinex], true);
	vid_DrawImage(drawx + spriteinex, drawy - spriteinex, int(explosion->size.x), int(explosion->size.y), 32, sprites[spriteinex], true);
	vid_DrawImage(drawx - spriteinex, drawy + spriteinex, int(explosion->size.x), int(explosion->size.y), 32, sprites[spriteinex], true);
	//vid_Rect(drawx, drawy, int(explosion->size.x), int(explosion->size.y), vid_Color(32, 32, 32), 255);
	//vid_DrawString(drawx, drawy, 255, "%f", explosion->ttl);
}
