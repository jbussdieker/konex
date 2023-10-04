/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: planets.cpp,v 1.14 2005/08/16 05:43:30 ecco Exp $   *
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
#define MAX_PLANETS 19
#define PLANET_COUNT 18

struct planets_planet_t
{	
	dword imageid;
	dword viewimageid;
	bool loaded;
};

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////
static planets_planet_t planets[MAX_PLANETS];
static unsigned int planetsCount;

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
unsigned int planets_PickRandom()
{
	dword randomposition = sys_Rand(1, planetsCount);
	dword i;
	dword curpos = 0;

	// Determine how many active planets there are
	for (i = 1; i < MAX_PLANETS; i++)
		if (planets[i].loaded == true)
		{
			curpos+=1;
			if (curpos == randomposition)
				return i;
		}

	return 0;
}

void planets_Init(void)
{
	dword i;
	char filename[128];

	for (i = 1; i <= PLANET_COUNT; i++)
	{	
		// Load planet sprite
		sprintf(filename, "%sdata/planets/%02d/pic.tga", sysAppPath, i - 1);
		planets[i].imageid = vid_LoadImage(filename);

		if (planets[i].imageid == 0)
			return;

		// Load 3D rendered view
		sprintf(filename, "%sdata/planets/%02d/view.tga", sysAppPath, i - 1);
		planets[i].viewimageid = vid_LoadImage(filename);

		planets[i].loaded = true;
		planetsCount += 1;
	}

	console_Print("PLANETS: Loaded %d planets...", PLANET_COUNT);
}

entities_entity_t *planets_SpawnPlanet(unsigned int planetid, int x, int y)
{
	entities_entity_t *newplanet;
	newplanet = entities_CreateEntity();

	newplanet->etype = ENTITY_PLANET;
	newplanet->planetid = planetid;
	newplanet->used = true;
	newplanet->pos.x = x;
	newplanet->pos.y = y;

	return newplanet;
}

void planets_DrawPlanet(entities_entity_t *planet)
{
	vgvec2 spos;
	spos = view_WorldToScreenCoord(&planet->pos);
	vid_DrawImage((int)spos.x, (int)spos.y, planets[planet->planetid].imageid, true);
}

void planets_ListPlanets()
{
	unsigned int i;

	for (i = 1; i <= PLANET_COUNT; i++)
	{
		if (planets[i].loaded == true)
		{
			console_Print("%02d Regular (%d) Scenic (%d)", i, planets[i].viewimageid, planets[i].imageid);
		}
	}
}
