/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: vid_linux.cpp,v 1.10 2005/08/14 06:56:45 ecco Exp $   *
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
static Window   vidlWindow;
static Display *sysDisplay = NULL;

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void vidl_Swap()
{
	glXSwapBuffers( sysDisplay, vidlWindow );
}

static char *getFont(char *name)
{
	char **fonts;
	int fcount;
	char search[128];

	sprintf(search, "*%s*", name);

	fonts = XListFonts(sysDisplay, search, 1, &fcount);

	return fonts[0];
}

void vidl_Main()
{
  	while(1)
    {
		XEvent event;
	
		sys_Frame();

		while( XPending(sysDisplay) )
		{
			XNextEvent( sysDisplay, &event );
	
			switch( event.type )
			{
				case UnmapNotify: 
				{
					sysActive = 0;
					break;
					//console_Print("%d", event.type);
				}
				case   MapNotify: 
				{
					sysActive = 1; 
					break;
					//console_Print("%d", event.type);
				}
				case KeyPress:
				{
					//int key = XLookupKeysym( &event.xkey, 0 );
					sysKeys[event.xkey.keycode] = true;
					//console_Print("KeyCode %d", event.xkey.keycode);
					break;
				}
				case KeyRelease:
				{
					//console_Print("KeyRelease (%d)", event.xkey.keycode);
					sysKeys[event.xkey.keycode] = false;
					break;
				}
				case ButtonPress:
				{
					sysMouseButtons[event.xbutton.button] = true;
					//console_Print("MousePress (%d)", event.xbutton.button);
					sysMouseX = event.xbutton.x;
					sysMouseY = event.xbutton.y;
					//console_Print(" x, y (%d, %d)", sysMouseX, sysMouseY);
					break;
				}
				case ButtonRelease:
				{
					sysMouseButtons[event.xbutton.button] = false;
					break;
				}
				case MotionNotify:
				{
					sysMouseX = event.xbutton.x;
					sysMouseY = event.xbutton.y;
					break;
				}
				case ConfigureNotify:
				{
					//console_Print("%d", event.type);
					vid_Resize(event.xconfigure.width, event.xconfigure.height);
					break;
				}
				case DestroyNotify:
				{
					//console_Print("Destroy Notify");
					sys_Shutdown();
					break;
				}
				default:
					//console_Print("%d", event.type);
					break;
					
			}
		}
    }
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
    XWindowAttributes windowAttributes;

	XResizeWindow(sysDisplay, vidlWindow, width, height);

	XGetWindowAttributes(sysDisplay, vidlWindow, &windowAttributes);
	windowAttributes.override_redirect = fullscreen;
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
