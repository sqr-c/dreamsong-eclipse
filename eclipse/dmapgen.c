#include "dmapgen.h"
#include "dynamicmap.h"

#include "mathfunction.h"

#include <string.h>

#define INIT_DMD_INC_SPRITE(s) \
	(s).x = xx;\
	(s).y = yy;\
	(s).z = 0;\
	(s).flip = 0;\
	(s).deltatime = 0;\

#define CACHE_FN_DM_STD_FIND	6
#define CACHE_FN_DM_FIND_ELEM	8
#define CACHE_DM_STD_CID	2

dm_dmapmd dmmdroot;

bt_breaktile dmbreaktemplate[] =
{
	//SECTION	0 0
	{ BREAKM_BLOCK * 1, BREAKM_BLOCK * 1, 7, },
	//SECTION	1 1
	{ BREAKM_BLOCK * 2, BREAKM_BLOCK * 2, 10, },
	//SECTION	2 1
	{ BREAKM_BLOCK * 2, BREAKM_BLOCK * 2, 0, },
};
int dmbreaktempi;

int dmcacrand;
cac_cache_min dmcachetemplate[] =
{
	//SECTION	0 2
	{SPR_BLOCK / (1024 * 8), 1, CACHE_FN_DM_STD_FIND},
	{SPR_BLOCK / (1024 * 8), 5, CACHE_FN_DM_STD_FIND},

	//SECTION	2 4
	{SPR_BLOCK / (1024 * 4), 1, CACHE_FN_DM_STD_FIND},
	{SPR_BLOCK / (1024 * 4), 5, CACHE_FN_DM_STD_FIND},
	{SPR_BLOCK / (1024 * 1), ARENA_TICKET_ID, CACHE_FN_DM_STD_FIND},
	{SPR_BLOCK / (1024 * 4), 5, CACHE_FN_DM_STD_FIND},

	//SECTION	6 3
	{0, ARENA_TICKET_ID, CACHE_FN_DM_STD_FIND},
	{0, 1, CACHE_FN_DM_STD_FIND},
	{0, 5, CACHE_FN_DM_STD_FIND},
};
int dmcachetempi;

e_element dmelemtemplate[] =
{
	{
		42,
		0, 0,
		{39, 40, 41, -1,},
	},
	{
		47,
		0, 0,
		{42, 40, 35, -1,}
	},
	{
		46,
		0, 0,
		{36, 40, 32, -1,}
	},

	{
		20,
		0, 0,
		{10, 9, -1, -1, },
	},
	{
		23,
		0,0,
		{33, 34, 35, -1, },
	},
	{
		26,
		0,0,
		{36, 37, 38, -1, },
	},

	//SECTION BEGINS:	5 3
	{
		35,
		0, 0,
		{ 43, 44, 45, -1, },
	},
	{
		44,
		0, 0,
		{ 60, 61, 62, -1, },
	},
	{
		45,
		0, 0,
		{17, 44, 18, -1, },
	},
};
int dmelemtempi;

int dmnpcrand;
n_npc_min dmnpctemplate[] =
{
	//SECTION	0 1
	{
		//elems
		{
			{
					46,
					0, 0,
					{36,40,32,-1},
			},
			{-1,},
		},
		//str
		0,
		//spr
		SPR_BLOCK * 1,
		//cast cycle
		1024,
	},
	//SECTION	1 1
	{
		//elems
		{
			//PRE:	park_elem[0],
			{
				39,
				0, 0,
				{13, 4, 14, -1, },
			},
			{-1,},
		},
		//str
		0,
		//spr
		SPR_BLOCK * 2,
		//cast cycle
		256,
	},
	//SECTION	2 1
	{
		//elems
		{
			//PRE:	pub_elem[14 + 1 * 0],
			{
				42,
				0,0,
				{50, 39, 51, -1, },
			},
			{-1,},
		},
		//str
		1.5f,
		//spr
		SPR_BLOCK * 2,
		//cast cycle
		512,
	},
}
;
int dmnpctempi;

inline void place_cache(m_map* m, cac_cache c)
{
	push_back_cache(&m->cache, &c);
}

void place_break(m_map* m, int x, int y, bt_breaktile* bt)
{
	int ati = p2idx(x, y, DM_LENGTH);

	if (dmnpcrand != 0 && 0 == xorrand() % dmnpcrand)
	{
		m_tilefn buf;

		buf.ix = x;
		buf.iy = y;
		buf.fnid = 0;

		push_back_tilefna(&m->tfn, &buf);

		bt->breakfnid = 2;
	}
	m->breaks.arr[ati] = *bt;

	return;
}

