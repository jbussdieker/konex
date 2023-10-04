/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: player.h,v 1.34 2005/08/14 08:12:27 ecco Exp $   *
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
#define PLAYER_TYPE_LOCAL 0
#define PLAYER_TYPE_NET 1
#define PLAYER_TYPE_NPC 2
#define PLAYER_TYPE_REMOTE 3

#define PLAYER_STANCE_GOOD 1
#define PLAYER_STANCE_NEUTRAL 2
#define PLAYER_STANCE_BAD 3

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Function
/////////////////////////////////////////////////////////////////////////////
void 			player_Init(void);
void 			player_Clear(void);

entities_entity_t *player_Create(unsigned int shipid);
entities_entity_t *player_CreateSinglePlayer();
entities_entity_t *player_CreateAI();
entities_entity_t *player_CreateMultiplayer();

void 			player_ChangeTeams(entities_entity_t *player, int team);

void 			player_ProcessMovement();
void 			player_DrawPlayers();

void 			player_MissileCollision(entities_entity_t *player, entities_entity_t *missile);
void 			player_ItemCollision(entities_entity_t *player, entities_entity_t *missile);

void 			player_ProcessDeath(entities_entity_t *player);
entities_entity_t *player_ClosestEnemy(entities_entity_t *player);
void 			player_Term();
void 			player_DrawPlayer(entities_entity_t *player);
void 			player_ProcessInGame(entities_entity_t *player);

