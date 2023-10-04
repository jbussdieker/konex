/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker   *
 *   jbussdieker@gmail.com   *
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
#include "defs.h"
bool gameRunning = false;
bool gamePaused = false;
bool gameNetwork = false;
dword gameRedTeam;
dword gameGreenTeam;
dword gameBlueTeam;
entities_entity_t *gameMainPlayer;

static void loadPlanets(TiXmlElement *xPlanets)
{
	TiXmlElement *xPlanet = 0;
	int nPlanetCount = 0;

	xPlanet = xPlanets->FirstChildElement("planet");
	while(xPlanet)
	{
		entities_entity_t *planet;

		planet = planets_SpawnPlanet(0, 0, 0);

		if (xPlanet->Attribute("xpos"))
			planet->pos.x = (float)atof(xPlanet->Attribute("xpos"));
		if (xPlanet->Attribute("ypos"))
			planet->pos.y = (float)atof(xPlanet->Attribute("ypos"));
		if (xPlanet->Attribute("sprite"))
			planet->planetid = (float)atof(xPlanet->Attribute("sprite"));
		
		//alien->color = jsiUtility::colourFromText( xAlien->Attribute("color") );
		
		xPlanet = xPlanet->NextSiblingElement("planet");
		++nPlanetCount;
	}
}

static void loadActors(TiXmlElement *xActors)
{
	TiXmlElement *xActor = 0;
	int nActorCount = 0;

	xActor = xActors->FirstChildElement("actor");
	while(xActor)
	{
		entities_entity_t *newplayer;

		newplayer = player_CreateAI();

		if (xActor->Attribute("xpos"))
			newplayer->pos.x = (float)atof(xActor->Attribute("xpos"));
		if (xActor->Attribute("ypos"))
			newplayer->pos.y = (float)atof(xActor->Attribute("ypos"));
		if (xActor->Attribute("shipid"))
			newplayer->shipid = (float)atof(xActor->Attribute("shipid"));
		if (xActor->Attribute("team"))
			newplayer->team = (float)atof(xActor->Attribute("team"));
		
		xActor = xActor->NextSiblingElement("actor");
		++nActorCount;
	}
}

static void loadSpawns(TiXmlElement *xSpawns)
{
	TiXmlElement *xSpawn = 0;
	int nSpawnCount = 0;
	entities_entity_t *newspawn;

	xSpawn = xSpawns->FirstChildElement("spawn");
	while(xSpawn)
	{
		newspawn = entities_Create(ENTITY_SPAWN);

		if (xSpawn->Attribute("xpos"))
			newspawn->pos.x = (float)atof(xSpawn->Attribute("xpos"));
		if (xSpawn->Attribute("ypos"))
			newspawn->pos.y = (float)atof(xSpawn->Attribute("ypos"));
		if (xSpawn->Attribute("team"))
			newspawn->team = (float)atof(xSpawn->Attribute("team"));
		
		//alien->color = jsiUtility::colourFromText( xAlien->Attribute("color") );
		
		xSpawn = xSpawn->NextSiblingElement("spawn");
		++nSpawnCount;
	}
}

void game_LoadMap(const char *filename)
{
	char f[MAX_PATH];

	sprintf(f, "%s%s", sysAppPath, filename);

	TiXmlDocument     *xmlDoc = new TiXmlDocument(f);
	if (!xmlDoc->LoadFile())
	{
		console_Print("GAME: Failed to load map %s", f);
		return;
	}

	TiXmlElement *xScene = 0;
	xScene = xmlDoc->FirstChildElement("scene");
	if (!xScene)
	{
		console_Print("GAME: Failed to read map from %s", f);
		xmlDoc->Clear();
		delete xmlDoc;
		return;
	}

	TiXmlElement *xMap = 0;
	xMap = xScene->FirstChildElement("map");

	if (xMap)
		map_Create((float)atof(xMap->Attribute("width")), (float)atof(xMap->Attribute("height")));
	else
		console_Print("GAME: No map settings defined in %s", f);

	TiXmlElement *xPlanets = 0;
	xPlanets = xScene->FirstChildElement("planets");

	if (xPlanets)
		loadPlanets(xPlanets);
	else
		console_Print("GAME: No planets defined in %s", f);

	TiXmlElement *xSpawns = 0;
	xSpawns = xScene->FirstChildElement("spawns");

	if (xSpawns)
		loadSpawns(xSpawns);
	else
		console_Print("GAME: No spawn points defined in %s", f);

	console_Print("GAME: Map loaded successfully %s", f);
	xmlDoc->Clear();
	delete xmlDoc;
}

