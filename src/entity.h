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
#define ENTITY_PLAYER 1
#define ENTITY_EXPLOSION 2
#define ENTITY_MISSILE 3
#define ENTITY_PLANET 4
#define ENTITY_SPAWN 5
#define ENTITY_ITEM 6

struct entities_entity_t
{
	vgvec2 		size;
	vgvec2 		pos;
	vgvec2 		velocity;
	float 		heading;
	dword 		shipid;
	dword		modelid;
	bool 		used;
	int 		type;
	int 		etype;
	int 		crewcount;
	int 		team;
	bool 		autotarget;
	int 		damage;
	int 		deaths;
	int 		health;
	int			money;
	int 		frags;
	float 		ttl;
	float 		firettl;
	float 		rechargettl;
	bool 		landed;
	dword 		planetid;
	char 		name[16];
	entities_entity_t 	*player;
	entities_entity_t 	*target;

	// AI Storage
	float		destx, desty;
	
	// Control Functions
	void (*processai)(entities_entity_t*);
	void (*turnleft)(entities_entity_t*);
	void (*turnright)(entities_entity_t*);
	void (*accelerate)(entities_entity_t*);
	void (*deccelerate)(entities_entity_t*);
	void (*slow)(entities_entity_t*);
	void (*fire)(entities_entity_t*);
};

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Functions
/////////////////////////////////////////////////////////////////////////////
entities_entity_t	*entities_GetEntity(unsigned int id);
entities_entity_t	*entities_CreateEntity();
entities_entity_t	*entities_Create(int type);

void				entities_EnumerateCollisions(entities_entity_t *source, void (*callback)(entities_entity_t *source, entities_entity_t *entity));
void				entities_Enumerate(void (*callback)(entities_entity_t *entity, void *data), void *data);

unsigned int		entities_GetID(entities_entity_t* player);
unsigned int		entities_GetFreeID();
void				entities_DrawEntities();
void				entities_ProcessMovement();
void				entities_DebugList();
