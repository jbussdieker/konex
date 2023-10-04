/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: konex.cpp,v 1.19 2005/08/14 06:56:45 ecco Exp $               *
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
unsigned int	konexBackground;

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////
static void fadeIn()
{
	static bool	konexFadeIn = true;
	static float konexFadeInStart = timer_FloatTime();
	int backalpha;

	if (konexFadeIn == true)
	{
		if (timer_FloatTime() - konexFadeInStart > 0.5)
		{
			konexFadeIn = false;
			backalpha = 255;
		}
		else
			backalpha = (unsigned int)((timer_FloatTime() - konexFadeInStart) * 511);
	}
	else
		backalpha = 255;

	// If there is no current game running then draw the background
	if (gameRunning == false)
		vid_DrawImage(0, 0, vidWidth, vidHeight, backalpha, konexBackground, false);
}

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void konex_Init()
{
	// Generic Init
	char file[128];
	sprintf(file, "%s%s", sysAppPath, "data/logos/conex.tga");
	konexBackground = vid_LoadImage(file);
	
	interface_Init();

	// Game Init
	game_Init();

	menu_Init(false);

	// Print complete message
	console_Print("**************************************************************************");
	console_Print("* Game initialization complete...                                        *");
	console_Print("**************************************************************************");
}

void konex_Frame()
{
	fadeIn();

	// Generic Frame
	interface_Input();

	// Game Frame
	game_Frame();

	// Interface Frame
	interface_Frame();
}

void konex_Shutdown()
{
	// Print complete message
	console_Print("**************************************************************************");
	console_Print("* Game shutdown initiated...                                             *");
	console_Print("**************************************************************************");

	// Save game configuration
	config_Save();

	// Shutdown the system
	sys_Shutdown();
}
