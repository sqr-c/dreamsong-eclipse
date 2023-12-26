#include "dglobal.h"

void(*renderfn)() = &render;
int calibrator = 0;
int ls_ptr = 0;
char statusr = 1;
char status_bar_mode = 1;

int gtidx = 0;

char screen_mode;
char seltik = 0;
char blit_tick_elem_cache = 1;

char t_bptr_limit = T_BPTR_LIMIT_DEFAULT;
char t_bidx = 0;
char t_aidx = 0;
char t_width;

int tileo = 0;

const Uint8* keys;
SDL_Rect screenrect;
SDL_Rect surfacerect;
SDL_Rect tilerect;
SDL_Window* window = NULL;
SDL_Renderer* renderer;

unsigned short tilew, tileh;
SDL_Texture* tile;

char deltakey = 0;
int chdx, chdy;

int npcai = -1;

s_spritedata sprdata[NUM_SPRITE];
m_map map;

int s_mapw;
int s_maph;

int debug_ch_collison = 0;

int tik = 0;

char gcdflag = 0;

void touch(const char* file)
{
	FILE* f;
	f = fopen(file, "a");
	fclose(f);
}

char sel_elem_menu_ofs;

icac_itemcache_min trade_in_cache;
icac_itemcache_min trade_out_cache;

void* trade_in;
void* trade_out;

void sm_exit_disp_cache()
{
	screen_mode = SCREEN_MODE_NORMAL;
	ls_ptr = 0;
}

void trade_screen_exit_idx(int idx)
{
	screen_mode = SCREEN_MODE_TEXT;
	t_aidx = 0;
	t_bidx = 0;
	gtidx = idx;
}

#define trade_screen_exit()	trade_screen_exit_idx(16)

void trade_std_exit()
{
	EXIT_MENU();
	menu_sel_idx_2 = -1;
}

char trade_item_item_s()
{
	/*
						if (menu_sel_idx_2 == 0)
							tradefn();
						else if (menu_sel_idx_2 == 1)
							goto exit;
	*/
	if (menu_sel_idx_2 == 0)
	{
		int i;
		for (i = 0; i < cache_top; i++) if ((cache[i].id == trade_in_cache.id)) break;

		if (1 ^ (i < cache_top))
		{
			trade_screen_exit_idx(TEXT_TRADE_EXIT_STD);
			extern char npcfn_state;
			npcfn_state = 2;
			return 0;
		}

		if (cache[i].mul < trade_in_cache.mul)
		{
			trade_screen_exit_idx(TEXT_TRADE_EXIT_STD);
			return 1;
		}
		if ((cache[i].mul -= trade_in_cache.mul) <= 0)
			rm_cache(cache, &cache_top, i);

		{
			icac_itemcache buf;
			buf.id = trade_out_cache.id;
			buf.mul = trade_out_cache.mul;
			buf.active = 0;
			add_cache(cache, &cache_top, ICACHE_MAX_SIZE, &buf);
		}

		trade_screen_exit();

		return 0;
	}
	else if (menu_sel_idx_2 == 1)
	{
		extern char npcfn_state;
		npcfn_state = 0;

		trade_std_exit();

		return 1;
	}
}

//todo: this code wont work, change
char trade_item_elem()
{
	if (menu_sel_idx_2 == 0)
	{
		if (ch.elemidx + 1 > (sizeof(ch.elem) / sizeof(e_element))) return 0;

		rm_cache(cache, &cache_top, ((icac_itemcache*)trade_in - cache));
		ch.elem[++ch.elemidx] = *((e_element*)trade_out);

		trade_screen_exit();

		return 0;
	}
	else if (menu_sel_idx_2 == 1)
	{
		trade_std_exit();
		return 1;
	}
}

//TODO: remove this code, replace with overwriting cache in the .smd file, remove anything .cid related
char cac_init_cache_flag = 0;
void init_cache()
{
	if (cac_init_cache_flag)
		return;

	readallcid(PATH_CID, &(map.cache));

	int t = time(NULL);

	for (int i = 0; i < map.cache.size; i++)
	{
		if (map.cache.arr[i].cd == -1 || map.cache.arr[i].cd > t)
		{
			int idx = p2idx(
				map.cache.arr[i].x,
				map.cache.arr[i].y,
				map.arr[1].width
			);
			s_sprite_a* at = &(map.arr[1].tile.arr[idx]);

			for (int i = 0; i < at->size; i++)
			{
				at
					->arr[i]
					.id = -1;
				at
					->arr[i]
					.flag = 0;
			}

			map.cache.arr[i--] = map.cache.arr[--map.cache.size];
		}
	}
}

