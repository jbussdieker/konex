/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: snd_main.h,v 1.2 2005/07/24 12:27:41 ecco Exp $   *
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
#define SOUND_COUNT 256
/*#define NUM_BUFFERS 8
#define NUM_SOURCES 8*/

struct snd_sound_t
{
	vgvec2 pos;
	vgvec2 velocity;
	bool used;

/*	ALenum		format;
	ALsizei		size;
	ALsizei		freq;
	ALboolean	loop;
	ALvoid*		data;
	ALuint 		buffers[NUM_BUFFERS];
	ALuint 		source[NUM_SOURCES];*/
};

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////
extern snd_sound_t sounds[SOUND_COUNT];

/////////////////////////////////////////////////////////////////////////////
// Interface Function
/////////////////////////////////////////////////////////////////////////////
void 			snd_Init(void);
void 			snd_Frame(void);
unsigned int 	snd_LoadSound(char *filename);
void 			snd_Play(unsigned int id);
void 			snd_UnloadSound(unsigned int id);
void			snd_Shutdown(void);
