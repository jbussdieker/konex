/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: sys_linux.cpp,v 1.9 2005/07/27 10:42:33 ecco Exp $   *
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
int main( int argc, char **argv )
{
	// Check for too many argumentsd
	if (argc == 1)
	{
		// Copy app path variable from argument 0
		char *lastslash;
		strcpy(sysAppPath, argv[0]);
		lastslash = strrchr(sysAppPath, '/') + 1;
		sysAppPath[lastslash - sysAppPath] = '\0';
		
		//sprintf(sysAppPath, "%s%s", sysAppPath, "../../dist/");
		//sprintf(sysAppPath, "%s", "");
	}
	else if (argc == 2)
	{
		// Copy app path variable from argument 1
		strcpy(sysAppPath, argv[1]);
	}
	else
	{
		console_Print("main(): Arguments are not currently supported");
		int i;
		for (i = 0; i < argc; i++)
			console_Print(" %s", argv[i]);
	}

	// Run system initialization
	sys_Init();

	// Run system shutdown
	sys_Shutdown();
}

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
