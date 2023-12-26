#include "arena.h"

#include "path.h"
#include "dglobal.h"

int32_t arena_seek;

seekxy arenasxy[2];
char astate = 0;

ar_arenareward arena_rew[] =
{
	//SECTION:	0 3
	//cache, token, spr, elem,
	{ {24, -1, 0,}, {0}, {0}, {-1}, },
	{ {PARK_TICKET_ID, -1, 0,}, {0}, {0}, {-1}, },
	{ {1, 5, 0,}, {0}, {SPR_BLOCK / 8}, {-1}, },

	//SECTION:	3 1
	//cache, token, spr, elem,
	{ {27, 1, 0,}, {0}, {0}, {-1}, },
	//SECTION:	4 2
	{ {27, 1, 0,}, {2}, {SPR_BLOCK / 8}, {-1}, },
	{ {27, 1, 0,}, {1}, {SPR_BLOCK / 8}, {-1}, },
};

ar_arenaadv arena_adv_template[] =
{
	//SECTION:	0 3
	{
		//npc info
		{
			//elements
			{
				{
					33,
					0, 0,
					{9,7,8,10},
				},
				{-1},
			},

			//npc stats: strength, spr, cast cycle,		sprite id
			.5f,
			SPR_BLOCK * 2,
			1024 * 2,
			0,
		},

		//rewards
		&arena_rew[0]
	},
	{
		{
			//elements
			{
				{
					32,
					0, 0,
					{9,7,8,10},
				},
				{-1},
			},

			//npc stats: strength, spr, cast cycle,		sprite id
			.5f,
			SPR_BLOCK * 2,
			1024 * 2,
			0,

		},

		//rewards
		&arena_rew[1]
	},
	{
		{
			//elements
			{
				{
					25,
					0, 0,
					{9,7,8,10},
				},
				{-1},
			},

			//npc stats: strength, spr, cast cycle,		sprite id
			.5f,
			SPR_BLOCK * 2,
			1024,
			0,
		},

		//rewards
		&arena_rew[2]
	},
	//SECTION:	3 3
	{
		{
			//elements
			{
				{
					40,
					ELEM_SPR_BLOCK * 1, ELEM_SPR_BLOCK * 1,
					{13, 22, 23, -1, },
				},
				{
					35,
					ELEM_SPR_BLOCK * 1, ELEM_SPR_BLOCK * 1,
					{ 43, 44, 45, -1, },
				},
				{-1},
			},

			//npc stats: strength, spr, cast cycle,		sprite id
			(.5f + .125f),
			SPR_BLOCK * 2,
			2048,
			0,
		},

		//rewards
		&arena_rew[3 + 0],
	},
	{
		{
			//elements
			{
				{
					36,
					ELEM_SPR_BLOCK * 1, ELEM_SPR_BLOCK * 1,
					{ 43, 48, 45, -1, },
				},
				{
					31,
					ELEM_SPR_BLOCK * 1, ELEM_SPR_BLOCK * 1,
					{ 47, 48, 41, -1, },
				},
				{-1},
			},

			//npc stats: strength, spr, cast cycle,		sprite id
			(.5f + .125f),
			SPR_BLOCK * 2,
			1024,
			0,
		},

		//rewards
		&arena_rew[3 + 0],
	},
	{
		{
			//elements
			{
				{
					41,
					ELEM_SPR_BLOCK * 1, ELEM_SPR_BLOCK * 1,
					{ 52,53,54,-1, },
				},
				{
					44,
					ELEM_SPR_BLOCK * 1, ELEM_SPR_BLOCK * 1,
					{ 15, 2, 16, -1, },
				},
				{-1},
			},

			//npc stats: strength, spr, cast cycle,		sprite id
			(.5f + .125f),
			SPR_BLOCK * 2,
			1024,
			0,
		},

		//rewards
		&arena_rew[3 + 0],
	},

	//SECTION:	6 3
	{
		{
			//elements
			{
				{
					39,
					0, 0,
					{11, 13, 4, 14, },
				},
				{-1},
			},

			//npc stats: strength, spr, cast cycle,		sprite id
			1.5f,
			SPR_BLOCK * 3,
			1024,
			0,

		},

		//rewards
		&arena_rew[4 + 0],
	},
	{
		{
			//elements
			{
				{
					18,
					0, 0,
					{ 63, 64, 65, -1, },
				},

				{-1,},
			},

			//npc stats: strength, spr, cast cycle,		sprite id
			1.0f,
			SPR_BLOCK,
			512,
			0,
		},

		//rewards
		&arena_rew[4 + 0],
	},
	{
		{
			//elements
			{
				{
					40,
					0, 0,
					{ 66, 67, 68, -1, },
				},

				{-1,},
			},

			//npc stats: strength, spr, cast cycle,		sprite id
			1.5f,
			SPR_BLOCK,
			128,
			0,
		},

		//rewards
		&arena_rew[4 + 1],
	},
};

