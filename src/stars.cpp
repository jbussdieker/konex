/***************************************************************************

 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *

 *   ecco@2nd-gen.net                                                      *

 *   $Id: stars.cpp,v 1.7 2005/08/14 06:56:45 ecco Exp $   *

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

struct stars_star_t

{

	vgvec2 pos;

	float speed;

	int spritenum;

};



/////////////////////////////////////////////////////////////////////////////

// Implementation Private Variables

/////////////////////////////////////////////////////////////////////////////

static stars_star_t* stars;

static long starcount;



/////////////////////////////////////////////////////////////////////////////

// Interface Variables

/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////

// Implementation Private Functions

/////////////////////////////////////////////////////////////////////////////

static void drawStars()

{

	int i;

	vid_color_t curcolor;



	// Draw all the stars

	for (i = 1; i < starcount; i++)

	{

		curcolor = vid_Color(stars[i].spritenum, stars[i].spritenum, stars[i].spritenum);

		if (stars[i].spritenum > 0 && stars[i].spritenum < 128)

		{

			vid_PSet((long)stars[i].pos.x,(long)stars[i].pos.y, curcolor);

		}

		if (stars[i].spritenum > 128 && stars[i].spritenum < 160)

		{

			vid_PSet((long)stars[i].pos.x,(long)stars[i].pos.y, curcolor);

			vid_PSet((long)stars[i].pos.x+1,(long)stars[i].pos.y, curcolor);

		}

		if (stars[i].spritenum > 160 && stars[i].spritenum < 192)

		{

			vid_PSet((long)stars[i].pos.x,(long)stars[i].pos.y, curcolor);

			vid_PSet((long)stars[i].pos.x,(long)stars[i].pos.y+1, curcolor);

		}

		if (stars[i].spritenum > 192 && stars[i].spritenum < 256)

		{

			vid_PSet((long)stars[i].pos.x+1,(long)stars[i].pos.y,curcolor);

			vid_PSet((long)stars[i].pos.x+1,(long)stars[i].pos.y+1,curcolor);

			vid_PSet((long)stars[i].pos.x,(long)stars[i].pos.y,curcolor);

		}



	}

}



/////////////////////////////////////////////////////////////////////////////

// Interface Public Functions

/////////////////////////////////////////////////////////////////////////////

void stars_Init(unsigned long StarCount)

{

	// Allocate memory for stars

	starcount = StarCount;

	stars = new stars_star_t[starcount];



	// Randomly create all the stars

	int i;

	for (i = 1; i < starcount; i++)

	{

		stars[i].pos.x = sys_Rand(0, vidWidth);

		stars[i].pos.y = sys_Rand(0, vidHeight);

		

		stars[i].spritenum = sys_Rand(50, 200);

		

		stars[i].speed = stars[i].spritenum / 50;

	}



	console_Print("STARS: Created %d stars...", starcount);

}



void stars_Frame()

{

	static float lastposx, lastposy;

	static bool firsttime = true;

	double diffX;

	double diffY;

	int i;



	// Only run this code if this is the first frame

	if (firsttime)

	{

		// Set the last values to the current values so the movement

		// on the first frame where we don't have enough info is 0

		firsttime = false;

	}

	

	// Calculate movement from the last frame

	diffX = (viewX - lastposx) / 8.0;

	diffY = (viewY - lastposy) / 8.0;



	// Move all the stars

	for (i = 1; i < starcount; i++)

	{

		stars[i].pos.x -= diffX * stars[i].speed;

		stars[i].pos.y += diffY * stars[i].speed;

	}



	// Check all stars for clipping and reinitialize the ones that went offscreen

	for (i = 1; i < starcount; i++)

	{

		// Went off the left side of the screen

		if (stars[i].pos.x < 0.0)

			stars[i].pos.x += vidWidth;

		// Went off the right side of the screen

		if (stars[i].pos.x > vidWidth)

			stars[i].pos.x -= vidWidth;

		// Went off the top side of the screen

		if (stars[i].pos.y < 0.0)

			stars[i].pos.y += vidHeight;

		// Went off the bottom side of the screen

		if (stars[i].pos.y > vidHeight)

			stars[i].pos.y -= vidHeight;

	}



	// Store current values as last values

	lastposx = viewX;

	lastposy = viewY;



	drawStars();

}



void stars_SetCount(long StarCount)

{

	// If we need more stars...

	if (StarCount > starcount)

	{

		stars_star_t* NewStarsArray;

		

		// Create the new star memory and initialize it to 0

		NewStarsArray = new stars_star_t[StarCount];

		memset(NewStarsArray, 0, sizeof(stars_star_t) * StarCount);



		// Copy old star to the begining of the star array and delete it

		memcpy(NewStarsArray, stars, sizeof(stars_star_t) * starcount);

		delete stars;

		stars = NewStarsArray;



		// Fill in the empty star space

		int i;

		for (i = starcount; i < StarCount; i++)

		{

			switch (sys_Rand(1, 4))

			{

				case 1:

					// Create on the right side of the screen

					stars[i].pos.x = vidWidth;

					stars[i].pos.y = sys_Rand(0, vidHeight);

				case 2:

					// Create on the left side of the screen

					stars[i].pos.x = 0.0;

					stars[i].pos.y = sys_Rand(0, vidHeight);

				case 3:

					// Create on the bottom of the screen

					stars[i].pos.x = sys_Rand(0, vidWidth);

					stars[i].pos.y = vidHeight;

				case 4:

					// Create on the top of the screen

					stars[i].pos.x = sys_Rand(0, vidWidth);

					stars[i].pos.y = 0.0;

			}



			stars[i].speed = sys_Rand(10, 80) / 10.0f;

			stars[i].spritenum = sys_Rand(0, 255);

		}



		starcount = StarCount;

	}

}



void stars_Term()

{

	// Deallocate memory

	delete stars;



	// Release all star sprites used

	//int i;

	//for (i = 0; i < STARS_SPRITE_COUNT; i++)

		//video_FreeImage(starsprites[i]);

}