char add_elem(ch_char* c, e_element* e)
{
	int i;
	for (i = 0; c->elem[i].id != -1 && i < ELEM_CACHE_MAX; i++);

	if (i >= ELEM_CACHE_MAX) return -1;

	c->elem[i] = *e;

	if (i + 1 < ELEM_CACHE_MAX)
		c->elem[i + 1].id = -1;

	return i;
}

char(*tradefn)() = &trade_item_elem;

int disp_elem_idx = -1;

it_iterfn_a cyfn;
dm_tablef_a dmtable;

disp_displacement disps[2];

int lifec(ch_char* c)
{
	char idx = c != &ch;
	return c->lifea + disps[idx].dsplif - disps[!idx].dspbrk;
}

int rar_fn0_adj = RAR_FN0_ADJ_DEF;

unsigned short rarity_fn_0(short x, int idx, int len)
{
	return
		sqrt(x / (rar_fn0_adj / RAR_FN0_ADJ_DEF))
		/ sqrt(RARITY_FN0_MAX)
		* len
		+ idx
		;
}

unsigned short rarity_fn_pkg(short x, int idx, int len)
{
	float r = (x % 1024) / 1024.0f;

	unsigned short buf = len;

	{
		r *= 1.5f;
		if (r > 1) r = 1;
	}

	buf *= r;
	if (buf >= len) buf = len - 1;
	buf += idx;

	return buf;
}

unsigned short (*rar_fn[])(short x, int idx, int len) = { &rarity_fn_0, &rarity_ash_fn0, };

float randf(unsigned int r)
{
	return (r % UINT32_MAX) / (float)(UINT32_MAX);
}

void rdr_flsh_scr()
{
	static char state = 0;

	if (state > 4)
	{
		state = 0;
		cyfn.size--;
	}

	if (state % 3 != 0)
		blit_fill_rect(0xFF * (state % 2));

	state++;
}

void render_end()
{
	tik = SDL_GetTicks();

	SDL_RenderClear(renderer);
	renderfn();
	SDL_RenderPresent(renderer);

	SDL_Delay(RENDER_CYCLE_DELAY);
}

void ch_action_pause()
{
	for (; keys[KEY_CH_ACTION];)
	{
		SDL_Event e;
		SDL_PollEvent(&e);
		keys = SDL_GetKeyboardState(NULL);

		render_end();
	}
}

void (*disp_elem_close)() = NULL;

//length determined by 0
char table_item_unique[] =
{
	12, 15, 20, 22, 23, ASHES_ID_0, ASHES_ID_1, 26,
	ASHES_ID_2, ASHES_ID_3,
	0,
};

mst_mapstate ms_mapstate;
unsigned short rand_state;

CIDSPR pub_cache_uniq[] =
{
	//0
	{ 1, SPR_BLOCK / 8192, },
	//1
	{ 1, 0, },
	//2
	{ ASHES_ID_1, 0, },
	//3
	{ 1, SPR_BLOCK / 6144, },
	//4
	{ 26, SPR_BLOCK / 64, },
	//5
	{ 19, SPR_BLOCK / 1024, },
	//6
	{ 2, SPR_BLOCK / 6144, },
	//7
	{ 2, SPR_BLOCK / 1024, },
	//8
	{ 2, 0, },
	//9
	{ 5, 0, },
	//10
	{ ASHES_ID_2, 0, },
};

