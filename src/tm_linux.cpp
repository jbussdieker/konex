/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: tm_linux.cpp,v 1.6 2005/07/25 15:32:18 ecco Exp $   *
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
static struct timeval start;
static struct timeval t1, t2;

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////
float tm_FrameTime;

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void timer_Init(void)
{
	gettimeofday(&start, NULL);

	srand((unsigned int)start.tv_usec);

	timer_Frame();

	// Debugging
	console_Print("TIMER: Time system is operational.");
}

float timer_FloatTime(void)
{
	struct timeval current;
	float dt;

	gettimeofday(&current, NULL);

	/* 1000000 microseconds in a second... */
	dt = (current.tv_sec - start.tv_sec) + (current.tv_usec - start.tv_usec) / 1000000.0; 
	return dt;
}

void timer_Frame(void)
{
	double dt;

	gettimeofday(&t1, NULL);

	/* 1000000 microseconds in a second... */
	dt = (t1.tv_sec - t2.tv_sec) + (t1.tv_usec - t2.tv_usec) / 1000000.0; 

	gettimeofday(&t2, NULL);

	tm_FrameTime = dt;
}

void timer_ShowFPS()
{
	console_Print("- %0.2f frames/sec", 1.0 / tm_FrameTime);
	console_Print("- last frame rendered in %0.6f second(s)", tm_FrameTime);
}