char tile_floor_exists(m_map* dmbufmap, int xx, int yy, int id)
{
	return dmbufmap->arr[0]
		.tile.arr[p2idx(xx, yy, DM_LENGTH)]
		.arr[0].id == id;
}

char tile_exists(m_map* dmbufmap, int xx, int yy)
{
	return dmbufmap->arr[1]
		.tile.arr[p2idx(xx, yy, DM_LENGTH)]
		.arr[0].id != -1;
}

//middle search, inside is shiny
char dm_place_msrch_0(m_map* dmbufmap, int x, int y)
{
	for (int iter = 0; iter < 2; iter++)
		for (int xx = x - (5 / 2); xx <= x + (5 / 2); xx++)
			for (int yy = y - (5 / 2); yy <= y + (5 / 2); yy++)
			{
				if (0 == iter)
				{
					if (tile_exists(dmbufmap, xx, yy) || tile_floor_exists(dmbufmap, xx, yy, HEAVDIRT))
						return 1;
				}
				else
				{
					comp_dm_cachei();

					s_sprite s;
					INIT_DMD_INC_SPRITE(s);

					s.id =
						xx <= x && xx >= x &&
						yy <= y && yy >= y
						? SHINY
						: (xx <= x + 1 && xx >= x - 1 &&
							yy <= y + 1 && yy >= y - 1
							? BWROCK
							: PYRBOT)
						;
					if (s.id == SHINY)
					{
						{
							comp_dm_cachee();

							s.flag = SPRITE_FLAG_CACHE;

							cac_cache c;
							c.cd = 0;
							c.cid = CACHE_DM_STD_CID;
							c.fnid = CACHE_FN_DM_FIND_ELEM;

							c.iid = dmcachetemplate[dmcachetempi].iid;
							c.spr = dmcachetemplate[dmcachetempi].spr;
							c.x = xx;
							c.y = yy;

							place_cache(dmbufmap, c);
						}
					}
					else
					{
						if (dmcacrand != 0 && xorrand() % dmcacrand)
						{
							s.flag = 2;

							bt_breaktile bt;
							bt.breakfnid = dmbreaktemplate[dmbreaktempi].breakfnid;
							bt.breakma = dmbreaktemplate[dmbreaktempi].breakma;
							bt.breakmb = dmbreaktemplate[dmbreaktempi].breakmb;

							place_break(dmbufmap, xx, yy, &bt);
						}
						else
						{
							s.flag = SPRITE_FLAG_CACHE;

							cac_cache c;
							c.cd = 0;
							c.cid = CACHE_DM_STD_CID;
							c.fnid = s.id == SHINY ? CACHE_FN_DM_FIND_ELEM : CACHE_FN_DM_STD_FIND;

							c.iid = dmcachetemplate[dmcachetempi].iid;
							c.spr = dmcachetemplate[dmcachetempi].spr;
							c.x = xx;
							c.y = yy;

							place_cache(dmbufmap, c);
						}
					}

					int sidx = p2idx(xx, yy, DM_LENGTH);
					dmbufmap->arr[1].tile.arr[sidx].size--;
					push_back_sprarr(&dmbufmap->arr[1].tile.arr[sidx], &s);
				}
			}

	return 0;
}

//middle search, bw rock inner, wh rock outer
char dm_place_msrch_1(m_map* dmbufmap, int x, int y)
{
	for (int iter = 0; iter < 2; iter++)
		for (int xx = x - (5 / 2); xx <= x + (5 / 2); xx++)
			for (int yy = y - (5 / 2); yy <= y + (5 / 2); yy++)
			{
				if (0 == iter)
				{
					if (tile_exists(dmbufmap, xx, yy) || tile_floor_exists(dmbufmap, xx, yy, HEAVDIRT))
						return 1;
				}
				else
				{
					comp_dm_cachei();

					s_sprite s;
					INIT_DMD_INC_SPRITE(s);
					s.id =
						xx <= x + 1 && xx >= x - 1 &&
						yy <= y + 1 && yy >= y - 1
						? BWROCK
						: WHROCK
						;

					if (dmcacrand != 0 && xorrand() % dmcacrand)
					{
						s.flag = 2;

						bt_breaktile bt;
						bt.breakfnid = dmbreaktemplate[dmbreaktempi].breakfnid;
						bt.breakma = dmbreaktemplate[dmbreaktempi].breakma;
						bt.breakmb = dmbreaktemplate[dmbreaktempi].breakmb;

						place_break(dmbufmap, xx, yy, &bt);
					}
					else
					{
						s.flag = 3;

						cac_cache c;
						c.cd = 0;
						c.cid = CACHE_DM_STD_CID;
						c.fnid = dmcachetemplate[dmcachetempi].fnid;
						c.iid = dmcachetemplate[dmcachetempi].iid;
						c.spr = dmcachetemplate[dmcachetempi].spr;
						c.x = xx;
						c.y = yy;

						place_cache(
							dmbufmap,
							c
						);
					}

					int sidx = p2idx(xx, yy, DM_LENGTH);
					dmbufmap->arr[1].tile.arr[sidx].size--;
					push_back_sprarr(&dmbufmap->arr[1].tile.arr[sidx], &s);
				}
			}

	return 0;
}

