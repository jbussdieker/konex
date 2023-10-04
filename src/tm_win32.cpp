/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: tm_win32.cpp,v 1.2 2005/07/30 01:02:14 ecco Exp $   *
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
///////////////////////////////////////////////////////////////
// Implementation Private Includes
///////////////////////////////////////////////////////////////
#include "defs.h"

///////////////////////////////////////////////////////////////
// Implementation Private Definitions, Enumeration and Typedefs
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
// Implementation Private Variables
///////////////////////////////////////////////////////////////
static __int64 timer_Start;
static __int64 timer_Current;
static __int64 timer_Freq;
static float timer_BaseTime;
static float timer_LastTime;

///////////////////////////////////////////////////////////////
// Interface Variables
///////////////////////////////////////////////////////////////
float tm_FrameTime;

///////////////////////////////////////////////////////////////
// Implementation Private Function Prototypes
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
// Implementation Private Functions
///////////////////////////////////////////////////////////////
static float TimeFloat()
{
	// Get the current count
	QueryPerformanceCounter((LARGE_INTEGER *)&timer_Current);
	
	// Calculate the count difference and return the converted (to seconds) value
	return (float)(timer_Current - timer_Start) / (float)(timer_Freq);
}

///////////////////////////////////////////////////////////////
// Interface Public Functions
///////////////////////////////////////////////////////////////
void timer_Init()
{
	BOOL result;

	// Get the frequency of the performance counter
	result = QueryPerformanceFrequency((LARGE_INTEGER *)&timer_Freq);
	if (!result)
	{
		console_Print("timer_Init()::QueryPerformanceFrequency returned an error");
	}

	// Get the initial value for the counter
	result = QueryPerformanceCounter((LARGE_INTEGER *)&timer_Start);
	if (!result)
	{
		console_Print("timer_Init()::QueryPerformanceCounter returned an error");
	}

	// Set the base and last time values
	timer_BaseTime = TimeFloat();
	timer_LastTime = TimeFloat();
}

void timer_Frame()
{
	tm_FrameTime = (TimeFloat() - timer_LastTime);
	timer_LastTime = TimeFloat();
}

float timer_FloatTime()
{
	return TimeFloat();
}

void timer_Shutdown()
{
}

void timer_ShowFPS()
{
	console_Print("- %0.2f frames/sec", 1.0 / tm_FrameTime);
	console_Print("- last frame rendered in %0.6f second(s)", tm_FrameTime);
}
