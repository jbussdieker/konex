/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: ai.cpp,v 1.17 2005/08/14 06:56:45 ecco Exp $             *
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
static float find_Heading(float x, float y, entities_entity_t* player)
{
	float dx, dy;
	float targetangle;

	dx = x - player->pos.x;
	dy = y - player->pos.y;
	
	targetangle = atan(dx / dy) * 180 / PI;

	if (dx < 0 && dy > 0)
		targetangle += 360.0f;
	//else if (dx > 0 && dy > 0)
		//console_Print("Quad II");
	else if (dx < 0 && dy < 0)
		targetangle += 180.0f;
	else if (dx > 0 && dy < 0)
		targetangle += 180.0f;

	return targetangle;
}

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void ai_FaceEntityAtPoint(entities_entity_t *player, float x, float y)
{
	float targetangle;

	targetangle = find_Heading(x, y, player);
	int targetdelta = abs(int(targetangle - player->heading));

	// Make sure that the difference between angles is not less that the movement for this frame
	if (targetdelta < ships[player->shipid].turnspeed * tm_FrameTime)
	{
		player->heading = targetangle;
		return;
	}

	if (targetangle > player->heading)
	{
		if (targetangle - player->heading < 180)
			player->turnright(player);
		else
			player->turnleft(player);
	}
	else
	{
		if (player->heading - targetangle < 180)
			player->turnleft(player);
		else
			player->turnright(player);
	}
}

void ai_TourPlanets(entities_entity_t *player)
{
	float proximity;

	proximity = (vgvec2(player->destx, player->desty) - player->pos).Length();

	if (proximity < 50)
	{
//		int newp = planets_PickRandom();
//		player->destx =  planets[newp].pos.x;
//		player->desty =  planets[newp].pos.y;
	}

	ai_FaceEntityAtPoint(player, player->destx, player->desty);

	if (proximity > 1250)
	{
		player->accelerate(player);
	}
	else if (proximity <= 1250 && proximity > 625)
	{
		if (player->velocity.Length() > 500.0f)
			player->slow(player);
		else
			player->accelerate(player);
	}
	else if (proximity <= 625 && proximity > 310)
	{
		if (player->velocity.Length() > 250.0f)
			player->slow(player);
		else
			player->accelerate(player);
	}
	else
	{
		if (player->velocity.Length() > 125.0f)
			player->slow(player);
		else
			player->accelerate(player);
	}
}

void ai_TourGalaxy(entities_entity_t *player)
{
	float proximity;

	proximity = (vgvec2(player->destx, player->desty) - player->pos).Length();

	if (proximity < 50)
	{
		player->destx = sys_Rand(0, mapWidth);
		player->desty = sys_Rand(0, mapHeight);
	}

	ai_FaceEntityAtPoint(player, player->destx, player->desty);

	if (proximity > 1250)
	{
		player->accelerate(player);
	}
	else if (proximity <= 1250 && proximity > 625)
	{
		if (player->velocity.Length() > 500.0f)
			player->slow(player);
		else
			player->accelerate(player);
	}
	else if (proximity <= 625 && proximity > 310)
	{
		if (player->velocity.Length() > 250.0f)
			player->slow(player);
		else
			player->accelerate(player);
	}
	else
	{
		if (player->velocity.Length() > 125.0f)
			player->slow(player);
		else
			player->accelerate(player);
	}
}

void ai_Rabbies(entities_entity_t *player)
{
	player->target = player_ClosestEnemy(player);

	if (player->target == NULL)
		return;

	float proximity;
	proximity = (player->target->pos - player->pos).Length();

	if (proximity < 1024)
	{
		player->fire(player);
	}

	ai_FaceEntityAtPoint(player, player->target->pos.x, player->target->pos.y);

	if (proximity > 512)
	{
		player->accelerate(player);
	}
}

void ai_Seige(entities_entity_t *player)
{
	player->target = player_ClosestEnemy(player);

	if (player->target == NULL)
		return;

	float proximity;
	proximity = (player->target->pos - player->pos).Length();

	if (proximity < 1024)
	{
		player->fire(player);
	}

	ai_FaceEntityAtPoint(player, player->target->pos.x, player->target->pos.y);

	if (proximity > 512)
	{
		player->accelerate(player);
	}
	else
	{
		player->slow(player);
	}
/*	else if (proximity <= 1250 && proximity > 625)
	{
		if (player->velocity.Length() > 500.0f)
			player->slow(player);
		else
			player->accelerate(player);
	}
	else if (proximity <= 625 && proximity > 310)
	{
		if (player->velocity.Length() > 250.0f)
			player->slow(player);
		else
			player->accelerate(player);
	}
	else
	{
		if (player->velocity.Length() > 125.0f)
			player->slow(player);
		else
			player->accelerate(player);
	}*/
}
