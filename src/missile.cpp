/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: missile.cpp,v 1.24 2005/08/14 08:12:27 ecco Exp $   *
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
#define MISSILE_SPEED 2000.0f

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////
void missileCollision(entities_entity_t *source, entities_entity_t *collision)
{
	if (collision->etype == ENTITY_PLAYER &&          // Of Player Type
		collision != source->player &&        // Not colliding with self
		collision->landed == false &&                 // Not currently landed
		collision->team != source->player->team) // Not on owners team (friendly fire)
	{
		// Run player proccessing for collision
		player_MissileCollision(collision, source);

		// Eliminate the current missile the has collided
		source->used = false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void missile_Init()
{
	missile_Clear();
}

void missile_DrawMissile(entities_entity_t *missile)
{
	if (missile->used == false)
		return;

	int drawx, drawy;

	vgvec2 spos;
	spos = view_WorldToScreenCoord(&missile->pos);

	drawx = int(spos.x);
	drawy = int(spos.y);

	vid_color_t missilecolor;

	missilecolor = vid_Color(192, 192, 192);

	vid_Rect(drawx, drawy, 4, 4, missilecolor, 192);
}

void missile_Clear()
{
	//memset(&entities, 0, sizeof(entities));
}

void missile_Spawn(entities_entity_t *player)
{
	entities_entity_t *newmissile;
	newmissile = entities_Create(ENTITY_MISSILE);

	newmissile->pos.x = player->pos.x;
	newmissile->pos.y = player->pos.y;
	newmissile->velocity.x = player->velocity.x;
	newmissile->velocity.y = player->velocity.y;

	vgvec2 headingnormal;
	headingnormal.x = sin(player->heading * PI / 180);
	headingnormal.y = cos(player->heading * PI / 180);


	newmissile->velocity += headingnormal * 2000;

	newmissile->heading = player->heading;
	newmissile->player = player;
	newmissile->ttl = 2.0f;
	newmissile->damage = player->damage;

	newmissile->used = true;
}

void missile_ProcessInGame(entities_entity_t *missile)
{
	entities_EnumerateCollisions(missile, missileCollision);

	missile->ttl -= tm_FrameTime;
	if (missile->ttl < 0.0f)
		missile->used = false;
}
