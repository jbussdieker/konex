/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: player.cpp,v 1.51 2005/08/16 22:55:28 ecco Exp $   *
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
/*void process_Land(entities_entity_t* player)
{
	dword i;
	for (i = 1; i < MAX_PLANETS; i++)
	{
		if (planets[i].used == true)
		{
			if ((planets[i].pos - player->pos) < vgvec2(256.0f, 0.0f))
			{
				player->landed = !player->landed;
				player->planetid = i;

				if (entities_GetID(player) == gameMainPlayer)
					interface_SetPlanetStatus(player->landed);

				player->velocity.x = player->velocity.y = 0;

				return;
			}
		}
	}

	console_Print("Nothing to land on.");
}*/

static void process_TurnLeft(entities_entity_t* player)
{
	player->heading -= ships[player->shipid].turnspeed * tm_FrameTime;

	// If the direction is more or less than the angle limits then add or subtract accordingly
	if (player->heading < 0.0f)
		player->heading += 360.0f;
}

static void process_TurnRight(entities_entity_t* player)
{
	player->heading += ships[player->shipid].turnspeed * tm_FrameTime;

	// If the direction is more or less than the angle limits then add or subtract accordingly
	if (player->heading > 360.0f)
		player->heading -= 360.0f;
}

static void process_Thrust(entities_entity_t* player)
{
	float angle;

	angle = player->heading * PI / 180;

	// Force = Mass (kg) * Acceleration (km/s^2)
	float forcex, forcey;

	forcex = sin(angle) * ships[player->shipid].acceleration * tm_FrameTime;
	forcey = cos(angle) * ships[player->shipid].acceleration * tm_FrameTime;

	player->velocity.x += forcex;
	player->velocity.y += forcey;
}

static void process_Reverse(entities_entity_t* player)
{
	float angle;

	angle = player->heading * PI / 180;

	player->velocity.x -= sin(angle) * ships[player->shipid].acceleration * tm_FrameTime;
	player->velocity.y -= cos(angle) * ships[player->shipid].acceleration * tm_FrameTime;
}

static void process_Slow(entities_entity_t* player)
{
	player->velocity.x /=  1 + tm_FrameTime;
	player->velocity.y /=  1 + tm_FrameTime;
}

static void process_Fire(entities_entity_t* player)
{
	if (player->firettl < 0.0f)
	{
		player->firettl = 0.2f;
		missile_Spawn(player);
	}
}

static void process_PlayerInput(entities_entity_t *player, in_event_t *event)
{
	if (player->landed != true)
	{
		// Left arrow down
		if (event->key == IN_LEFTARROW)
		{
			process_TurnLeft(player);
			event->handled = true;
			player->autotarget = false;
		}
		
		// Right arrow down
		if (event->key == IN_RIGHTARROW)
		{
			process_TurnRight(player);
			event->handled = true;
			player->autotarget = false;
		}
	
		// Up arrow down
		if (event->key == IN_UPARROW)
		{
			process_Thrust(player);
			event->handled = true;
		}
		
		// Down arrow down
		if (event->key == IN_DOWNARROW)
		{
			process_Reverse(player);
			event->handled = true;
		}
	
		// 0 (ins) key
		if (event->key == IN_INSERT0)
		{
			process_Slow(player);
			event->handled = true;
		}
	
		if (event->key == IN_SPACEBAR)
		{
			process_Fire(player);
			event->handled = true;
		}
	
		if (event->key == IN_LSHIFT)
		{
			player->target = player_ClosestEnemy(gameMainPlayer);
			event->handled = true;
		}
	
		if (event->key == IN_LCTRL)
		{
			player->autotarget = !player->autotarget;
			event->handled = true;
			event->remove = true;
		}
	}

	if (event->key == IN_RCTRL)
	{
//		process_Land(player);
		event->handled = true;
		event->remove = true;
	}
}

static void entityInputCallback(entities_entity_t *player, void *data)
{
	// Pull input data from callback
	in_event_t *event = (in_event_t *)data;
	if (event == NULL)
		return;

	// Process input from local players
	if (player->etype == ENTITY_PLAYER && player->type == PLAYER_TYPE_LOCAL)
		process_PlayerInput(player, event);
}

static void playerInputHandler(in_event_t *event)
{
	entities_Enumerate(entityInputCallback, (void *)event);
}

