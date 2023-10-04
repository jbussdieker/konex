/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: console.cpp,v 1.33 2005/08/16 22:55:28 ecco Exp $             *
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
#define KC_NONE 		0
#define KC_ANIMATING 	1
#define KC_SHOWN 		2

#define CONSOLE_SPEED   500.0f
#define CONSOLE_SIZE	270.0f
#define CONSOLE_ALPHA	160
#define CONSOLE_LINES   255
#define CONSOLE_LINEWIDTH 128
#define CONSOLE_DISPLAYLINES 16
#define CONSOLE_NOTIFYSIZE 16

struct console_notify_t
{
	char line[CONSOLE_LINEWIDTH];
	float ttl;	
};

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Variables
/////////////////////////////////////////////////////////////////////////////
static unsigned int consoleState = KC_NONE;
static float consoleBottom = 0.0f;
static char consoleBuffer[CONSOLE_LINES][CONSOLE_LINEWIDTH];
static char consoleCmd[CONSOLE_LINEWIDTH]; // = "bmp2tga callisto.bmp\0";
static char consoleLastCmd[CONSOLE_LINEWIDTH];
static unsigned int consolePosition = 0;
static unsigned int consoleLines = 0;
static float consoleAnimationSpeed = 0.0f;
static console_notify_t consoleNotify[CONSOLE_NOTIFYSIZE];

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////
static void executeCommand()
{
	if (strlen(consoleCmd) < 1)
		return;

	if (strcmp(consoleCmd, "quit") == 0 || strcmp(consoleCmd, "exit") == 0 || strcmp(consoleCmd, "end") == 0 || strcmp(consoleCmd, "bye") == 0 || strcmp(consoleCmd, "close") == 0 || strcmp(consoleCmd, "q") == 0)
	{
		konex_Shutdown();
	}
	else if (strncmp(consoleCmd, "aicount ", 8) == 0)
	{
		char buffer[128];
		strcpy(buffer, consoleCmd+8);

		unsigned int result;
		sscanf(buffer, "%u", &result);

		config.AICount = result;

		console_Print("- AI Count Set To (%d)", result);
	}
	else if (strcmp(consoleCmd, "stat") == 0)
	{
		console_Print("- Health: %d Kills: %d", gameMainPlayer->health, gameMainPlayer->frags);
	}
	else if (strcmp(consoleCmd, "god") == 0)
	{
		if (config.GodMode == true)
		{
			config.GodMode = false;
			console_Print("- God Mode OFF");
		}
		else
		{
			config.GodMode = true;
			console_Print("- God Mode ON");
		}
	}
	else if (strncmp(consoleCmd, "bmp2tga ", 8) == 0)
	{
		char imagefile[128];
		strcpy(imagefile, consoleCmd + 8);

		console_Print("- Loading image (%s)...", imagefile);

		vid_Bmp2tga(imagefile);
	}
	else if (strncmp(consoleCmd, "chat ", 5) == 0)
	{
		char imagefile[128];
		strcpy(imagefile, consoleCmd + 5);

		//net_SendChat(imagefile, strlen(imagefile));
		console_Print(imagefile);
	}
	else if (strcmp(consoleCmd, "uptime") == 0)
	{
		console_Print("- Running for %0.2f second(s)", timer_FloatTime());
	}
	else if (strcmp(consoleCmd, "spawn") == 0)
	{
		player_CreateAI();
	}
	else if (strcmp(consoleCmd, "fps") == 0)	
	{
		interface_ToggleFPS();
	}
	else if (strcmp(consoleCmd, "hud") == 0)	
	{
		interface_ToggleHUD();
	}
	else if (strcmp(consoleCmd, "viewsprite") == 0)	
	{
		interface_ToggleViewSprite();
	}
	else if (strcmp(consoleCmd, "minimap") == 0)	
	{
		interface_ToggleMiniMap();
	}
	else if (strcmp(consoleCmd, "target") == 0)	
	{
		interface_ToggleTarget();
	}
	else if (strcmp(consoleCmd, "showfps") == 0)
	{
		timer_ShowFPS();
	}
	else if (strcmp(consoleCmd, "savegame") == 0)
	{
		game_Save();
	}
	else if (strcmp(consoleCmd, "loadgame") == 0)
	{
		game_Load();
	}
	else if (strcmp(consoleCmd, "listentities") == 0)
	{
		entities_DebugList();
	}
	else if (strncmp(consoleCmd, "viewentity ", 11) == 0)
	{
		char buffer[128];
		strcpy(buffer, consoleCmd+11);

		unsigned int result;
		sscanf(buffer, "%u", &result);

		viewPlayer = entities_GetEntity(result);

		console_Print("- Viewing entity (%d)", result);		
	}
	else if (strncmp(consoleCmd, "viewsprite ", 11) == 0)
	{
		char buffer[128];
		strcpy(buffer, consoleCmd+11);

		unsigned int result;
		sscanf(buffer, "%u", &result);

//		interfaceViewSpriteID = result;

		console_Print("- Viewing sprite (%d)", result);		
	}
	else if (strncmp(consoleCmd, "starcount ", 10) == 0)
	{
		char buffer[128];
		strcpy(buffer, consoleCmd+10);

		unsigned int result;
		sscanf(buffer, "%u", &result);

		stars_Init(result);
		config.StarCount = result;

		console_Print("- Star count set to (%d)", result);		
	}
	else if (strncmp(consoleCmd, "team ", 5) == 0)
	{
		char buffer[128];
		strcpy(buffer, consoleCmd+5);

		unsigned int result;
		sscanf(buffer, "%u", &result);

		player_ChangeTeams(gameMainPlayer, result);
		config.Team = result;
		
		console_Print("- Player team set to (%d)", result);		
	}
	else if (strncmp(consoleCmd, "playership ", 11) == 0)
	{
		char buffer[128];
		strcpy(buffer, consoleCmd+11);

		unsigned int result;
		sscanf(buffer, "%u", &result);

		gameMainPlayer->shipid = result;
		config.PlayerShipID = result;

		console_Print("- Player ship set to ID (%d)", result);		
	}
	else if (strcmp(consoleCmd, "startserver") == 0)
	{
		//net_StartServer();
	}
	else if (strncmp(consoleCmd, "connect ", 8) == 0)
	{
		char server[128];
		strcpy(server, consoleCmd+8);
		//net_Connect(server);
	}
	else if (strncmp(consoleCmd, "loadship ", 9) == 0)
	{
		char filename[128];
		strcpy(filename, consoleCmd+9);

		unsigned int result;
		result = ships_LoadShip(filename);		

		console_Print("- Ship (%s) loaded succesfully to ID (%d)", filename, result);
	}
	else if (strncmp(consoleCmd, "saveship ", 9) == 0)
	{
		char buffer[128];
		strcpy(buffer, consoleCmd+9);

		unsigned int result;
		sscanf(buffer, "%u", &result);

		ships_SaveShip(result);
	}
	else if (strcmp(consoleCmd, "saveships") == 0)
	{
		ships_SaveShips();
	}
	else if (strncmp(consoleCmd, "shipturnspeed ", 14) == 0)
	{
		char buffer[128];
		strcpy(buffer, consoleCmd+14);

		unsigned int shipid;
		unsigned int turnspeed;

		sscanf(buffer, "%u %u", &shipid, &turnspeed);

		ships[shipid].turnspeed = turnspeed;
	}
	else if (strncmp(consoleCmd, "shipmaxvelocity ", 16) == 0)
	{
		char buffer[128];
		strcpy(buffer, consoleCmd+16);

		unsigned int shipid;
		unsigned int maxvelocity;

		sscanf(buffer, "%u %u", &shipid, &maxvelocity);

		ships[shipid].maxvelocity = maxvelocity;
	}
	else if (strncmp(consoleCmd, "shipacceleration ", 17) == 0)
	{
		char buffer[128];
		strcpy(buffer, consoleCmd+17);

		unsigned int shipid;
		unsigned int acceleration;

		sscanf(buffer, "%u %u", &shipid, &acceleration);

		ships[shipid].acceleration = acceleration;
	}
	else if (strcmp(consoleCmd, "loc") == 0 || strcmp(consoleCmd, "nav") == 0 || strcmp(consoleCmd, "where") == 0 || strcmp(consoleCmd, "location") == 0 || strcmp(consoleCmd, "coord") == 0 || strcmp(consoleCmd, "coordinates") == 0)
	{
		console_Print("- Location: %0.2f, %0.2f Heading: %0.2f degrees", gameMainPlayer->pos.x, gameMainPlayer->pos.y, gameMainPlayer->heading);
		console_Print("- Velocity: %0.2f, %0.2f", gameMainPlayer->velocity.x, gameMainPlayer->velocity.y);
	}
	else if (strcmp(consoleCmd, "listships") == 0 || strcmp(consoleCmd, "shiplist") == 0)
	{
		ships_ListShips();
	}
	else if (strcmp(consoleCmd, "listplanets") == 0 || strcmp(consoleCmd, "planetlist") == 0)
	{
		planets_ListPlanets();
	}
	else
	{
		console_Print("Unknown Command (%s)", consoleCmd);
	}

	strcpy(consoleLastCmd, consoleCmd);
	consoleCmd[0] = '\0';
}