//edge search
char dm_place_esrch(m_map* dmbufmap, int x, int y, int w, char o, int l)
{
	int xmin = 0 + (x - (w / 2)) - 1, xmax = w + 2 + (x - (w / 2)) - 1;
	int ymin = y, ymax = y + 3;

	for (int iter = 0; iter < 2; iter++)
		for (int xx = xmin; xx < xmax; xx++)
			for (int yy = ymin; yy < ymax; yy++)
			{
				if (0 == iter)
				{
					if (!o)
					{
						if (tile_exists(dmbufmap, xx, yy) || tile_floor_exists(dmbufmap, xx, yy, HEAVDIRT))
							return 1;
					}
					else
					{
						if (tile_exists(dmbufmap, yy, xx) || tile_floor_exists(dmbufmap, yy, xx, HEAVDIRT))
							return 1;
					}
				}
				else
				{
					comp_dm_cachei();

					if (o)
					{
						xswap(xx, yy);
						xswap(xmin, ymin);
						xswap(xmax, ymax);
					}

					s_sprite s;
					INIT_DMD_INC_SPRITE(s);

					if (o)
					{
						xswap(xx, yy);
						xswap(xmin, ymin);
						xswap(xmax, ymax);
					}

					s.id =
						((xx != xmin && xx != xmax - 1)
							&& (yy >= ymax - l))
						? BWROCK
						: PYRTOP
						;

					if (s.id == BWROCK)
					{
						if (dmcacrand != 0 && xorrand() % dmcacrand)
						{
							s.flag = 2;

							bt_breaktile bt;
							bt.breakfnid = dmbreaktemplate[dmbreaktempi].breakfnid;
							bt.breakma = dmbreaktemplate[dmbreaktempi].breakma;
							bt.breakmb = dmbreaktemplate[dmbreaktempi].breakmb;

							place_break(dmbufmap, s.x, s.y, &bt);
						}
						else
						{
							s.flag = 3;

							cac_cache c;
							c.cd = 0;
							c.cid = CACHE_DM_STD_CID;
							c.fnid = dmcachetemplate[dmcachetempi].fnid;
							c.iid = dmcachetemplate[dmcachetempi].iid;
							c.spr = dmcachetemplate[dmcachetempi].spr;
							c.x = s.x;
							c.y = s.y;

							place_cache(
								dmbufmap,
								c
							);
						}
					}
					else s.flag = 1;

					int sidx = p2idx(s.x, s.y, DM_LENGTH);
					dmbufmap->arr[1].tile.arr[sidx].size--;
					push_back_sprarr(&dmbufmap->arr[1].tile.arr[sidx], &s);
				}
			}
	return 0;
}

//fixed length path, no adds
char dm_place_path_0(m_map* dmbufmap, int x, int y, char o)
{
	for (int iter = 0; iter < 2; iter++)
		for (int xx = x - !o * (5 / 2); xx <= x + !o * (5 / 2); xx++)
			for (int yy = y - o * (5 / 2); yy <= y + o * (5 / 2); yy++)
			{
				if (0 == iter)
				{
					if (tile_exists(dmbufmap, xx, yy) || tile_floor_exists(dmbufmap, xx, yy, HEAVDIRT))
						return 1;
				}
				else
				{
					s_sprite s;
					INIT_DMD_INC_SPRITE(s);

					s.id = HEAVDIRT;

					dmbufmap->arr[0].tile.arr[p2idx(xx, yy, DM_LENGTH)].arr[0] = s;
				}
			}

	return 0;
}

