/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: menu.cpp,v 1.22 2005/08/14 08:12:27 ecco Exp $                 *
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

// XML Description
//<menuitem>
//		<caption></>
//		<action></>
//</menuitem>

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Includes
/////////////////////////////////////////////////////////////////////////////
#include "defs.h"

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Definitions, Enumeration and Typedefs
/////////////////////////////////////////////////////////////////////////////
#define MAX_MENUITEMS 10

struct menu_menuitem_t
{
	unsigned int imageid;
	char caption[128];
	char *value;
	void (*action)();
	bool used;
	bool mousedown;
	bool input;
	bool focus;
};

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Variables
/////////////////////////////////////////////////////////////////////////////
menu_menuitem_t menu[MAX_MENUITEMS];

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Function Prototypes
/////////////////////////////////////////////////////////////////////////////
static void loadMainMenu();
static void loadOptionsMenu();
static void loadSinglePlayerGameMenu();
static void loadMultiplayerGameMenu();
static void loadLoadGameMenu();
static void loadGameMenu();

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////
static unsigned char getFadeAlpha(bool reset)
{
	const float fadetime = 0.2f;

	static bool	konexFadeIn = true;
	static float konexFadeInStart = timer_FloatTime();
	unsigned char backalpha;

	if (reset == true)
	{
		konexFadeIn = true;
		konexFadeInStart = timer_FloatTime();
	}

	if (konexFadeIn == true)
	{
		if (timer_FloatTime() - konexFadeInStart > fadetime)
		{
			konexFadeIn = false;
			backalpha = 255;
		}
		else
			backalpha = (unsigned char)((timer_FloatTime() - konexFadeInStart) * (255.0f / fadetime));
	}
	else
		backalpha = 255;

	return backalpha;
}

static void loadMainMenu()
{
	if (gameRunning == true)
	{
		loadGameMenu();
		return;
	}

	menu_Clear();
	menu_AddItem("Single Player", loadSinglePlayerGameMenu);
	//menu_AddItem("Multiplayer", loadMultiplayerGameMenu);
	menu_AddItem("Options", loadOptionsMenu);
	menu_AddItem("Load Game", game_Load);
	menu_AddItem("Quit", konex_Shutdown);
}

static void loadSinglePlayerGameMenu()
{
	menu_Clear();
	menu_AddItem("Team Deathmatch", game_CreateTeamDeathmatch);
	menu_AddItem("Sunday Drive", game_CreateSundayDrive);
	menu_AddItem("Return to Main Menu", loadMainMenu);
}

static void loadMultiplayerJoinGameMenu()
{
	menu_Clear();
	menu_AddItem("Server Address", game_JoinServer);
	menu[0].input = true;
	menu[0].value = config.ServerAddress;
	menu_AddItem("Join Game", game_JoinServer);
	menu_AddItem("Return to Main Menu", loadMainMenu);
}

static void loadMultiplayerGameMenu()
{
	menu_Clear();
	menu_AddItem("Host Game", game_CreateServer);
	menu_AddItem("Join Game", loadMultiplayerJoinGameMenu);
	menu_AddItem("Return to Main Menu", loadMainMenu);
}

static void loadGameMenu()
{
	menu_Clear();
	menu_AddItem("End Current Game", game_End);
	menu_AddItem("Options", loadOptionsMenu);
	menu_AddItem("Load Game", game_Load);
	menu_AddItem("Save Game", game_Save);
	menu_AddItem("Select Ship", interface_ToggleSelectShip);
	menu_AddItem("Resume Game", interface_ToggleMenu);
	menu_AddItem("Quit", konex_Shutdown);
}

static void loadOptionsDisplayMenu()
{
	menu_Clear();
	menu_AddItem("Toggle FPS", interface_ToggleFPS);

	if (gameRunning == true)
	{
		menu_AddItem("Toggle Mini Map", interface_ToggleMiniMap);
		menu_AddItem("Toggle HUD", interface_ToggleHUD);
		menu_AddItem("Toggle Target", interface_ToggleTarget);
	}

	menu_AddItem("Return to Options", loadOptionsMenu);
}

static void loadPlayerMenu()
{
	menu_Clear();
	menu_AddItem("Player Name", loadOptionsDisplayMenu);
	menu[0].input = true;
	menu[0].value = config.PlayerName;
	menu_AddItem("Return to Options", loadOptionsMenu);
}

static void loadOptionsVideoMenu()
{
	menu_Clear();
	menu_AddItem("640x480", menu_Resolution1);
	menu_AddItem("800x600", menu_Resolution2);
	menu_AddItem("1024x768", menu_Resolution3);
	menu_AddItem("1600x1200", menu_Resolution4);
	menu_AddItem("Toggle Fullscreen", menu_ToggleFullscreen);
	menu_AddItem("Return to Options", loadOptionsMenu);
}