void playerCollision(entities_entity_t *source, entities_entity_t *collision)
{
	if (collision->etype == ENTITY_PLAYER &&          // Of Player Type
		collision != source &&        // Not colliding with self
		collision->landed == false)
	{
		/*vgvec2 oldvel;
		oldvel = source->velocity;

		source->velocity = -oldvel + collision->velocity;
		collision->velocity = oldvel - collision->velocity;*/
	}
}

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void player_Init(void)
{
	player_Clear();
	in_CreateHandler(playerInputHandler);
	//vid_LoadModel("models/paul.x");
}

void player_Clear(void)
{
//	memset(entities, 0, sizeof(entities));
}

entities_entity_t *player_Create(unsigned int shipid)
{
	entities_entity_t *newplayer;
	newplayer = entities_Create(ENTITY_PLAYER);

	newplayer->shipid = shipid;
	newplayer->damage = ships[newplayer->shipid].damage;

	newplayer->pos.x = mapWidth / 2;
	newplayer->pos.y = mapHeight / 2;

	newplayer->crewcount = sys_Rand(1, 50);
	
	if (newplayer->team == 0)
		newplayer->team = sys_Rand(1, 3);

	newplayer->health = 100;
	newplayer->money = 1;

	newplayer->turnleft = &process_TurnLeft;
	newplayer->turnright = &process_TurnRight;
	newplayer->accelerate = &process_Thrust;
	newplayer->deccelerate = &process_Reverse;
	newplayer->slow = &process_Slow;
	newplayer->fire = &process_Fire;

	newplayer->used = true;

	return newplayer;
}

entities_entity_t *player_CreateMultiplayer()
{
	entities_entity_t *newplayer;

	newplayer = player_Create(config.PlayerShipID);

	strcpy(newplayer->name, config.PlayerName);
	newplayer->type = PLAYER_TYPE_REMOTE;

	return newplayer;
}

entities_entity_t *player_CreateSinglePlayer()
{
	entities_entity_t *newplayer;

	newplayer = player_Create(config.PlayerShipID);

	strcpy(newplayer->name, config.PlayerName);
	newplayer->type = PLAYER_TYPE_LOCAL;
	newplayer->team = config.Team;

	return newplayer;
}

entities_entity_t *player_CreateAI()
{
	entities_entity_t *newplayer;

	newplayer = player_Create(ships_PickRandom());

	//int newp = planets_PickRandom();
	
	//newplayer->destx =  planets[newp].pos.x;
	//newplayer->desty =  planets[newp].pos.y;
	
	strcpy(newplayer->name, "AI Player");
	
	switch(sys_Rand(1, 2))
	{
		case 1:
			newplayer->processai = &ai_Rabbies;
			break;
		case 2:
			newplayer->processai = &ai_Seige;
			break;
	}

	newplayer->type = PLAYER_TYPE_NPC;

	return newplayer;
}

void player_ChangeTeams(entities_entity_t *player, int team)
{
	if (player->used == true && player->etype == ENTITY_PLAYER)
	{
		player->team = team;
		console_Notify("%s is now on team %d", player->name, player->team);
	}
}

void player_ProcessDeath(entities_entity_t *player)
{
	// Check for player death
	if (player->health <= 0)
	{
		explosion_SpawnFromPlayer(player);
		item_SpawnFromPlayer(player);

		player->deaths += 1;
		
/*		int i;
		for (i = 1; i < ENTITY_COUNT; i++)
		{
			if (entities[i].used == true && player == entities[i].target)
				entities[i].target = NULL;
		}*/

		entities_entity_t *spawnpoint;

		spawnpoint = spawn_GetSpawnPoint(player->team);
		if (spawnpoint == NULL)
		{	
			console_Print("PLAYER: No suitable spawn point found for player using default.");
			player->pos.x = mapWidth / 2;
			player->pos.y = mapHeight / 2;
		}
		else
		{
			player->pos.x = spawnpoint->pos.x;
			player->pos.y = spawnpoint->pos.y;
		}

		player->velocity.x = 0;
		player->velocity.y = 0;

		player->heading = sys_Rand(1, 360);

		player->target = NULL;

		player->health = 100;
	}
}

static float nearestdist;
static entities_entity_t *nearest;
static void player_ClosestEnemyCallback(entities_entity_t *entity, void *data)
{
	float curdist;

	entities_entity_t *player = (entities_entity_t *)data;

	if (entity->used == true && 
		entity != player && 
		entity->team != player->team && 
		entity->landed == false &&
		entity->etype == ENTITY_PLAYER)
	{
		vgvec2 distance;
		curdist = (player->pos - entity->pos).Length();

		if (curdist < nearestdist)
		{
			nearestdist = curdist;
			nearest = entity;
		}
	}

}

