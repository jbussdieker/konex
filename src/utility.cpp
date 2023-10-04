/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: utility.cpp,v 1.5 2005/07/24 01:29:45 ecco Exp $   *
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
void drawFPS()
{
	static float lasttime = 0.0f;
	static float lastfps = 0.0f;
	
	if (timer_FloatTime() - lasttime > 1.0)
	{
		lasttime = timer_FloatTime();
		lastfps = 1.0 / tm_FrameTime;
	}

	vid_DrawString(10, vidHeight - 30, "FPS: %0.2f", lastfps);
}

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void utility_Frame()
{
	if (config.ShowFPS == 1)
		drawFPS();
}