//variable length path, no adds
char dm_place_path_1(m_map* dmbufmap, int x, int y, char o, int l)
{
	int xmin = x - !o * (5 / 2);
	int xmax = x + !o * (5 / 2) + l * o;
	int ymin = y - o * (5 / 2);
	int ymax = y + o * (5 / 2) + l * !o;

	if (xmax < xmin) { xswap(xmax, xmin); }
	if (ymax < ymin) { xswap(ymax, ymin); }

	for (int iter = 0; iter < 2; iter++)
		for (int xx = xmin; xx <= xmax; xx++)
			for (int yy = ymin; yy <= ymax; yy++)
			{
				if (0 == iter)
				{
					if (tile_exists(dmbufmap, xx, yy) || tile_floor_exists(dmbufmap, xx, yy, HEAVDIRT))
						return 1;
				}
				else
				{
					s_sprite s;
					INIT_DMD_INC_SPRITE(s);

					s.id = HEAVDIRT;

					dmbufmap->arr[0].tile.arr[p2idx(xx, yy, DM_LENGTH)].arr[0] = s;
				}
			}
	return 0;
}

//variable length path, adds, moddable
char dm_place_path_2(m_map* dmbufmap, int x, int y, char o, int l, char m)
{
	int xmin = x - !o * (5 / 2), xmax = x + !o * (5 / 2) + l * o;
	int ymin = y - o * (5 / 2), ymax = y + o * (5 / 2) + l * !o;

	if (xmin > xmax) { xswap(xmin, xmax); }
	if (ymin > ymax) { xswap(ymin, ymax); }

	for (int iter = 0; iter < 2; iter++)
		for (int xx = xmin; xx <= xmax; xx++)
			for (int yy = ymin; yy <= ymax; yy++)
			{
				if (0 == iter)
				{
					if (tile_exists(dmbufmap, xx, yy) || tile_floor_exists(dmbufmap, xx, yy, HEAVDIRT))
						return 1;
				}
				else
				{
					s_sprite s;
					INIT_DMD_INC_SPRITE(s);

					s.id = HEAVDIRT;

					dmbufmap->arr[0].tile.arr[p2idx(xx, yy, DM_LENGTH)].arr[0] = s;
				}
			}

	for (int xx = xmin; xx <= xmax; xx += !o * (5 / 2) + 1 + !o)
		for (int yy = ymin; yy <= ymax; yy += o * (5 / 2) + 1 + o)
		{
			if (m && (xx % 2 ^ yy % 2)) continue;

			s_sprite s;
			INIT_DMD_INC_SPRITE(s);

			s.id = WHROCK;
			s.flag = 1;

			int sidx = p2idx(xx, yy, DM_LENGTH);
			dmbufmap->arr[1].tile.arr[sidx].size--;
			push_back_sprarr(&dmbufmap->arr[1].tile.arr[sidx], &s);
		}

	return 0;
}

//places breaks
char dm_place_break(m_map* dmbufmap, int x, int y, int w, int h, char m)
{
	for (int iter = 0; iter < 2; iter++)
		for (int xx = x - (w / 2); xx <= x + (w / 2); xx++)
			for (int yy = y - (h / 2); yy <= y + (h / 2); yy++)
			{
				if (0 == iter)
				{
					if (tile_exists(dmbufmap, xx, yy))
						return 1;
				}
				else
				{
					s_sprite s;
					INIT_DMD_INC_SPRITE(s);

					s.id = m && ((xx ^ yy) % 2) ? MINIROCK : BWROCK;
					s.flag = SPRITE_FLAG_BREAK;

					bt_breaktile bt;
					bt.breakfnid = dmbreaktemplate[dmbreaktempi].breakfnid;
					bt.breakma = dmbreaktemplate[dmbreaktempi].breakma;
					bt.breakmb = dmbreaktemplate[dmbreaktempi].breakmb;

					place_break(dmbufmap, xx, yy, &bt);

					int sidx = p2idx(xx, yy, DM_LENGTH);
					dmbufmap->arr[1].tile.arr[sidx].size--;
					push_back_sprarr(&dmbufmap->arr[1].tile.arr[sidx], &s);
				}
			}

	return 0;
}

//places a wall
char dm_place_wall(m_map* dmbufmap, int x, int y, char o, int l)
{
	for (int iter = 0; iter < 2; iter++)
		for (int xx = x; xx < x + l; xx++)
			for (int yy = y; yy < y + 2; yy++)
			{
				if (0 == iter)
				{
					if (!o)
					{
						if (tile_exists(dmbufmap, xx, yy) || tile_floor_exists(dmbufmap, xx, yy, HEAVDIRT))
							return 1;
					}
					else
					{
						if (tile_exists(dmbufmap, yy, xx) || tile_floor_exists(dmbufmap, yy, xx, HEAVDIRT))
							return 1;
					}
				}
				else
				{
					if (o) { xswap(xx, yy); }
					s_sprite s;
					INIT_DMD_INC_SPRITE(s);


					if (o) { xswap(xx, yy); }

					s.id = (yy != y)
						? WHROCK
						: PYRTOP
						;

					s.flag = 1;

					int sidx = p2idx(xx, yy, DM_LENGTH);
					dmbufmap->arr[1].tile.arr[sidx].size--;
					push_back_sprarr(&dmbufmap->arr[1].tile.arr[sidx], &s);
				}
			}

	return 0;
}