e_element pub_elem_uniq[] =
{
	//0
	{
		48,
		0, 0,
		{9, 10, -1, -1, },
	},
	//1
	{
		49,
		0, 0,
		{9, 10, -1, -1, },
	},
	//2
	{
		40,
		0, 0,
		{13, 22, 23, -1, },
	},
	//3
	{
		20,
		0, 0,
		{10, 9, -1, -1, },
	},
	//4
	{
		38,
		0, 0,
		{27, 28, 29, -1, },
	},
	//5
	{
		25,
		0,0,
		{30 ,31 ,32 ,-1, },
	},
	//6
	{
		23,
		0,0,
		{33, 34, 35, -1, },
	},
	//7
	{
		26,
		0,0,
		{36, 37, 38, -1, },
	},
	//8
	{
		46,
		0,0,
		{ 43, 44, 45, -1, },
	},
	//9
	{
		45,
		0, 0,
		{17, 18, -1, -1, },
	},
	//10
	{
		22,
		0,0,
		{47, 48, 49, -1, },
	},
	//11
	{
		42,
		0,0,
		{50, 39, 51, -1, },
	},
	//12
	{
		48,
		0, 0,
		{50, 48, 35, -1, },
	},
	//13
	{
		47,
		0, 0,
		{ 53, 36, 35, -1, },
	},
	//14
	{
		30,
		0, 0,
		{ 39, 55, 41, -1, },
	},
	//15
	{
		32,
		0, 0,
		{ 56, 57, 58, -1, },
	},
};

n_npc_min pub_ennpc_uniq[] =
{
	//0
	{
		//elems
		{
			{
					38,
					0, 0,
					{0,-1,-1,-1},
			},
			{-1,},
		},
		//str
		0,
		//spr
		SPR_BLOCK * 1,
		//cast cycle
		1024,

		//sprite id
		0,
	},
	//1
	{
		//elems
		{
			{
				39,
				0, 0,
				{13, 4, 14, -1, },
			},
			{
				44,
				0, 0,
				{15, 2, 16, -1, },
			},
			{-1,},
		},
		//str
		1,
		//spr
		SPR_BLOCK * 3,
		//cast cycle
		256,

		//sprite id
		0,
	},
	//2
	{
		//elems
		{
			{
				15,
				(unsigned short)(ELEM_SPR_BLOCK * 1),
				(unsigned short)(ELEM_SPR_BLOCK * 1),
				{ 13, 47, 14, -1, },
			},
			{-1,},
		},
		//str
		1.0f,
		//spr
		SPR_BLOCK * 2,
		//cast cycle
		256,

		//sprite id
		0,
	},
	//3
	{
		//elems
		{
			{
				11,
				(unsigned short)(ELEM_SPR_BLOCK * 1),
				(unsigned short)(ELEM_SPR_BLOCK * 1),
				{ 47, 44, 46, 49, },
			},
			{-1,},
		},
		//str
		1.75f,
		//spr
		SPR_BLOCK * 2,
		//cast cycle
		256,

		//sprite id
		0,
	},
	//4
	{
		//elems
		{
			{
				15,
				(unsigned short)(ELEM_SPR_BLOCK * 1),
				(unsigned short)(ELEM_SPR_BLOCK * 1),
				{ 13, 47, 14, -1, },
			},
			{-1,},
		},
		//str
		1.0f,
		//spr
		SPR_BLOCK * 2,
		//cast cycle
		1,

		//sprite id
		0,
	},
	//5
	{
		//elems
		{
			{
				22,
				0,0,
				{47, 48, 49, -1, },
			},
			{-1,},
		},
		//str
		8.0f,
		//spr
		SPR_BLOCK * 2,
		//cast cycle
		1,

		//sprite id
		0,
	},
	//6
	{
		//elems
		{
			{
				42,
				0,0,
				{50, 39, 51, -1, },
			},
			{-1,},
		},
		//str
		8.0f,
		//spr
		SPR_BLOCK * 2,
		//cast cycle
		1,

		//sprite id
		0,
	},
};

CIDSPR* pub_cache[] =
{
	//SECTION:	0 2
	&pub_cache_uniq[0], &pub_cache_uniq[1],
	//SECTION:	2 3
	&pub_cache_uniq[2], &pub_cache_uniq[3], &pub_cache_uniq[1],
	//SECTION:	5 5
	&pub_cache_uniq[4], &pub_cache_uniq[5], &pub_cache_uniq[6], &pub_cache_uniq[3], &pub_cache_uniq[1],
	//SECTION:	10 3
	&pub_cache_uniq[7], &pub_cache_uniq[8], &pub_cache_uniq[1],
	//SECTION:	13 2
	&pub_cache_uniq[1], &pub_cache_uniq[9],
	//SECTION:	15 3
	&pub_cache_uniq[1],&pub_cache_uniq[9],
	//SECTION:	15 3
	&pub_cache_uniq[1],&pub_cache_uniq[9],
};

