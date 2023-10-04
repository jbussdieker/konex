/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: vid_sdl.cpp,v 1.1 2005/07/25 18:37:16 ecco Exp $   *
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
#include <SDL/SDL.h>

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Definitions, Enumeration and Typedefs
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Variables
/////////////////////////////////////////////////////////////////////////////
SDL_Surface *surface;
int videoFlags;

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void vidsdl_Init(int width, int height, bool fullscreen)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		console_Print("VID_SDL: Video initialization failed: (%s)", SDL_GetError());
	}

    const SDL_VideoInfo *videoInfo;

    videoInfo = SDL_GetVideoInfo();
	
	if (!videoInfo)
 	{
     	console_Print("VID_SDL: Video query failed: (%s)", SDL_GetError());
	}

    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL          */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering       */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
    videoFlags |= SDL_RESIZABLE;       /* Enable window resizing        */

	if (fullscreen == true)
		videoFlags |= SDL_FULLSCREEN;

	if (videoInfo->hw_available)
 		videoFlags |= SDL_HWSURFACE;
    else
 		videoFlags |= SDL_SWSURFACE;

	if ( videoInfo->blit_hw )
 		videoFlags |= SDL_HWACCEL;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    surface = SDL_SetVideoMode(width, height, 32, videoFlags);

    if (!surface)
 	{
    	console_Print("VID_SDL: Video mode set failed: %s", SDL_GetError());
 	}
}

void vidsdl_Main()
{
    SDL_Event event;
	bool done = false;
	bool isActive;
	while ( !done )
	{
		while (SDL_PollEvent(&event ))
  		{
      		switch(event.type)
   			{
   				case SDL_ACTIVEEVENT:
					if (event.active.gain == 0)
						isActive = FALSE;
					else
						isActive = TRUE;
					break;
				case SDL_VIDEORESIZE:
			       surface = SDL_SetVideoMode( event.resize.w, event.resize.h, 16, videoFlags );
       				if (!surface)
    				{
        				console_Print("VID_SDL: Could not get a surface after resize: %s", SDL_GetError());
    				}
       				vid_Resize(event.resize.w, event.resize.h);
       				break;
   				case SDL_KEYDOWN:
					sysKeys[event.key.keysym.scancode] = true;
       				//handleKeyPress( &event.key.keysym );
       				break;
   				case SDL_KEYUP:
					sysKeys[event.key.keysym.scancode] = false;
					break;
				case SDL_MOUSEMOTION:
					sysMouseX = event.motion.x;
					sysMouseY = event.motion.y;
					break;
				case SDL_MOUSEBUTTONDOWN:
					sysMouseButtons[event.button.button] = true;
					break;
				case SDL_MOUSEBUTTONUP:
					sysMouseButtons[event.button.button] = false;
					break;
   				case SDL_QUIT:
       				done = TRUE;
       				break;
   				default:
       				break;
			}
  		}
		sys_Frame();
	}
}

