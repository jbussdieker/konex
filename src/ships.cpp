/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: ships.cpp,v 1.11 2005/08/14 06:56:45 ecco Exp $   *
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
ships_ship_t ships[MAX_SHIPS];
unsigned int shipsCount;

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////
static unsigned int getFreeShip()
{
	int i;
	for (i = 1; i < MAX_SHIPS; i++)
	{
		if (ships[i].used == false)
			return i;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
unsigned int ships_PickRandom()
{
	dword randomposition = sys_Rand(1, shipsCount);
	dword i;
	dword curpos = 0;

	for (i = 1; i < MAX_SHIPS; i++)
		if (ships[i].used == true)
		{
			curpos+=1;
			if (curpos == randomposition)
				return i;
		}

	return 0;
}

void ships_ListShips()
{
	unsigned int i;

	for (i = 1; i < MAX_SHIPS; i++)
	{
		if (ships[i].used == true)
		{
			console_Print("%02d %s Sprites (%d-%d)", i, ships[i].name, ships[i].sprites[0], ships[i].sprites[35]);
			console_Print(" Target (%d) Yard (%d) Comm (%d)", ships[i].target, ships[i].yard, ships[i].comm);
		}
	}
}

void ships_Init()
{
	memset(&ships, 0, sizeof(ships));

	ships_LoadShip("advisor");
	ships_LoadShip("callisto");
	ships_LoadShip("citrus");
	ships_LoadShip("dart");
	ships_LoadShip("defender");
	ships_LoadShip("gryphon");
	ships_LoadShip("large pin");
	ships_LoadShip("necromancer");
	ships_LoadShip("small pin");
	ships_LoadShip("tomaquad");
	ships_LoadShip("trident");
	ships_LoadShip("yodacon");

	console_Print("SHIPS: Loaded 12 ships.");
}

unsigned int ships_LoadShip(char *foldername)
{
	int i;
	unsigned int id;
	id = getFreeShip();
	ships[id].used = true;
	char filename[256];

	strcpy(ships[id].foldername, foldername);		

	for (i = 0; i < 36; i++)
	{
		// open Data\Ships\[shipname]\[spritenumber].bmp
		sprintf(filename, "%sdata/ships/%s/%02d.tga", sysAppPath, ships[id].foldername, i);
		ships[id].sprites[i] = vid_LoadImage(filename);

		// Check to see if the sprite was sucessfully loaded
		if (ships[id].sprites == 0)
		{
			return 0;
		}
	}

	sprintf(filename, "%sdata/ships/%s/target.tga", sysAppPath, ships[id].foldername);
	ships[id].target = vid_LoadImage(filename);
	sprintf(filename, "%sdata/ships/%s/yard.tga", sysAppPath, ships[id].foldername);
	ships[id].yard = vid_LoadImage(filename);
	sprintf(filename, "%sdata/ships/%s/comm.tga", sysAppPath, ships[id].foldername);
	ships[id].comm = vid_LoadImage(filename);

	if (ships[id].target == 0 || ships[id].yard == 0 || ships[id].comm == 0)
		return 0;

	sprintf(filename, "%sdata/ships/%s/specs.xml", sysAppPath, ships[id].foldername);

	// Document
	TiXmlDocument *xmlDoc = new TiXmlDocument(filename);
	if (!xmlDoc->LoadFile())
	{
		console_Print("SHIPS: Failed to load %s", filename);
		return 0;
	}

	TiXmlElement *xmlElementShip = 0;
	xmlElementShip = xmlDoc->FirstChildElement("Ship");
	if (!xmlElementShip)
	{
		console_Print("SHIPS: Couldn't locate ship element in %s", filename);
		xmlDoc->Clear();
		delete xmlDoc;
		return 0;
	}

	xml_GetSetting(xmlElementShip, "ShipName", ships[id].name);
	xml_GetSetting(xmlElementShip, "MaxVelocity", &ships[id].maxvelocity);
	xml_GetSetting(xmlElementShip, "TurnSpeed", &ships[id].turnspeed);
	xml_GetSetting(xmlElementShip, "Acceleration", &ships[id].acceleration);
	xml_GetSetting(xmlElementShip, "Mass", &ships[id].mass);
	xml_GetSetting(xmlElementShip, "CollisionRadius", &ships[id].collisionradius);
	xml_GetSetting(xmlElementShip, "Damage", &ships[id].damage);

	shipsCount += 1;

	return id;
}

void ships_SaveShips()
{
	dword i;

	for (i = 1; i < MAX_SHIPS; i++)
	{
		ships_SaveShip(i);
	}
}

void ships_SaveShip(unsigned int id)
{
	if (ships[id].used != true)
	{
		console_Print("SHIPS: Invalid ship to save.");
		return;
	}

	char filename[256];

	sprintf(filename, "%sdata/ships/%s/specs.xml", sysAppPath, ships[id].foldername);

	// Document
	TiXmlDocument *xmlDoc = new TiXmlDocument();
		// Config element
		TiXmlNode *xmlElementShip = new TiXmlElement("Ship");
			xml_SaveSetting(xmlElementShip, "ShipName", ships[id].name);
			xml_SaveSetting(xmlElementShip, "MaxVelocity", ships[id].maxvelocity);
			xml_SaveSetting(xmlElementShip, "TurnSpeed", ships[id].turnspeed);
			xml_SaveSetting(xmlElementShip, "Acceleration", ships[id].acceleration);
			xml_SaveSetting(xmlElementShip, "Mass", ships[id].mass);
			xml_SaveSetting(xmlElementShip, "CollisionRadius", ships[id].collisionradius);
			xml_SaveSetting(xmlElementShip, "Damage", ships[id].damage);
		xmlDoc->InsertEndChild(*xmlElementShip);
	xmlDoc->SaveFile(filename);

	delete xmlElementShip;
	delete xmlDoc;

	console_Print("SHIPS: Ship (%s) saved.", ships[id].name);
}

void ships_UnloadShip(unsigned int id)
{
	shipsCount -= 1;
	ships[id].used = false;
}