e_element* pub_elem[] =
{
	//SECTION:	0 2
	&pub_elem_uniq[0], &pub_elem_uniq[1],
	//SECTION:	2 2
	&pub_elem_uniq[2], &pub_elem_uniq[3],
	//SECTION:	4 2
	&pub_elem_uniq[4], &pub_elem_uniq[5],
	//SECTION:	6 2
	&pub_elem_uniq[6], &pub_elem_uniq[7],
	//SECTION:	8 3
	&pub_elem_uniq[8], &pub_elem_uniq[5],&pub_elem_uniq[9],
	//SECTION:	11 3
	&pub_elem_uniq[10], &pub_elem_uniq[11],&pub_elem_uniq[7],
	//SECTION:	14 3
	&pub_elem_uniq[11], &pub_elem_uniq[3],&pub_elem_uniq[9],
	//SECTION:	17 3
	&pub_elem_uniq[10], &pub_elem_uniq[12],&pub_elem_uniq[7],
	//SECTION:	20 2
	&pub_elem_uniq[13], &pub_elem_uniq[3],
	//SECTION:	22 3
	&pub_elem_uniq[14], &pub_elem_uniq[15], &pub_elem_uniq[8],
	//SECTION:	25 4
	&pub_elem_uniq[4], &pub_elem_uniq[8],&pub_elem_uniq[9], &pub_elem_uniq[7],
};

n_npc_min* pub_ennpc[] =
{
	//SECTION:	0 1
	&pub_ennpc_uniq[0],
	//SECTION:	1 1
	&pub_ennpc_uniq[1],
	//SECTION:	2 2
	&pub_ennpc_uniq[2], &pub_ennpc_uniq[3],
	//SECTION:	4 4
	&pub_ennpc_uniq[5], &pub_ennpc_uniq[4], &pub_ennpc_uniq[6], &pub_ennpc_uniq[3],
};

mst_mapstate pub_map_state[] =
{
	//0
	{
		//index, length, rarity fn: cache
		0, 1, 0,
		//index, length, rarity fn: elem
		0, 2, 0,
		//index, length, rarity fn: npc
		0, 1, 0,

		//cache, elem, npc event rarity: 0-255
		32,
		4,
		0,
	},
	//1
	{
		//index, length, rarity fn: cache
		0, 2, 0,
		//index, length, rarity fn: elem
		2, 2, 0,
		//index, length, rarity fn: npc
		0, 1, 0,

		//cache, elem, npc event rarity: 0-255
		32,
		4,
		0,
	},
	//2
	{
		//index, length, rarity fn: cache
		2, 2, 0,
		//index, length, rarity fn: elem
		4, 2, 0,
		//index, length, rarity fn: npc
		0, 1, 0,

		//cache, elem, npc event rarity: 0-255
		32,
		4,
		0,
	},
	//3
	{
		//index, length, rarity fn: cache
		2, 2, 0,
		//index, length, rarity fn: elem
		6, 2, 0,
		//index, length, rarity fn: npc
		0, 1, 0,

		//cache, elem, npc event rarity: 0-255
		32,
		4,
		0,
	},
	//4
	{
		//index, length, rarity fn: cache
		2, 2, 0,
		//index, length, rarity fn: elem
		3, 1, 0,
		//index, length, rarity fn: npc
		0, 1, 0,

		//cache, elem, npc event rarity: 0-255
		32,
		4,
		0,
	},
	//5
	{
		//index, length, rarity fn: cache
		5, 5, 0,
		//index, length, rarity fn: elem
		8, 3, 0,
		//index, length, rarity fn: npc
		0, 1, 0,

		//cache, elem, npc event rarity: 0-255
		32,
		4,
		0,
	},
	//6
	{
		//index, length, rarity fn: cache
		5, 5, 0,
		//index, length, rarity fn: elem
		8, 3, 0,
		//index, length, rarity fn: npc
		0, 1, 0,

		//cache, elem, npc event rarity: 0-255
		32,
		4,
		0,
	},
	//7
	{
		//index, length, rarity fn: cache
		10, 3, 0,
		//index, length, rarity fn: elem
		14, 3, 0,
		//index, length, rarity fn: npc
		1, 1, 0,

		//cache, elem, npc event rarity: 0-255
		32,
		4,
		0,
	},
	//8
	{
		//index, length, rarity fn: cache
		13, 2, 0,
		//index, length, rarity fn: elem
		8, 3, 0,
		//index, length, rarity fn: npc
		1, 1, 0,

		//cache, elem, npc event rarity: 0-255
		(32 + 16),
		2,
		0,
	},
	//9
	{
		//index, length, rarity fn: cache
		15, 3, 0,
		//index, length, rarity fn: elem
		8, 3, 0,
		//index, length, rarity fn: npc
		2, 3, 1,

		//cache, elem, npc event rarity: 0-255
		(32 + 8 + 4),
		3,
		0,
	},
	//10
	{
		//index, length, rarity fn: cache
		15, 3, 0,
		//index, length, rarity fn: elem
		17, 3, 0,
		//index, length, rarity fn: npc
		2, 3, 1,

		//cache, elem, npc event rarity: 0-255
		(32 + 2),
		5,
		0,
	},
	//11
	{
		//index, length, rarity fn: cache
		15, 3, 0,
		//index, length, rarity fn: elem
		-1, -1, 0,
		//index, length, rarity fn: npc
		2, 3, 1,

		//cache, elem, npc event rarity: 0-255
		(32 + 2),
		0,
		0,
	},
	//12
	{
		//index, length, rarity fn: cache
		15, 3, 0,
		//index, length, rarity fn: elem
		17, 3, 0,
		//index, length, rarity fn: npc
		2, 3, 1,

		//cache, elem, npc event rarity: 0-255
		(32 + 2),
		5,
		0,
	},
	//13
	{
		//index, length, rarity fn: cache
		15, 3, 0,
		//index, length, rarity fn: elem
		22, 3, 0,
		//index, length, rarity fn: npc
		2, 3, 1,

		//cache, elem, npc event rarity: 0-255
		2,
		5,
		0,
	},
	//14
	{
		//index, length, rarity fn: cache
		15, 2, 0,
		//index, length, rarity fn: elem
		25, 4, 0,
		//index, length, rarity fn: npc
		4, 4, 0,

		//cache, elem, npc event rarity: 0-255
		(16),
		112,
		0,
	},
};