ar_arena arena_template[] =
{
	{
		//load in arena position
		3, 3,
		//load in arena wait position
		4, 3,
		//MAP SEEK:		./translation/m5/0000.smd		arena 0
		78292,
		//MAP SEEK:		./translation/m4/0000.smd		arena wait entrance 0
		74364,

		//ennpc
		&arena_adv_template[0],
	},
	{
		//load in arena position
		5, 5,
		//load in arena wait position
		9, 5,
		//MAP SEEK:		./translation/m9/0000.smd		arena 1
		138612,
		//MAP SEEK:		./translation/m10/0000.smd		arena wait entrance 1
		34156,

		//ennpc
		&arena_adv_template[3],
	},
};
int arena_temp_idx = -1;
int arena_temp_npc_idx = -1;

char rewflag;
int advidx;

n_npc arenanpc;
#define AREN_TEMPLATE_LENGTH	(sizeof(arena_template) / sizeof(ar_arena))

void en_npc_update();
extern float castp;

void update_arena()
{
	static char arenastate = 0;

	if (arenastate == 0 && map.npca.arr[map.npca.size - 1].ch.lifea + disps[1].dsplif <= 0)
	{
		arenastate = 1;

		ch.state = CH_STATE_WALK;
		ch_set_state(&ch, ch.rot, CH_STATE_IDLE);

		for (int tik = SDL_GetTicks(); tik + 2000 >= SDL_GetTicks();)
		{
			renderfn();
			SDL_RenderPresent(renderer);
			SDL_Delay(RENDER_CYCLE_DELAY);
		}
	}

	if (ch.lifea + disps[0].dsplif - disps[1].dspbrk < 0) astate = 0;

	en_npc_update();

	if (map.npca.size == 0)
	{
		arenastate = 0;

		renderfn();
		SDL_RenderPresent(renderer);
		SDL_Delay(1500);

		map.npca.size++;
		map.npca.arr[map.npca.size - 1].ch.lifea = CH_MAX_LIFE;
		map.npca.arr[map.npca.size - 1].istate = NPC_STATE_DEFAULT;

		map.npca.arr[map.npca.size - 1].fnidx = 5;
		map.npca.arr[map.npca.size - 1].textidx = 51 - 10;

		{
			map.npca.arr[map.npca.size - 1].ch.dx = arena_template[arena_temp_idx].cx - 1 + defr;
			map.npca.arr[map.npca.size - 1].ch.dy = arena_template[arena_temp_idx].cy + defr;
		}

		npcai = map.npca.size - 1;
	}
}

