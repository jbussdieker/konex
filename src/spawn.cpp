#include "defs.h"

static dword count;
static dword desired;
static entities_entity_t *selected;

static void count_Spawns(entities_entity_t *entity, void *data)
{
	int team = *((int *)data);

	if (entity->etype == ENTITY_SPAWN && entity->team == team)
		count++;
}

static void pick_Spawn(entities_entity_t *entity, void *data)
{
	int team = *(int *)data;

	if (entity->etype == ENTITY_SPAWN && entity->team == team)
	{
		count++;

		if (count == desired)
			selected = entity;
	}
}

entities_entity_t *spawn_GetSpawnPoint(int team)
{
	// Count spawn point for given team
	desired = 0;
	count = 0;
	selected = NULL;
	entities_Enumerate(count_Spawns, (void *)&team);

	// Pick random index of team spawn points
	desired = sys_Rand(1, count);
	count = 0;
	entities_Enumerate(pick_Spawn, (void *)&team);

	// Return pointer to randomly selected spawn point for given team
	return selected;
}