static void drawBuffer()
{
	unsigned int i;
	int offset = (int)consoleBottom - 40;
	
	for (i = consolePosition; i < consolePosition + CONSOLE_DISPLAYLINES; i++)
	{
		if (i > CONSOLE_LINES - 1)
			break;

		vid_DrawString(10, offset - (i - consolePosition) * 15, 255, consoleBuffer[i]);
	}

	vid_DrawString(10, offset + 15, 255, consoleCmd);
}

static void drawNotify()
{
	int i;
	int count = 0;

	// Draw all notifications
	for (i = 0; i < CONSOLE_NOTIFYSIZE; i++)
	{
		if (consoleNotify[i].ttl > 0.0f)
		{
			vid_DrawString(32, 32 + (count * 20), 255, consoleNotify[i].line);
			consoleNotify[i].ttl -= tm_FrameTime;
			count += 1;
		}
	}
}

static void drawConsole()
{
	switch (consoleState)
	{
		case KC_NONE:
		{
			if (sysKeys[IN_TILDA] == true)
			{
				sysKeys[IN_TILDA] = false;
				consoleAnimationSpeed = CONSOLE_SPEED;
				consoleState = KC_ANIMATING;
			}
			break;
		}
		case KC_ANIMATING:
		{
			if (consoleBottom >= CONSOLE_SIZE && consoleAnimationSpeed > 0.0f)
			{
				consoleBottom = CONSOLE_SIZE;		
				consoleState = KC_SHOWN;
			}
			if (consoleBottom <= 0.0f && consoleAnimationSpeed < 0.0f)
			{
				consoleBottom = 0.0f;
				consoleState = KC_NONE;
			}
			else
				consoleBottom += tm_FrameTime * consoleAnimationSpeed;
			
			vid_Rect(0, 0, vidWidth, (int)consoleBottom, vid_Color(128, 128, 128), CONSOLE_ALPHA);
			drawBuffer();
			break;
		}
		case KC_SHOWN:
		{
			if (sysKeys[IN_TILDA] == true)
			{
				sysKeys[IN_TILDA] = false;
				consoleState = KC_ANIMATING;
				consoleAnimationSpeed = -CONSOLE_SPEED;
			}

			vid_Rect(0, 0, vidWidth, (int)consoleBottom, vid_Color(128,128,128),CONSOLE_ALPHA);
			drawBuffer();
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void console_Print(char *message, ...)
{
	static char textBuffer[CONSOLE_LINEWIDTH];
	//static char previousBuffer[1024];

	va_list args;
	va_start(args, message);
	vsprintf(textBuffer, message, args);
	va_end(args);

	int i;
	for (i = CONSOLE_LINES - 1; i > 0; i--)
	{
		sprintf(consoleBuffer[i], "%s", consoleBuffer[i-1]);
	}
	sprintf(consoleBuffer[0], "%s", textBuffer);
	printf("%s\n", textBuffer);
	fflush(NULL);

	if (consoleLines < CONSOLE_LINES)
		consoleLines++;

	if (consolePosition < CONSOLE_LINES - CONSOLE_DISPLAYLINES && consolePosition > 0)
		consolePosition++;
}

void console_Notify(char *message, ...)
{
	static char textBuffer[CONSOLE_LINEWIDTH];
	int i;
	//static char previousBuffer[1024];

	va_list args;
	va_start(args, message);
	vsprintf(textBuffer, message, args);
	va_end(args);

	console_Print(textBuffer);

	// Move all notifications forward in the array
	for (i = 0; i < CONSOLE_NOTIFYSIZE - 1; i++)
	{
		strcpy(consoleNotify[i].line, consoleNotify[i+1].line);
		consoleNotify[i].ttl = consoleNotify[i+1].ttl;
	}
	
	// Create the new item at the beginning
	strcpy(consoleNotify[CONSOLE_NOTIFYSIZE - 1].line, textBuffer);
	consoleNotify[CONSOLE_NOTIFYSIZE - 1].ttl = 5.0f;
}

void console_Input()
{
	if (consoleState != KC_SHOWN)
		return;

	char cmdbk[CONSOLE_LINEWIDTH];

	// Go back in console history
	if (sysKeys[IN_PAGEUP] == true)
	{
		sysKeys[IN_PAGEUP] = false;

		if (consolePosition < consoleLines - CONSOLE_DISPLAYLINES)
			consolePosition += 1;
	}
	// Go forward in console history
	if (sysKeys[IN_PAGEDOWN] == true)
	{	
		sysKeys[IN_PAGEDOWN] = false;

		if (consolePosition > 0)
			consolePosition -= 1;
	}

	if (sysKeys[IN_UPARROW] == true)
	{
		strcpy(consoleCmd, consoleLastCmd);
		sysKeys[IN_UPARROW] = false;
	}

	if (sysKeys[IN_BACKSPACE] == true)
	{
		sysKeys[IN_BACKSPACE] = false;

		if (strlen(consoleCmd) > 0)
			consoleCmd[strlen(consoleCmd) - 1] = '\0';
	}

	if (sysKeys[IN_ENTER] == true)
	{
		sysKeys[IN_ENTER] = false;

		executeCommand();
	}

	if (strlen(consoleCmd) < CONSOLE_LINEWIDTH - 1)
	{
		char input;
		input = in_ASCII();

		if (input > 0)
		{
			// Process text character input
			sprintf(cmdbk, "%s", consoleCmd);
			sprintf(consoleCmd, "%s%c", cmdbk, input);
		}
	}
}

void console_DrawNotify()
{
	if (consoleState == KC_NONE)
		drawNotify();
}

void console_DrawConsole()
{
	drawConsole();
}