mst_tilemapstate tile_map_state[] =
{
	{
		-1, -1,
		//MAP SEEK:		./translation/wm0/0007.smd		enter town 1
		2012128,
		&pub_map_state[0],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm0/0011.smd		exit starting town
		2080968,
		&pub_map_state[0],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm0/0054.smd	enter hollow cave
		2808528,
		&pub_map_state[0],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm0/0056.smd	town 1 digin
		2846904,
		&pub_map_state[0],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm0/0037.smd	town 1 digin
		2526980,
		&pub_map_state[0],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm0/0031.smd	town 1 digin
		2427580,
		&pub_map_state[0],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm0/0042.smd	town 1 digin
		2610552,
		&pub_map_state[0],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/m7/0000.smd		hollow cave reward
		98236,
		&pub_map_state[1],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm1/0003.smd		town 2 sect 2
		3222816,
		&pub_map_state[2],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm1/0001.smd		town 2 sect 2
		3189848,
		&pub_map_state[2],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm1/0004.smd		town 2 sect 2
		3239400,
		&pub_map_state[2],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm1/0024.smd		town 2 sect 2
		3570752,
		&pub_map_state[2],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm1/0042.smd		town 2 sect 1
		3880152,
		&pub_map_state[3],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm1/0079.smd		town 2 digout
		4490960,
		&pub_map_state[3],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm1/0039.smd		town 2 sect 1
		3829932,
		&pub_map_state[3],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm1/0096.smd		town 2 digout
		4771188,
		&pub_map_state[3],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm5/0003.smd		hollow cave main
		7505560,
		&pub_map_state[4],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/t0/0000.smd		sky train load
		929884,
		&pub_map_state[9],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm2/0021.smd		town 3 sect 1
		5483256,
		&pub_map_state[6],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm2/0031.smd		town 3 sect 1
		5648116,
		&pub_map_state[6],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm2/0007.smd		town 3 sect 1
		5252480,
		&pub_map_state[6],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm2/0037.smd		enter sky train
		5747020,
		&pub_map_state[6],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/r0/0000.smd		enter route 0
		155096,
		&pub_map_state[7],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/r0/0024.smd		exit route 0
		550712,
		&pub_map_state[7],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/t1/0000.smd		load sky train 2
		1243080,
		&pub_map_state[9],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm2/0014.smd		enter sky train 2. sky train 2 sect 0
		5367868,
		&pub_map_state[6],
	},
	{
		2, 14,
		//MAP SEEK:		./translation/t1/0009.smd		sky train 1 sect 0
		1391436,
		&pub_map_state[9],
	},
	{
		3, 14,
		//MAP SEEK:		./translation/t1/0009.smd		sky train 1 sect 1
		1391436,
		&pub_map_state[6],
	},
	{
		4, 8,
		//MAP SEEK:		./translation/r1/0000.smd		enter r1
		583700,
		&pub_map_state[10],
	},
	{
		5, 8,
		//MAP SEEK:		./translation/r1/0018.smd		exit r1
		880412,
		&pub_map_state[10],
	},
	{
		1, 2,
		//MAP SEEK:		./translation/wm4/0003.smd		town 4 sect 1
		6311876,
		&pub_map_state[11],
	},
	{
		6, 4,
		//MAP SEEK:		./translation/wm4/0015.smd		town 4 sect 1
		6518744,
		&pub_map_state[11],
	},
	{
		6, 16,
		//MAP SEEK:		./translation/wm4/0008.smd		town 4 sect 1
		6398976,
		&pub_map_state[11],
	},
	{
		6, 7,
		//MAP SEEK:		./translation/wm4/0030.smd		town 4 sect 2
		6767624,
		&pub_map_state[12],
	},
	{
		6, 5,
		//MAP SEEK:		./translation/wm4/0059.smd		town 4 sect 2
		7249840,
		&pub_map_state[12],
	},
	{
		6, 5,
		//MAP SEEK:		./translation/wm4/0033.smd		town 4 sect 2
		6817076,
		&pub_map_state[12],
	},
	{
		6, 8,
		//MAP SEEK:		./translation/wm4/0037.smd		town 4 sect 3
		6883012,
		&pub_map_state[13],
	},
	{
		6, 15,
		//MAP SEEK:		./translation/wm4/0056.smd		town 4 sect 3
		7200388,
		&pub_map_state[13],
	},
	{
		6, 9,
		//MAP SEEK:		./translation/wm4/0056.smd		town 4 sect 3
		7200388,
		&pub_map_state[13],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/z0s0/0000.smd		blue cave entrance
		7790028,
		&pub_map_state[14],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/z0s5/0001.smd		blue cave exit
		8856728,
		&pub_map_state[14],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm1/0105.smd		town 2 sect 2
		4925532,
		&pub_map_state[2],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/wm4/0054.smd		town 4 digout from sky train 1
		7163816,
		&pub_map_state[11],
	},
	{
		-1, -1,
		//MAP SEEK:		./translation/t1/0033.smd		sky train 1 digout from town 4
		1787052,
		&pub_map_state[6],
	},
};

