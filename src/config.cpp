/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: config.cpp,v 1.13 2005/08/10 04:11:52 ecco Exp $           *
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
static char configFileName[128];

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////
config_setting_t config;

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void config_Init(char *filename)
{
	sprintf(configFileName, "%s%s", sysAppPath, filename);
	console_Print("CONFIG: Using configuration file (%s)", configFileName);
}

void config_Save()
{
	// Document
	TiXmlDocument *xmlDoc = new TiXmlDocument("testing");
		// Config element
		TiXmlNode *xmlElementConfig = new TiXmlElement("Config");
			xml_SaveSetting(xmlElementConfig, "PlayerName", config.PlayerName);
			xml_SaveSetting(xmlElementConfig, "StarCount", config.StarCount);
			xml_SaveSetting(xmlElementConfig, "ShowFPS", config.ShowFPS);
			xml_SaveSetting(xmlElementConfig, "GodMode", config.GodMode);
			xml_SaveSetting(xmlElementConfig, "ShowHUD", config.ShowHUD);
			xml_SaveSetting(xmlElementConfig, "ShowTarget", config.ShowTarget);
			xml_SaveSetting(xmlElementConfig, "ShowMiniMap", config.ShowMiniMap);
			xml_SaveSetting(xmlElementConfig, "PlayerShipID", config.PlayerShipID);
			xml_SaveSetting(xmlElementConfig, "ServerAddress", config.ServerAddress);
			xml_SaveSetting(xmlElementConfig, "Team", config.Team);
		xmlDoc->InsertEndChild(*xmlElementConfig);
	xmlDoc->SaveFile(configFileName);

	delete xmlElementConfig;
	delete xmlDoc;

	console_Print("CONFIG: Configuration saved sucessfully", configFileName);
}

void config_Load()
{
	// Document
	TiXmlDocument *xmlDoc = new TiXmlDocument(configFileName);
	if (!xmlDoc->LoadFile())
	{
		console_Print("CONFIG: Failed to load %s", configFileName);
		return;
	}

	TiXmlElement *xConfig = 0;
	xConfig = xmlDoc->FirstChildElement("Config");
	if (!xConfig)
	{
		console_Print("CONFIG: Couldn't locate config element in %s", configFileName);
		xmlDoc->Clear();
		delete xmlDoc;
		return;
	}

	xml_GetSetting(xConfig, "PlayerName", config.PlayerName);
	xml_GetSetting(xConfig, "StarCount", &config.StarCount);
	xml_GetSetting(xConfig, "ShowFPS", (int *)&config.ShowFPS);
	xml_GetSetting(xConfig, "GodMode", (int *)&config.GodMode);
	xml_GetSetting(xConfig, "ShowHUD", (int *)&config.ShowHUD);
	xml_GetSetting(xConfig, "ShowTarget", (int *)&config.ShowTarget);
	xml_GetSetting(xConfig, "ShowMiniMap", (int *)&config.ShowMiniMap);
	xml_GetSetting(xConfig, "PlayerShipID", &config.PlayerShipID);
	xml_GetSetting(xConfig, "ServerAddress", config.ServerAddress);
	xml_GetSetting(xConfig, "Team", &config.Team);

	console_Print("CONFIG: Configuration loaded sucessfully", configFileName);
}