inline char dm_place_floor(m_map* dmbufmap)
{
	*dmbufmap = default_map(2, DM_LENGTH, DM_LENGTH, ROCKSOIL);
	return 0;
}

//5 mode
char dm_gen_std_wall(m_map* dmbufmap, char mode, int lo, int xo, int yo, char oo)
{
	char buf = 0;

	int walll = 0;
	int wallx = 0;
	int wally = 0;
	char wallo = 0;

	if (0 == mode)
	{
		//standard left wall
		walll = DM_LENGTH;
		wallx = 0;
		wally = 0;
		wallo = 1;

		buf |= dm_place_wall(dmbufmap, wallx + xo, wally + yo, wallo ^ !oo, walll + lo);
	}
	else if (1 == mode)
	{
		//standard right wall
		walll = DM_LENGTH;
		wallx = 0;
		wally = 0;
		wallo = 0;

		buf |= dm_place_wall(dmbufmap, wallx + xo, wally + yo, wallo ^ !oo, walll + lo);
	}
	else if (2 == mode)
	{
		//standard double left wall
		walll = DM_LENGTH / 2 - 2;
		wallx = 0;
		wally = 0;
		wallo = 1;
		buf |= dm_place_wall(dmbufmap, wallx + xo, wally + yo, wallo ^ !oo, walll + lo);

		walll = DM_LENGTH / 2 - 2;
		wallx = DM_LENGTH - walll;
		wally = 0;
		wallo = 1;
		buf |= dm_place_wall(dmbufmap, wallx + xo, wally + yo, wallo ^ !oo, walll + lo);
	}
	else if (3 == mode)
	{
		//standard double right wall
		walll = DM_LENGTH / 2 - 2;
		wallx = 0;
		wally = 0;
		wallo = 0;
		buf |= dm_place_wall(dmbufmap, wallx + xo, wally + yo, wallo ^ !oo, walll + lo);

		walll = DM_LENGTH / 2 - 2;
		wallx = DM_LENGTH - walll;
		wally = 0;
		wallo = 0;
		buf |= dm_place_wall(dmbufmap, wallx + xo, wally + yo, wallo ^ !oo, walll + lo);
	}

	else if (4 == mode)
	{
		//standard double right wall
		walll = DM_LENGTH / 2 - 2;
		wallx = 0;
		wally = 0;
		wallo = 0;
		buf |= dm_place_wall(dmbufmap, wallx + xo, wally + yo, wallo ^ !oo, walll + lo);

		walll = DM_LENGTH / 2 - 2;
		wallx = DM_LENGTH - walll;
		wally = 0;
		wallo = 0;
		buf |= dm_place_wall(dmbufmap, wallx + xo, wally + yo, wallo ^ !oo, walll + lo);
	}

	return buf;
}

//5 mode
char dm_gen_db_wall(m_map* dmbufmap, char mode, int xo)
{
	char buf = 0;

	buf |= dm_gen_std_wall(dmbufmap, mode
		, 0
		, 0
		, 0
		, 0
	);
	buf |= dm_gen_std_wall(dmbufmap, mode
		, 0
		, 0
		, xo
		, 0
	);

	return buf;
}