void (*select_none_fn)() = NULL;

unsigned short rarity_ash_fn0(short x, int idx, int len)
{
	static unsigned char state = 0;

	if (state++ % 2)
		return
		idx +
		len * ((float)(x) / RARITY_FN0_MAX)
		;

	char part_table[] =
	{
		ASHES_ID_0,
		ASHES_ID_2,
		ASHES_ID_3,
	};

	int sprl = (pub_ennpc[idx]->espr + 1) / SPR_BLOCK;

	int tabi;
	for (tabi = sizeof(part_table) / sizeof(*part_table) - 1; tabi != 0; tabi--)
		for (int ii = 0; ii < cache_top; ii++)
			if (cache[ii].id == part_table[tabi])
				break;

	float m = (sprl / ((tabi + sprl) + ((float)x / rar_fn0_adj)));

	int r = idx + len - len * m;

	if (r < idx) r = idx;
	else if (r > idx + len) r = idx + len;

	return r;
}

void tile_set_map_state()
{
	int i;
	for (i = 0;
		i < (sizeof(tile_map_state) / sizeof(*tile_map_state))
		; i++)
		if ((
			(tile_map_state[i].x < 0
				||
				tile_map_state[i].y < 0
				|| (tile_map_state[i].x + defr == ch.dx
					&& tile_map_state[i].y + defr == ch.dy))
			&& tile_map_state[i].seek == mapseek))
		{
			memcpy(&ms_mapstate, tile_map_state[i].state, sizeof(mst_mapstate));
			return;
		}
}