void vidsdl_Swap()
{
    SDL_GL_SwapBuffers();
}
/*
char *getFont(char *name)
{
	char **fonts;
	int fcount;
	char search[128];

	sprintf(search, "*%s*", name);

	fonts = XListFonts(sysDisplay, search, 1, &fcount);

	return fonts[0];
}

int vidl_CreateFont()
{
	int font;
	Cursor null_cursor;
    XFontStruct *fixed;
    XColor black =
    {
        0, 0, 0, 0, 0, 0
    };
   
	font = glGenLists( 256 );

	//    fixed = XLoadQueryFont(
  	//      sysDisplay, "-misc-fixed-medium-r-*-*-20-*-*-*-*-*-*-*" );
    //fixed = XLoadQueryFont(
        //sysDisplay, "-Adobe-Times-Medium-R-Normal--14-100-100-100-P-74-ISO8859-1" );
    fixed = XLoadQueryFont(sysDisplay, getFont("misc?fixed?medium?r*30"));


    null_cursor = XCreateGlyphCursor(
        sysDisplay, fixed->fid, fixed->fid, ' ', ' ', &black, &black );

	//max_bounds.rbearing - min_bounds.lbearing
    glXUseXFont( fixed->fid, 0, 256, font );

    XFreeFont( sysDisplay, fixed );

	return font;
}

void vidl_SetMode(int width, int height, bool fullscreen)
{
	XResizeWindow(sysDisplay, vidlWindow, width, height);
}

void vidl_Init(int width, int height, bool fullscreen)
{
    XSetWindowAttributes windowAttributes;
    XVisualInfo *visualInfo = NULL;
    Colormap colorMap;
    GLXContext glxContext;
    int errorBase;
	int eventBase;

    // Open a connection to the X server
    sysDisplay = XOpenDisplay( NULL );

    if( sysDisplay == NULL )
    {
        console_Print("glxsimple: %s\n", "could not open display");
        exit(1);
    }

    // Make sure OpenGL's GLX extension supported
    if( !glXQueryExtension( sysDisplay, &errorBase, &eventBase ) )
    {
        console_Print("glxsimple: %s\n", "X server has no OpenGL GLX extension");
        exit(1);
    }

    // Find an appropriate visual

    int doubleBufferVisual[]  =
    {
		GLX_RGBA,
		GLX_DOUBLEBUFFER,
		GLX_RED_SIZE,       8, 
		GLX_GREEN_SIZE,     8, 
		GLX_BLUE_SIZE,      8, 
		GLX_DEPTH_SIZE,    24,
		None    
	};

    // Try for the double-bufferd visual first
    visualInfo = glXChooseVisual( sysDisplay, DefaultScreen(sysDisplay), doubleBufferVisual );

    // Create an OpenGL rendering context
    glxContext = glXCreateContext( sysDisplay, 
                                   visualInfo, 
                                   NULL,      // No sharing of display lists
                                   GL_TRUE ); // Direct rendering if possible
                           
    if( glxContext == NULL )
    {
        console_Print("glxsimple: %s\n", "could not create rendering context");
        exit(1);
    }

    // Create an X colormap since we're probably not using the default visual 
    colorMap = XCreateColormap( sysDisplay, 
                                RootWindow(sysDisplay, visualInfo->screen), 
                                visualInfo->visual, 
                                AllocNone );

    windowAttributes.colormap     = colorMap;
    windowAttributes.border_pixel = 0;
	windowAttributes.override_redirect = fullscreen;
    windowAttributes.event_mask   = ExposureMask           |
                                    VisibilityChangeMask   |
                                    KeyPressMask           |
                                    KeyReleaseMask         |
                                    ButtonPressMask        |
                                    ButtonReleaseMask      |
                                    PointerMotionMask      |
                                    StructureNotifyMask    |
									SubstructureRedirectMask |
                                    SubstructureNotifyMask |
                                    FocusChangeMask;
    
    // Create an X window with the selected visual
    vidlWindow = XCreateWindow( sysDisplay, 
                              RootWindow(sysDisplay, visualInfo->screen), 
                              0, 0,     // x/y position of top-left outside corner of the window
                              width, height, // Width and height of window
                              0,        // Border width
                              visualInfo->depth,
                              InputOutput,
                              visualInfo->visual,
                              CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
                              &windowAttributes );

    XSetStandardProperties( sysDisplay,
                            vidlWindow,
                            SYS_APPNAME,
                            SYS_APPNAME,
                            None,
                            NULL, // Test
                            0,  // Test
                            NULL );

    // Bind the rendering context to the window
    glXMakeCurrent( sysDisplay, vidlWindow, glxContext );

    // Request the X window to be displayed on the screen
    XMapWindow( sysDisplay, vidlWindow );

	if (fullscreen == true)
	{
		XGrabKeyboard(  sysDisplay, vidlWindow, True, GrabModeAsync, GrabModeAsync, CurrentTime );
	}
}
*/