static void loadOptionsMenu()
{
	menu_Clear();
	menu_AddItem("Display", loadOptionsDisplayMenu);
	//menu_AddItem("Video", loadOptionsVideoMenu);
	menu_AddItem("Player", loadPlayerMenu);
	menu_AddItem("Return to Main Menu", loadMainMenu);
}

static dword getFreeMenuItem()
{
	dword i;

	for (i = 0; i < MAX_MENUITEMS; i++)
		if (menu[i].used == false)
			return i;
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void menu_Resolution1()
{
	vid_SetMode(640,480,vidFullscreen);
}

void menu_Resolution2()
{
	vid_SetMode(800,600,vidFullscreen);
}

void menu_Resolution3()
{
	vid_SetMode(1024,768,vidFullscreen);
}

void menu_Resolution4()
{
	vid_SetMode(1600,1200,vidFullscreen);
}

void menu_ToggleFullscreen()
{
	vid_SetMode(vidWidth, vidHeight, !vidFullscreen);
}

void menu_Init(bool ingame)
{
	if (ingame == true)
		loadGameMenu();
	else
		loadMainMenu();
}

void menu_Clear()
{
	// Clear menu item array
	memset(&menu, 0, sizeof(menu));

	// Reset fade in
	getFadeAlpha(true);
}

void menu_AddItem(char *caption, void (*action)())
{
	dword tmpid;
	tmpid = getFreeMenuItem();

	strcpy(menu[tmpid].caption, caption);
	//menu[tmpid].value = NULL;
	menu[tmpid].used = true;
	menu[tmpid].action = action;
}

void menu_Input(window *win)
{
	#define MENU_HEIGHT 40
	#define MENU_PADDING 10

	int i;
	unsigned char alpha = getFadeAlpha(false);

	for (i = 0; i < MAX_MENUITEMS; i++)
	{
		if (menu[i].used == true && menu[i].focus == true && menu[i].input == true)
		{
			char bak[128];
			unsigned char input = in_ASCII();

			//console_Print("%d", input);

			if (input == IN_BACKSPACE)
			{
				int s = strlen(menu[i].value);

				if (s > 0)
					menu[i].value[s - 1] = '\0';
			}
			else if (input == IN_ENTER)
			{
				menu[i].focus = false;
			}
			else
			{
				strcpy(bak, menu[i].value);
				sprintf(menu[i].value, "%s%c", bak, input);
			}
		}
	}
}

void menu_Draw(window *win)
{
	#define MENU_HEIGHT 40
	#define MENU_PADDING 10

	int i;
	vid_color_t buttoncolor;
	unsigned char alpha = getFadeAlpha(false);

	for (i = 0; i < MAX_MENUITEMS; i++)
	{
		if (menu[i].used == true)
		{
			int drawx, drawy;

			drawx = win->x + MENU_PADDING;
			drawy = (i * (MENU_HEIGHT + MENU_PADDING)) + win->y + MENU_PADDING;
		
			if (sysMouseX > drawx && sysMouseX < drawx + win->width - MENU_PADDING * 2 &&
				sysMouseY > drawy && sysMouseY < drawy + MENU_HEIGHT)
			{
				buttoncolor = vid_Color(64, 64, 64);
				if (sysMouseButtons[1] == true)
				{
					menu[i].mousedown = true;
				}
				else if (menu[i].mousedown == true)
				{
					menu[i].mousedown = false;
					sysMouseButtons[1] = false;

					if (menu[i].input == false)
						menu[i].action();
					else
						menu[i].focus = true;
				}
			}
			else
			{
				menu[i].mousedown = false;
				buttoncolor = vid_Color(48, 48, 48);	
			}

			if (menu[i].focus == true)
				buttoncolor = vid_Color(96, 96, 96);	

			vid_Rect(drawx, drawy, win->width - MENU_PADDING * 2, MENU_HEIGHT, buttoncolor, alpha);

			if (menu[i].imageid != 0)
			{
				vid_DrawImage(drawx, drawy, MENU_HEIGHT, MENU_HEIGHT, alpha, menu[i].imageid, false);
			}

			vid_DrawString(drawx + 10, drawy + 10, alpha, menu[i].caption);

			// Only draw the value parameter if it's a valid pointer
			if (menu[i].value != NULL)
				vid_DrawString(drawx + win->width / 2, drawy + 10, alpha, menu[i].value);


			win->sheight = drawy - win->sy + MENU_HEIGHT + MENU_PADDING + 7;
		}
	}

	menu_Input(win);
}
