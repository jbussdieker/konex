/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: config.h,v 1.6 2005/08/06 06:29:56 ecco Exp $   *
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
struct config_setting_t
{
	char PlayerName[128];
	int StarCount;
	bool ShowFPS;
	bool GodMode;
	int AICount;
	bool ShowHUD;
	bool ShowTarget;
	bool ShowMiniMap;
	int PlayerShipID;
	char ServerAddress[128];
	int Team;
};

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////
extern 	config_setting_t 	config;

/////////////////////////////////////////////////////////////////////////////
// Interface Function
/////////////////////////////////////////////////////////////////////////////
void config_Init(char *filename);
void config_Save();
void config_Load();