//12 mode
char dm_gen_std_path(m_map* dmbufmap, char mode)
{
	char buf = 0;

	int pathx;
	int pathy;
	char patho;

	int pathl;
	char pathm;

	if (0 == mode)
	{
		//standard path r0
		pathx = DM_LENGTH / 2;
		pathy = DM_LENGTH - 1;
		patho = 0;
		buf |= dm_place_path_0(dmbufmap, pathx, pathy, patho);
	}
	else if (1 == mode)
	{
		//standard path r1
		pathx = DM_LENGTH - 1;
		pathy = DM_LENGTH / 2;
		patho = 1;
		buf |= dm_place_path_0(dmbufmap, pathx, pathy, patho);
	}
	else if (2 == mode)
	{
		//standard path r2
		pathx = DM_LENGTH / 2;
		pathy = 0;// DM_LENGTH - 1;
		patho = 0;
		buf |= dm_place_path_0(dmbufmap, pathx, pathy, patho);
	}
	else if (3 == mode)
	{
		//standard path r3
		pathx = 0;// DM_LENGTH - 1;
		pathy = DM_LENGTH / 2;
		patho = 1;
		buf |= dm_place_path_0(dmbufmap, pathx, pathy, patho);
	}
	else if (4 == mode)
	{
		//standard path r0
		pathx = DM_LENGTH / 2;
		pathy = DM_LENGTH - 1;
		patho = 0;
		pathl = -DM_LENGTH / 4;

		buf |= dm_place_path_1(dmbufmap, pathx, pathy, patho, pathl);
	}
	else if (5 == mode)
	{
		//standard path r1
		pathx = DM_LENGTH - 1;
		pathy = DM_LENGTH / 2;
		patho = 1;
		pathl = -DM_LENGTH / 4;
		buf |= dm_place_path_1(dmbufmap, pathx, pathy, patho, pathl);
	}
	else if (6 == mode)
	{
		//standard path r2
		pathx = DM_LENGTH / 2;
		pathy = 0;// DM_LENGTH - 1;
		patho = 0;
		pathl = DM_LENGTH / 4;
		buf |= dm_place_path_1(dmbufmap, pathx, pathy, patho, pathl);
	}
	else if (7 == mode)
	{
		//standard path r3
		pathx = 0;// DM_LENGTH - 1;
		pathy = DM_LENGTH / 2;
		patho = 1;
		pathl = DM_LENGTH / 4;
		buf |= dm_place_path_1(dmbufmap, pathx, pathy, patho, pathl);
	}
	else if (mode >= 8)
	{
		if (mode >= 8 + 4) { mode -= 4; pathm = 0; }
		else pathm = 1;

		if (8 == mode)
		{
			//standard padded path r0
			pathx = DM_LENGTH / 2;
			pathy = DM_LENGTH - 1;
			pathl = -DM_LENGTH / 4;
			patho = 0;
			buf |= dm_place_path_2(dmbufmap, pathx, pathy, patho, pathl, pathm);
		}
		else if (9 == mode)
		{
			//standard padded path r1
			pathx = DM_LENGTH - 1;
			pathy = DM_LENGTH / 2;
			pathl = -DM_LENGTH / 4;
			patho = 1;
			buf |= dm_place_path_2(dmbufmap, pathx, pathy, patho, pathl, pathm);
		}
		else if (10 == mode)
		{
			//standard padded path r2
			pathx = DM_LENGTH / 2;
			pathy = 0;
			pathl = DM_LENGTH / 4;
			patho = 0;
			buf |= dm_place_path_2(dmbufmap, pathx, pathy, patho, pathl, pathm);
		}
		else if (11 == mode)
		{
			//standard padded path r3
			pathx = 0;
			pathy = DM_LENGTH / 2;
			pathl = DM_LENGTH / 4;
			patho = 1;
			buf |= dm_place_path_2(dmbufmap, pathx, pathy, patho, pathl, pathm);
		}
	}

	return buf;
}

char dm_gen_std_break(m_map* dmbufmap, char mode, char m)
{
	char buf = 0;

	int breakw;
	int breakh;

	if (0 == mode)
	{
		breakw = 3;
		breakh = 3;

		buf |= dm_place_break(dmbufmap,
			DM_LENGTH / 2,
			DM_LENGTH - 1 - breakh / 2 - 1,
			breakw,
			breakh,
			m
		);
	}
	else if (1 == mode)
	{
		breakw = 3;
		breakh = 3;

		buf |= dm_place_break(dmbufmap,
			DM_LENGTH - 1 - breakw / 2 - 1,
			DM_LENGTH / 2,
			breakw,
			breakh,
			m
		);
	}
	else if (2 == mode)
	{
		breakw = 3;
		breakh = 3;

		buf |= dm_place_break(dmbufmap,
			DM_LENGTH / 2,
			breakh / 2 + 1,
			breakw,
			breakh,
			m
		);
	}
	else if (3 == mode)
	{
		breakw = 3;
		breakh = 3;

		buf |= dm_place_break(dmbufmap,
			breakw / 2 + 1,
			DM_LENGTH / 2,
			breakw,
			breakh,
			m
		);
	}

	return buf;
}