void init_arena()
{
	astate++;

	screen_mode = SCREEN_MODE_NORMAL;

	free_map(&map);
	map = readmapp(PATH_SMD, arena_template[arena_temp_idx].arenamap);

	for (int i = 0; i < sizeof(map.adj) / sizeof(int32_t); i++)
		map.adj[i] = -1;

	npcai = map.npca.size - 1;
	ch.dx = ch.sprite.x = arena_template[arena_temp_idx].cx;
	ch.dy = ch.sprite.y = arena_template[arena_temp_idx].cy;
	ch.spra = ch.sprb;
	ch.lifea = ch.lifeb;

	ch.rot = 2;
	ch.state = CH_STATE_IDLE;
	ch_set_state(&ch, ch.rot, CH_STATE_IDLE);

	{
		linkmap();
		ch.sprite.x = (ch.dx += defr);
		ch.sprite.y = (ch.dy += defr);
	}

	if (map.npca.capacity == 0)
		map.npca = create_NPCA(1);

	push_en_npc(
		&arena_template[arena_temp_idx]
		.arr[arena_temp_npc_idx].npcm
	);

	{
		ch_char* c = &
			map.npca.arr[0].ch;
		c->dx = c->sprite.x = arena_template[arena_temp_idx].cx - 1 + defr;
		c->dy = c->sprite.y = arena_template[arena_temp_idx].cy + defr;
	}

	{
		rmf_IterFNA(&cyfn, &en_npc_update);
		push_back_IterFNA(&cyfn, &update_arena);
	}
}

char tile_load_arena(m_map* m, int loadidx)
{
	if (astate < map.npca.size)
		return 1;

	astate = 0;

	{
		free_map(&map);
		map = readmapp(PATH_VAR_MAP, mapseek = arenasxy[0].seek);
		ch.sprite.x = ch.dx = arenasxy[0].x;
		ch.sprite.y = ch.dy = arenasxy[0].y;
		linkmap();

		push_back_mapload(&map.loads, &loadstack.arr[loadstack.size - 1].load);
	}

	return 1;
}

void trs_ent_arena()
{
	int i;
	for (i = 0; i < cache_top; i++)
		if (cache[i].id == ARENA_TICKET_ID)
			break;

	dec_rm_cache(&cache[i]);
	writesed_npos(PATH_SED);

	menu_sel_idx_0 = -1;
	menu_sel_idx_1 = -1;
	menu_sel_idx_2 = -1;

	if (astate == 0)
	{
		arenasxy[0].seek = mapseek;
		arenasxy[0].x = ch.sprite.x;
		arenasxy[0].y = ch.sprite.y;
	}

	mapseek = arena_template[arena_temp_idx].arenawaitmap;

	m_map buf = readmapp(PATH_SMD, mapseek);
	if (mapfp == NULL) mapfp = fopen(PATH_SMD, "rb+");
	free_map(&map);
	map = buf;
	linkmap();

	for (int i = 0; i < map.npca.size; i++)
	{
		map.npca.arr[i]
			.ch.spra = map.npca.arr[i]
			.ch.sprb = arena_template[arena_temp_idx]
			.arr[i].npcm.espr;

		memcpy(map.npca.arr[i]
			.ch.elem
			, arena_template[arena_temp_idx]
			.arr[i].npcm.eelem
			, sizeof(arena_template[arena_temp_idx].arr[i].npcm.eelem)
		);

		map.npca.arr[i]
			.ch.spriteo =
			arena_template[arena_temp_idx]
			.arr[i].npcm.sprite_id * CHAR_SPRD_SIZE;

		map.npca.arr[i].ch.state = CH_STATE_WALK;
		ch_set_state(&map.npca.arr[i].ch, 0, CH_STATE_IDLE);
	}

	if (advidx != -1)
	{
		map.npca.arr[advidx].istate = NPC_STATE_INACTIVE;
		writensm(PATH_NSM, &map.npca.arr[advidx]);
	}

	for (int i = 0; i < map.npca.size; i++)
		readnsm(PATH_NSM,
			&map.npca.arr[i]);

	map.loads.arr[0].seek = -1;

	tile_load_map_spec = &tile_load_arena;

	npcai = -1;

	ch.dx = ch.sprite.x = arena_template[arena_temp_idx].cxw + defr;
	ch.dy = ch.sprite.y = arena_template[arena_temp_idx].cyw + defr;

	ch.state = CH_STATE_WALK;
	ch_set_state(&ch, 2, CH_STATE_IDLE);
}
