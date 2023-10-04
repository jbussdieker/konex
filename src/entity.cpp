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
#include "defs.h"
#define ENTITY_COUNT 400

static entities_entity_t entities[ENTITY_COUNT];

entities_entity_t* entities_GetEntity(unsigned int id)
{
    //if ((!id) || (id > ENTITY_COUNT))
        //return(NULL);
    //if (!entities[id-1].used)
    //if (!entities[id].used)
        //return(NULL);
    //return(&entities[id-1]);
    return(&entities[id]);
}

unsigned int entities_GetID(entities_entity_t* player)
{
    if (!player)
        return(0);
    //return((unsigned int)(player - players)+1);
    return((unsigned int)(player - entities));
}

entities_entity_t *entities_CreateEntity()
{
	unsigned int i;

	for (i = 1; i < ENTITY_COUNT; i++)
	{
		if (entities[i].used == false)
		{
			//memset(&entities[i], 0, sizeof(entities[i]));
			return &entities[i];
		}
	}

	console_Print("ENTITY: No free entities");
	return 0;
}

unsigned int entities_GetFreeID()
{
	unsigned int i;

	for (i = 1; i < ENTITY_COUNT; i++)
	{
		if (entities[i].used == false)
		{
			//memset(&entities[i], 0, sizeof(entities[i]));
			return i;
		}
	}

	console_Print("ENTITY: No free entities");
	return 0;
}

entities_entity_t *entities_Create(int type)
{
	entities_entity_t* newentity;
	newentity = entities_CreateEntity();
	newentity->used = true;
	newentity->etype = type;
	return newentity;
}

void entities_MoveEntities(entities_entity_t *player)
{
	player->pos += player->velocity * tm_FrameTime;
}

void entities_DebugInfo(entities_entity_t *debugentity, void *data)
{
	char type[16];

	switch (debugentity->etype)
	{
		case ENTITY_PLAYER:
			strcpy(type, "ENTITY_PLAYER");
			break;
		case ENTITY_EXPLOSION:
			strcpy(type, "ENTITY_EXPLOSION");
			break;
		case ENTITY_MISSILE:
			strcpy(type, "ENTITY_MISSILE");
			break;
		case ENTITY_PLANET:
			strcpy(type, "ENTITY_PLANET");
			break;
		case ENTITY_SPAWN:
			strcpy(type, "ENTITY_SPAWN");
			break;
		case ENTITY_ITEM:
			strcpy(type, "ENTITY_ITEM");
			break;
		default:
			strcpy(type, "ENTITY_UNKNOWN");
			break;
	}

	console_Print(" Entity Type: %s Position (%0.2f, %0.2f)", type, debugentity->pos.x, debugentity->pos.y);
}

void entities_EnumerateCollisions(entities_entity_t *entity, void (*callback)(entities_entity_t *, entities_entity_t *))
{
	dword i;

	if (entity->used == false)
		return;

	for (i = 1; i < ENTITY_COUNT; i++)
		if (entities[i].used == true)
			if ((entity->pos - entities[i].pos).Length() < 64.0f)
				callback(entity, &entities[i]);
}

void entities_Enumerate(void (*callback)(entities_entity_t *entity, void *data), void *data)
{
	dword i;

	for (i = 1; i < ENTITY_COUNT; i++)
		if (entities[i].used == true)
			callback(&entities[i], data);
}

void entities_DebugList()
{
	entities_Enumerate(entities_DebugInfo, NULL);
}

void entities_ProcessMovement()
{
	dword i;

	for (i = 1; i < ENTITY_COUNT; i++)
	{
		if (entities[i].used == true)
		{
			entities_MoveEntities(&entities[i]);

			switch (entities[i].etype)
			{
				case ENTITY_PLAYER:
					if (entities[i].type == PLAYER_TYPE_NPC)
						entities[i].processai(&entities[i]);
					player_ProcessInGame(&entities[i]);
					break;
				case ENTITY_EXPLOSION:
					explosion_ProcessInGame(&entities[i]);
					break;
				case ENTITY_MISSILE:
					missile_ProcessInGame(&entities[i]);
					break;
				case ENTITY_ITEM:
					item_ProcessInGame(&entities[i]);
					break;
			}
		}
	}
}

void entities_DrawEntities()
{
	dword i;

	for (i = 1; i < ENTITY_COUNT; i++)
	{
		if (entities[i].used == true)
		{
			switch (entities[i].etype)
			{
				case ENTITY_PLAYER:
					player_DrawPlayer(&entities[i]);
					break;
				case ENTITY_EXPLOSION:
					explosion_DrawExplosion(&entities[i]);
					break;
				case ENTITY_MISSILE:
					missile_DrawMissile(&entities[i]);
					break;
				case ENTITY_PLANET:
					planets_DrawPlanet(&entities[i]);
					break;
				case ENTITY_ITEM:
					item_DrawItem(&entities[i]);
					break;
			}
		}
	}
}
