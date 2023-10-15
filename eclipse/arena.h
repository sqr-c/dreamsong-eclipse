#pragma once

#include "map.h"

typedef struct
{
	icac_itemcache rew_ic;
	int rew_t;
	int spr;
	e_element rew_e;
} ar_arenareward;

typedef struct
{
	n_npc_min npcm;
	ar_arenareward* rew;
} ar_arenaadv; 

typedef struct
{
	int cx, cy;
	int cxw, cyw;

	long arenamap;
	long arenawaitmap;

	ar_arenaadv* arr;
} ar_arena;

extern ar_arenareward arena_rew[];

extern ar_arena arena_template[];
extern ar_arenaadv arena_adv_template[];

void trs_ent_arena();
void init_arena();

char tile_load_arena(m_map* m, int loadidx);

extern int arena_temp_idx;
extern int arena_temp_npc_idx;
extern char rewflag;