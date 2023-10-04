/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: ships.h,v 1.8 2005/08/09 07:00:37 ecco Exp $   *
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
// Interface Definitions, Enumeration and Typedefs
/////////////////////////////////////////////////////////////////////////////
#define MAX_SHIPS 14

struct ships_ship_t
{
	char name[128];
	char foldername[128];
	unsigned int sprites[36];
	unsigned int target;
	unsigned int yard;
	unsigned int comm;
	float maxvelocity;			// km /sec
	float turnspeed;			// degrees / sec
	float acceleration;			// km / sec
	float mass;					// kg
	float collisionradius;		// pixels
	int damage;
	bool used;
};

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////
extern ships_ship_t ships[MAX_SHIPS];

/////////////////////////////////////////////////////////////////////////////
// Interface Function
/////////////////////////////////////////////////////////////////////////////
void 			ships_ListShips();
void 			ships_Init();
unsigned int 	ships_LoadShip(char *shipname);
void 			ships_UnloadShip(unsigned int id);
void			ships_SaveShips();
void 			ships_SaveShip(unsigned int id);
unsigned int 	ships_PickRandom();
void			ships_Load();