//12 mode
char dm_gen_multi_path(m_map* dmbufmap, char mode, dm_dpath* r, dm_dpath* rr, char m)
{
	char buf = 0;

	if (r->p[0]) buf |= dm_gen_std_path(dmbufmap, mode + 0);
	if (r->p[1]) buf |= dm_gen_std_path(dmbufmap, mode + 1);
	if (r->p[2]) buf |= dm_gen_std_path(dmbufmap, mode + 2);
	if (r->p[3]) buf |= dm_gen_std_path(dmbufmap, mode + 3);

	if (mode < 4 * 2) return buf;

	if (rr->p[0]) buf |= dm_gen_std_break(dmbufmap, 0, m);
	if (rr->p[1]) buf |= dm_gen_std_break(dmbufmap, 1, m);
	if (rr->p[2]) buf |= dm_gen_std_break(dmbufmap, 2, m);
	if (rr->p[3]) buf |= dm_gen_std_break(dmbufmap, 3, m);

	return buf;
}

//2 mode
char dm_gen_msrch(m_map* dmbufmap, char mode)
{
	char buf = 0;

	//standard midsrch
	int msrchx;
	int msrchy;
	int msrchl;
	char msrcho;

	if (mode == 0)
	{
		msrchx = DM_LENGTH / 2;
		msrchy = DM_LENGTH / 2;
		msrchl = 8;
		msrcho = 0;
		buf |= dm_place_msrch_0(dmbufmap, msrchx, msrchy);
	}
	else if (mode == 1)
	{
		msrchx = DM_LENGTH / 2;
		msrchy = DM_LENGTH / 2;
		msrchl = 8;
		msrcho = 0;
		buf |= dm_place_msrch_1(dmbufmap, msrchx, msrchy);
	}

	return buf;
}

//8 mode
char dm_gen_esrch(m_map* dmbufmap, char mode, int esrchl)
{
	char buf = 0;

	int esrchx;
	int esrchy;
	int esrchw;
	//int esrchl;
	char esrcho;

	if (mode == 0)
	{
		//standard left edgesrch
		esrchx = DM_LENGTH / 2;
		esrchy = 0;
		esrchw = 4;
		//esrchl = 1;
		esrcho = 1;

		buf |= dm_place_esrch(dmbufmap, esrchx, esrchy, esrchw, esrcho, esrchl);
	}
	else if (1 == mode)
	{
		//standard right edgesrch
		esrchx = DM_LENGTH / 2;
		esrchy = 0;
		esrchw = 4;
		//esrchl = 1;
		esrcho = 0;

		buf |= dm_place_esrch(dmbufmap, esrchx, esrchy, esrchw, esrcho, esrchl);
	}
	else if (2 == mode)
	{
		//standard left double esrch
		esrchw = 3;
		esrchx = esrchw;
		esrchy = 0;
		//esrchl = 1;
		esrcho = 1;
		buf |= dm_place_esrch(dmbufmap, esrchx, esrchy, esrchw, esrcho, esrchl);

		esrchw = 3;
		esrchx = DM_LENGTH - esrchw - esrchw % 2;
		esrchy = 0;
		//esrchl = 1;
		esrcho = 1;
		buf |= dm_place_esrch(dmbufmap, esrchx, esrchy, esrchw, esrcho, esrchl);
	}
	else if (3 == mode)
	{
		//standard right double esrch
		esrcho = 0;

		esrchw = 3;
		esrchx = esrchw;
		esrchy = 0;
		//esrchl = 1;
		buf |= dm_place_esrch(dmbufmap, esrchx, esrchy, esrchw, esrcho, esrchl);

		esrchw = 3;
		esrchx = DM_LENGTH - esrchw - esrchw % 2;
		esrchy = 0;
		esrchl = 1;
		buf |= dm_place_esrch(dmbufmap, esrchx, esrchy, esrchw, esrcho, esrchl);
	}
	else if (4 == mode)
	{
		//standard edgesrch, done in middle left
		esrchx = DM_LENGTH / 2;
		esrchy = DM_LENGTH / 2;
		esrchw = 3;
		//esrchl = 1;
		esrcho = 1;

		buf |= dm_place_esrch(dmbufmap, esrchx, esrchy, esrchw, esrcho, esrchl);
	}
	else if (5 == mode)
	{
		//standard edgesrch, done in middle right
		esrchx = DM_LENGTH / 2;
		esrchy = DM_LENGTH / 2;
		esrchw = 3;
		//esrchl = 1;
		esrcho = 0;

		buf |= dm_place_esrch(dmbufmap, esrchx, esrchy, esrchw, esrcho, esrchl);
	}
	else if (6 == mode)
	{
		//standard double edgesrch, done in middle left
		esrcho = 1;

		esrchw = 3;
		esrchx = DM_LENGTH - (esrchw);
		esrchy = DM_LENGTH / 2;
		//esrchl = 1;
		buf |= dm_place_esrch(dmbufmap, esrchx, esrchy, esrchw, esrcho, esrchl);

		esrchw = 3;
		esrchx = 1 + esrchw / 2;
		esrchy = DM_LENGTH / 2;
		//esrchl = 1;
		buf |= dm_place_esrch(dmbufmap, esrchx, esrchy, esrchw, esrcho, esrchl);
	}
	else if (7 == mode)
	{
		//standard double edgesrch, done in middle right
		esrcho = 0;

		esrchw = 3;
		esrchx = DM_LENGTH - (esrchw);
		esrchy = DM_LENGTH / 2;
		//esrchl = 1;
		buf |= dm_place_esrch(dmbufmap, esrchx, esrchy, esrchw, esrcho, esrchl);

		esrchw = 3;
		esrchx = 1 + esrchw / 2;
		esrchy = DM_LENGTH / 2;
		//esrchl = 1;
		buf |= dm_place_esrch(dmbufmap, esrchx, esrchy, esrchw, esrcho, esrchl);
	}

	return buf;
}