void game_LoadScene(char *filename)
{
	char f[MAX_PATH];

	sprintf(f, "%s%s", sysAppPath, filename);

	TiXmlDocument     *xmlDoc = new TiXmlDocument(f);
	if (!xmlDoc->LoadFile())
	{
		console_Print("GAME: Failed to load %s", f);
		return;
	}

	TiXmlElement *xScene = 0;
	xScene = xmlDoc->FirstChildElement("scene");
	if (!xScene)
	{
		console_Print("GAME: Failed to read scene from %s", f);
		xmlDoc->Clear();
		delete xmlDoc;
		return;
	}

	TiXmlElement *xMap = 0;
	xMap = xScene->FirstChildElement("map");

	if (xMap)
		game_LoadMap(xMap->Attribute("filename"));
	else
		console_Print("GAME: No map settings defined in %s", f);

	TiXmlElement *xActors = 0;
	xActors = xScene->FirstChildElement("actors");

	if (xActors)
		loadActors(xActors);
	else
		console_Print("GAME: No actors defined in %s", f);

	console_Print("GAME: Scene loaded successfully %s", f);
	xmlDoc->Clear();
	delete xmlDoc;
}

void game_Save()
{
	char filename[128];

	sprintf(filename, "%s%s", sysAppPath, "save.dat");
	FILE *f = fopen(filename, "wb");
	//fwrite(&entities, 1, sizeof(entities), f);
	fwrite(&gameRedTeam, 1, sizeof(gameRedTeam), f);
	fwrite(&gameBlueTeam, 1, sizeof(gameBlueTeam), f);
	fwrite(&gameGreenTeam, 1, sizeof(gameGreenTeam), f);
	fwrite(&gameMainPlayer, 1, sizeof(gameMainPlayer), f);
	fclose(f);

	interface_HideMenu();

	console_Print("- Game saved...");
}

void game_Load()
{
	char filename[128];

	sprintf(filename, "%s%s", sysAppPath, "save.dat");
	FILE *f = fopen(filename, "rb");
	//fread(&entities, 1, sizeof(entities), f);
	fread(&gameRedTeam, 1, sizeof(gameRedTeam), f);
	fread(&gameBlueTeam, 1, sizeof(gameBlueTeam), f);
	fread(&gameGreenTeam, 1, sizeof(gameGreenTeam), f);
	fread(&gameMainPlayer, 1, sizeof(gameMainPlayer), f);
	fclose(f);

	// Switch view to main player
	viewPlayer = gameMainPlayer;

	// Notify the interface of in-game status
	interface_SetGameStatus(true);

	// Game is now running
	gameRunning = true;
	gamePaused = false;

	console_Print("- Game loaded...");
}

void game_CreateEmpty()
{
	// Clear all player, missile, explosion, and planet entities
	player_Clear();
	missile_Clear();
	explosion_Clear();

	// Clear score counts
	gameRedTeam = 0;
	gameBlueTeam = 0;
	gameGreenTeam = 0;

	// Notify the interface of in-game status
	interface_SetGameStatus(true);

	// Game is now running
	gameRunning = true;
	gamePaused = false;
}

void game_CreateTeamDeathmatch()
{
	game_CreateEmpty();
	game_LoadScene("deathmatch.xml");
	gameMainPlayer = player_CreateSinglePlayer();
	viewPlayer = gameMainPlayer;
}

void game_CreateSundayDrive()
{
	game_CreateEmpty();
	game_LoadScene("sundaydrive.xml");
	gameMainPlayer = player_CreateSinglePlayer();
	viewPlayer = gameMainPlayer;
}

void game_CreateServer()
{
	game_CreateTeamDeathmatch();
	//net_StartServer();
}

void game_JoinServer()
{
	//net_Connect(config.ServerAddress);
	gameNetwork = true;
}

void game_End()
{
	// Clear all player, missile, explosion, and planet entities
	player_Clear();
	missile_Clear();
	explosion_Clear();

	gameRunning = false;
	interface_SetGameStatus(false);
}

void game_Init()
{
	player_Init();
	stars_Init(config.StarCount);
	planets_Init();
	missile_Init();
	explosion_Init();
	item_Init();
	ships_Init();
}

void game_Frame()
{
	if (gameRunning == true)
	{
		if (gamePaused == false)
		{
			if (gameNetwork == false)
				entities_ProcessMovement();

			view_Frame();
		}

		stars_Frame();
		target_DrawDisplay();

		entities_DrawEntities();
	}
}
