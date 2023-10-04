/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: interface.h,v 1.12 2005/08/16 22:55:28 ecco Exp $   *
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
struct window
{
	// Object properties
	char caption[128];

	int sx, sy; // Screen coords
	int swidth, sheight; 

	int x, y; // Drawing coords
	int width, height;

	bool visible;
	void (*draw2)(window*);
	void (*close)();
	
	// Event flags
	bool moving;
	int moveoffsetx, moveoffsety;

	// Input flags
	bool lmouse;
	int mousex, mousey;

	bool used;
};

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////
//extern dword interfaceViewSpriteID;

/////////////////////////////////////////////////////////////////////////////
// Interface Function
/////////////////////////////////////////////////////////////////////////////
void 			interface_Init();
void 			interface_Frame();
void 			interface_Set800x600();
void 			interface_Set1024x768();
void 			interface_Set1600x1200();
void 			interface_ToggleFullscreen();
void 			interface_ToggleFPS();
void 			interface_ToggleHUD();
void 			interface_ToggleTarget();
void 			interface_ToggleViewSprite();
void 			interface_ToggleMiniMap();
void 			interface_ToggleMap();
void 			interface_ToggleMenu();
void 			interface_HideMenu();
void 			interface_ToggleSelectShip();
void			interface_SetGameStatus(bool ingame);
void 			interface_SetPlanetStatus(bool landed);
item_c<window>* interface_CreateWindow(char *caption, int x, int y, int width, int height);
void 			interface_Input();