entities_entity_t *player_ClosestEnemy(entities_entity_t *player)
{
	nearestdist = mapWidth;
	nearest = NULL;
	entities_Enumerate(player_ClosestEnemyCallback, (void *)player);
	return nearest;
}

void player_MissileCollision(entities_entity_t *player, entities_entity_t *missile)
{
	if (player == gameMainPlayer &&
		config.GodMode == true)
		return;

	player->health -= missile->damage;
	
	if (player->health <= 0)
	{
		// Increase missle owners frags count
		missile->player->frags += 1;
	
		if (missile->player->team == 1)
			gameRedTeam += 1;
		if (missile->player->team == 2)
			gameGreenTeam += 1;
		if (missile->player->team == 3)
			gameBlueTeam += 1;
	
		// Print console message
		console_Notify("%s killed %s", missile->player->name, player->name);
	
		// Process player death in player module
		player_ProcessDeath(player);
	}
}

void player_ItemCollision(entities_entity_t *player, entities_entity_t *item)
{
	if (item->type == 1)
	{
		if (player->health < 100)
			player->health++;
	}
	else if (item->type ==2)
		player->money += item->money;
}

void player_DrawPlayer(entities_entity_t *player)
{
	unsigned int imageid;
	int drawx;
	int drawy; 
		
	imageid = ships[player->shipid].sprites[(int)(player->heading / 10)];

	vgvec2 spos;
	spos = view_WorldToScreenCoord(&player->pos);

	drawx = int(spos.x);
	drawy = int(spos.y);
	
	//if (player->type == PLAYER_TYPE_LOCAL)
		//vid_DrawModel(drawx, drawy, player->heading, player->team, 1);
	//else
		vid_DrawImage(drawx, drawy, imageid, true);

	// TODO Clean up this section
	drawx -= vid_GetImageWidth(imageid) / 2;
	drawy -= vid_GetImageHeight(imageid) / 2;

	drawy += vid_GetImageHeight(imageid);
	drawx += vid_GetImageWidth(imageid) / 2 - 25;

	vid_color_t highlight, normal;
	if (player->team == 1)
	{
		highlight = vid_Color(255, 0, 0);
		normal = vid_Color(128, 0, 0);
	}
	else if (player->team == 2)
	{
		highlight = vid_Color(0, 255, 0);
		normal = vid_Color(0, 128, 0);
	}
	else if (player->team == 3)
	{
		highlight = vid_Color(0, 0, 255);
		normal = vid_Color(0, 0, 255);
	}

	vid_Rect(drawx, drawy, 50, 5, normal, 64);
	vid_Rect(drawx, drawy, player->health / 2, 5, highlight, 64);
	// END TODO
}

void player_ProcessInGame(entities_entity_t *player)
{
	// Check for collisions with other players
	entities_EnumerateCollisions(player, playerCollision);

	// Check for max velocity
	float curvel = player->velocity.Length();
	if (curvel > ships[player->shipid].maxvelocity)
	{
		vgvec2 velnormal;

		velnormal = player->velocity;
		velnormal.Normalize();
		velnormal *= ships[player->shipid].maxvelocity;
		player->velocity = velnormal;
	}

	// Check for map boundries
	if (player->pos.x < 0.0f)
	{
		player->velocity.x = -player->velocity.x;
		player->pos.x = 0.0f;
	}
	if (player->pos.y < 0.0f)
	{
		player->velocity.y = -player->velocity.y;
		player->pos.y = 0.0f;
	}
	if (player->pos.x > mapWidth)
	{	
		player->velocity.x = -player->velocity.x;
		player->pos.x = mapWidth;
	}
	if (player->pos.y > mapHeight)
	{
		player->velocity.y = -player->velocity.y;
		player->pos.y = mapHeight;
	}

	player->rechargettl -= tm_FrameTime;
	player->firettl -= tm_FrameTime;

	if (player->rechargettl < 0.0f)
	{
		if (player->health < 100)
			player->health += 1;
		
		player->rechargettl = 3.0f;
	}

	if (player->type == PLAYER_TYPE_LOCAL)
		if (player->autotarget == true && player->target != NULL)
			ai_FaceEntityAtPoint(player, player->target->pos.x, player->target->pos.y);
}