void dm_place_end(m_map* dmbufmap)
{
	for (int y = -1; y <= 1; y++)
		for (int x = -1; x <= 1; x++)
		{
			dmbufmap->arr[0].tile
				.arr[p2idx(x + DM_LENGTH / 2, y + DM_LENGTH / 2, dmbufmap->arr[0].width)]
				.arr[0]
				.id = HEAVDIRT;

			dmbufmap->arr[0].tile
				.arr[p2idx(x + DM_LENGTH / 2, y + DM_LENGTH / 2, dmbufmap->arr[0].width)]
				.arr[0]
				.flag = SPRITE_FLAG_DMOUT;
		}
}

void dm_generate(m_map* dmbufmap, dm_dmapmd* dmmd, unsigned long seed)
{
	comp_dm_all();

	xrseed = seed;

	memset(dmmd->path.p, 0, sizeof(dmmd->path.p));
	for (int rra = dmmd->path.p[0] + dmmd->path.p[1] + dmmd->path.p[2] + dmmd->path.p[3]; rra < dmmd->pathl; )
	{
		dmmd->path.p[0] |= xorrand() % 2;
		dmmd->path.p[1] |= xorrand() % 2;
		dmmd->path.p[2] |= xorrand() % 2;
		dmmd->path.p[3] |= xorrand() % 2;

		rra = dmmd->path.p[0] + dmmd->path.p[1] + dmmd->path.p[2] + dmmd->path.p[3];
	}

	//floor
	dm_place_floor(dmbufmap);

	dmnpcrand = dmmd->npcrand;

	if (dmmd->endtick-- <= -1)
	{
		dm_place_end(dmbufmap);
		dmmd->endtick = -1;
	}
	//else
	{
		//paths
		{
			int r0 = xorrand() % 256;

			dmmd->path.p
				[(ch.rot + 2 +
					((((ch.rot + 1) % 2) * 2) - 1))
				% 4] = 1;

			//buf is path adds
			dm_dpath buf = (dm_dpath)
			{
				(char)(dmmd->path.p[0] & xorrand() % 2),
				(char)(dmmd->path.p[1] & xorrand() % 2),
				(char)(dmmd->path.p[2] & xorrand() % 2),
				(char)(dmmd->path.p[3] & xorrand() % 2),
			};

			dm_gen_multi_path(dmbufmap,
				(4 *
					(r0 <= 16
						? (0)
						: (r0 <= 32
							? (1)
							: (r0 <= 64
								? (2)
								: (3)
								)
							)
						)
					)
				, &dmmd->path
				, &buf
				, xorrand() % 2
			);
		}

		//msrch
		{
			dmcacrand = dmmd->cacrand;

			if (dmmd->smsrchm != 0 && 0 == xorrand() % dmmd->smsrchm)
			{
				dm_gen_msrch(dmbufmap, 0);
			}

			else if (dmmd->msrchm != 0 && 0 == xorrand() % dmmd->msrchm)
			{
				dmcacrand = dmmd->cacrand;

				dm_gen_msrch(dmbufmap, 1);
			}
		}

		//esrch
		{
			int r0 = xorrand() % 256;

			if (dmmd->esrchm != 0 && 0 == xorrand() % dmmd->esrchm)
			{
				int r0 = xorrand() % 64;

				dmcacrand = dmmd->cacrand / 2;
				for (int i = 0; dm_gen_esrch(dmbufmap, xorrand() % 8,
					(r0 <= 16)
					? (1)
					: ((r0 <= 64)
						? (2)
						: (0))
				););
			}
		}

		dm_gen_db_wall(dmbufmap, xorrand() % 5, xorrand() % 2 * 2);
	}
}