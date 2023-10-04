/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: snd_main.cpp,v 1.3 2005/08/14 06:56:45 ecco Exp $           *
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
static snd_sound_t sounds[SOUND_COUNT];

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////
static unsigned int getFreeSound()
{
	int i;
	for (i = 1; i < SOUND_COUNT; i++)
		if (sounds[i].used == false)
			return i;

	return 0;
}
/*
void createSource(snd_sound_t *sound)
{
	unsigned int error;

	// Generate the sources
	alGenSources(NUM_SOURCES, sound->source);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		console_Print("alGenSources : %d", error);
		return;
	}

	alSourcei(sound->source[0], AL_BUFFER, sound->buffers[0]);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		console_Print("alSourcei : %d", error);
		return;
	}

	//ALvoid alSourcefv(ALuint source,ALenum pname,ALfloat *values);

	ALfloat spos[3] = {1.0f, 1.0f, 1.0f};
	ALfloat svel[3] = {0.0f, 0.0f, 0.0f};
	ALfloat sori[3] = {1.0f, 1.0f, 1.0f};
	//ALfloat listenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };

	alSourcefv (sound->source[0], AL_POSITION, spos);
	alSourcefv (sound->source[0], AL_VELOCITY, svel);
	alSourcefv (sound->source[0], AL_DIRECTION, sori);
}

void createListeners()
{
	//ALfloat lpos[3] = {0.0f, 0.0f, 0.0f};
	//ALfloat lvel[3] = {0.0f, 0.0f, 0.0f};
	//ALfloat lori[3] = {0.0f, 0.0f, 0.0f};

	//alListenerfv(AL_POSITION,lpos);
	//alListenerfv(AL_VELOCITY,lvel);
	//alListenerfv(AL_ORIENTATION,lori);

	ALfloat listenerPos[] = { 1.0, 0.0, 1.0 };
	ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
	ALfloat listenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };
	
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
}*/

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void snd_Init()
{
	memset(&sounds, 0, sizeof(sounds));

/*	// Init openAL
	alutInit(0, NULL);
	// Clear Error Code (so we can catch any new errors)
	alGetError();

	// Debugging
	console_Print("SND: Sound system is operational.");

	console_Print(" - Vendor (%s)", alGetString(AL_VENDOR));
	console_Print(" - Version (%s)", alGetString(AL_VERSION));
	console_Print(" - Renderer (%s)", alGetString(AL_RENDERER));*/
	console_Print("SND: Dummy sound system is operational.");
}

void snd_Play(unsigned int id)
{
/*	//tell the sound to loop continuously
	alSourcei((ALuint)sounds[id].source,AL_LOOPING,AL_TRUE);
	
	//play the sound
	alSourcePlay((ALuint)sounds[id].source);

	console_Print("SND: Playing %d.", id);*/
}

unsigned int snd_LoadSound(char *filename)
{
/*	unsigned int tmpid;
	unsigned int error;
	tmpid = getFreeSound();
	char tmpfilename[1024];
	
	sprintf(tmpfilename, "%sdata/%s", sysAppPath, filename);

	// Create the buffers
	alGenBuffers(NUM_BUFFERS, sounds[tmpid].buffers);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		console_Print("alGenBuffers : %d", error);
		return 0;
	}

	alutLoadWAVFile((ALbyte *)tmpfilename, &sounds[tmpid].format, &sounds[tmpid].data, &sounds[tmpid].size, &sounds[tmpid].freq, &sounds[tmpid].loop);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		console_Print("alutLoadWAVFile exciting_sound.wav : %d", error);
		// Delete Buffers
		alDeleteBuffers(NUM_BUFFERS, sounds[tmpid].buffers);
		return 0;
	}

	alBufferData(sounds[tmpid].buffers[0], sounds[tmpid].format, sounds[tmpid].data, sounds[tmpid].size, sounds[tmpid].freq);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		console_Print("alBufferData buffer 0 : %d", error);
		// Delete buffers
		alDeleteBuffers(NUM_BUFFERS, sounds[tmpid].buffers);
		return 0;
	}

	createSource(&sounds[tmpid]);
	createListeners();

	console_Print("SND: Sound loaded into ID (%d).", tmpid);

	return tmpid;*/
	return 0;
}

void snd_Frame()
{

}

void snd_UnloadSound(unsigned int id)
{
/*	unsigned int error;

	alutUnloadWAV(sounds[id].format,sounds[id].data,sounds[id].size,sounds[id].freq);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		console_Print("alutUnloadWAV : %d", error);
		// Delete buffers
		alDeleteBuffers(NUM_BUFFERS, sounds[id].buffers);
		return;
	}*/

	sounds[id].used = false;
}

void snd_Shutdown()
{
	/*unsigned int i;

	for (i = 0; i < SOUND_COUNT; i++)
	{
		if (sounds[i].used == true)
		{
			alDeleteSources(NUM_SOURCES, sounds[i].source);
			alDeleteBuffers(NUM_BUFFERS, sounds[i].buffers);
			sounds[i].used = false;
		}
	}

	alutExit();*/
}
