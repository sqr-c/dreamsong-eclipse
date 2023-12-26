#pragma once

#include "itemcachefn.h"
#include "save.h"
#include "usermap.h"
#include "dglobal.h"

char (*itmcacfn[])(icac_itemcache* c) =
{
	&itmcacfn_refil_spr,
	&itmcacfn_refil_lif,
	&itmcacfn_set_cas,
	&itmcacfn_apply_gem,
	&itmcacfn_tport,
	&itmcacfn_apply_espr,
	&itmcacfn_apply_spr,
	&itmcacfn_apply_mat,
	&itmcacfn_apply_pkg,
};

typedef struct
{
	int id;
	int val;
} ID_VAL;

ID_VAL icid_val[] =
{
	{1, SPR_BLOCK * 1},
	{2, SPR_BLOCK * 2},
	{3, SPR_BLOCK * 4},
	{4, SPR_BLOCK * 8},

	{5, CH_MAX_LIFE / (4 + (1.0f / 4))},
	{6, CH_MAX_LIFE / 4},
	{7, CH_MAX_LIFE / 2},
	{8, CH_MAX_LIFE / 1},
};
#define ICID_VAL_LENGTH	(sizeof(icid_val) / sizeof(ID_VAL))

/*
1...4	spr
5...8	lif
21...38	mat
*/

ICID_ICIDFN icid_icidfn[] =
{
	{1, 0}, {2, 0}, {3, 0}, {4, 0},

	{5, 1}, {6, 1}, {7, 1}, {8, 1},

	{12, 2}, {15, 2}, {20, 2}, {22, 2},
	{23, 2}, {26, 2}, {54, 2},

	{16, 3},

	{18, 4},

	{19, 5}, {27, 5},

	{17, 6}, {28, 6},

	{31, 7, }, {32, 7, }, {33, 7, }, {34, 7, },
	{35, 7, }, {36, 7, }, {37, 7, }, {38, 7, },
	{39, 7, }, {40, 7, }, {41, 7, }, {42, 7, },
	{43, 7, }, {44, 7, }, {45, 7, }, {46, 7, },
	{47, 7, }, {48, 7, }, {56, 7, }, {57, 7, },
	{58, 7, }, {59, 7, }, {60, 7, }, {61, 7, },
	{62, 7, }, {63, 7, }, {64, 7, }, {65, 7, },
	{66, 7, },

	{49, 8, }, {50, 8, }, {51, 8, }, {52, 8, }, {53, 8, },
};
#define ICID_ICIDFN_LENGTH	(sizeof(icid_icidfn) / sizeof(ICID_ICIDFN))

ID_VAL_CHAR icid_mat[] =
{
	{31, 0, }, {32, 1, }, {33, 2, }, {34, 3, },
	{35, 4, }, {36, 5, }, {37, 6, }, {38, 7, },
	{39, 8, }, {40, 9, }, {41, 10, }, {42, 11, },
	{43, 12, }, {44, 13, }, {45, 14, }, {46, 15, },
	{47, 16, }, {48, 17, }, {56, 18, }, {57, 19, },
	{58, 20, }, {59, 21, }, {60, 22, }, {61, 23, },
	{62, 24, }, {63, 25, }, {64, 26, }, {65, 27, },
	{66, 28, },
};

void apply_ic(int i)
{
	for (int ii = 0; ii < ICID_ICIDFN_LENGTH; ii++)
		if (icid_icidfn[ii].id == cache[i].id)
		{
			itmcacfn[icid_icidfn[ii].idx](&cache[i]);
			break;
		}
}

void apply_all_ic()
{
	for (int i = 0; i < cache_top; i++)
		if (cache[i].active)
			apply_ic(i);
}

void set_clip_max(int* a, int b, int v)
{
	*a += v;
	if (*a > b) *a = b;
}

char std_refil(icac_itemcache* c, int* a, int b)
{
	int i;
	for (i = 0; i < ICID_VAL_LENGTH; i++)
		if (icid_val[i].id == c->id)
			break;

	set_clip_max(a, b, icid_val[i].val);
	dec_rm_cache(c);

	return i >= ICID_VAL_LENGTH;
}

char itmcacfn_refil_spr(icac_itemcache* c)
{
	return std_refil(c, &ch.spra, ch.sprb);
}

char itmcacfn_refil_lif(icac_itemcache* c)
{
	return std_refil(c, &ch.lifea, ch.lifeb);
}

ID_VAL_CHAR icid_castid[] =
{
	{ 12, 7, },
	{ 20, 11, },
	{ 15, 12, },
	{ 22, 6, },
	{ 23, 25, },
	{ 26, 46, },
	{ 54, 69, },
};
#define ICID_CASTID_LENGTH (sizeof(icid_castid) / sizeof(ID_VAL_CHAR))

ID_VAL_CHAR cast_whitelist[] =
{
	//CASID:	2
	{-1,2},{1,2},{5,2},{15,2},{16,2},{19,2},{21,2},{22,2},{23,2},{26,2},{32,2},{34,2},{35,2},{36,2},{37,2},{38,2},{43,2},{44,2},{45,2},{48,2},
	//CASID:	3
	{-1,3},{1,3},{5,3},{15,3},{16,3},{19,3},{21,3},{22,3},{23,3},{26,3},{32,3},{34,3},{35,3},{36,3},{37,3},{38,3},{43,3},{44,3},{45,3},{48,3},
	//CASID:	4
	{-1,4},{1,4},{5,4},{15,4},{16,4},{19,4},{21,4},{22,4},{23,4},{26,4},{32,4},{34,4},{35,4},{36,4},{37,4},{38,4},{43,4},{44,4},{45,4},{48,4},
	//CASID:	7
	{2,7},{3,7},{6,7},{7,7},{8,7},{9,7},{10,7},{11,7},{12,7},{13,7},{14,7},{24,7},{25,7},{27,7},{28,7},{29,7},{30,7},{31,7},{33,7},{39,7},{40,7},{41,7},{42,7},{46,7},{47,7},{49,7},
	//CASID:	11
	{1,11},{5,11},{15,11},{16,11},{19,11},{21,11},{22,11},{23,11},{26,11},{32,11},{34,11},{35,11},{36,11},{37,11},{38,11},{43,11},{44,11},{45,11},{48,11},
	//CASID:	12
	{-1,12,},
	//CASID:	6
	{-1,6,},
	//CASID:	25
	{-1,25,},
	//CASID:	46
	{-1,46},{4,46},{18,46},{20,46},
	//CASID:	69
	{-1,69},
};

ID_VAL gem_rng[] =
{
	{16, GEM_RNG_UNIT * 1.1875f},
};

ID_VAL cid_espr[] =
{
	{19, ELEM_SPR_BLOCK, },
	{27, ELEM_SPR_BLOCK / 4, },
};

float cid_espr_decay[] =
{
	0,
	0.486275f,
};

#define CAST_WHITELIST_LENGTH ( (sizeof(cast_whitelist)) / (sizeof(ID_VAL_CHAR)) )

char apply_casid;
icac_itemcache* itemcache_rm;

int srch_whtlst(int id)
{
	int i;
	for (i = 0; i < CAST_WHITELIST_LENGTH; i++)
		if (cast_whitelist[i].id == id
			&& cast_whitelist[i].val == apply_casid)
			break;
	return i;
}

char apply_setcac()
{
	short* castfield;

	castfield =
		(menu_sel_idx_1 == -1)
		? ch.defcast
		: ch.elem[menu_sel_idx_1].cast
		;

	if (srch_whtlst(
		castfield == ch.defcast
		? -1
		: ch.elem[menu_sel_idx_1].id)
		>= CAST_WHITELIST_LENGTH)
	{
		gtidx = 45 - 10;
		goto endfn;
	}

	castfield[menu_sel_idx_2] = apply_casid;
	int i;
	for (i = 0; i < CAST_WHITELIST_LENGTH; i++)
		if (cast_whitelist[i].id == -1
			&& cast_whitelist[i].val == apply_casid
			)
			break;
	if (i >= CAST_WHITELIST_LENGTH)
	{

	}

	gtidx = 44 - 10;

	dec_rm_cache(itemcache_rm);

endfn:
	t_aidx = 0;
	t_bidx = 0;

	renderfn = &render;

	screen_mode = SCREEN_MODE_TEXT;

	menu_sel_idx_0 = -1;
	menu_sel_idx_1 = -1;
	menu_sel_idx_2 = -1;

	return 1;
}

char itmcacfn_set_cas(icac_itemcache* c)
{
	npcai = -1;

	renderfn = &render_elem_cache;

	ls_ptr = 0;
	screen_mode = SCREEN_MODE_APPLY_CAS;

	for (int i = 0; i < ICID_CASTID_LENGTH; i++)
		if (icid_castid[i].id == c->id)
		{
			apply_casid = icid_castid[i].val;
			itemcache_rm = c;
			break;
		}

	return 1;
}

char itmcacfn_apply_gem(icac_itemcache* c)
{
	int i;
	for (i = 0;; i++)
		if (gem_rng[i].id == c->id)
			break;

	if (rar_fn0_adj == gem_rng[i].val)
		return 1;

	rar_fn0_adj = gem_rng[i].val;

	if (xorrand() % 256 < 32)
		dec_rm_cache(c);

	return 0;
}

char stdcast(c_cast* c, int a);

char itmcacfn_tport(icac_itemcache* c)
{
	static int cd = 0;
	static char state = 0;

	if (state > 8) return 1;
	else if (cd > tik) return 1;
	else if (xorrand() % 256 <= 8) dec_rm_cache(c);
	else { state++; cd = tik + 1000 * 8; }

	int d = 8;

	int dx = ROTDIR((ch.rot + 1) % 4);
	int dy = ROTDIR((ch.rot) % 4);

	int i;
	for (i = 0; i < d; i++)
	{
		int ddx = ch.dx + dx * i;
		int ddy = ch.dy + dy * i;

		int idx = p2idx(ddx, ddy, map.arr[1].width);

		if (map.arr[1].tile.arr
			[idx].arr[0].flag != 0
			|| map.arr[0].tile.arr
			[idx].arr[0].id == -1

			|| (ddx < 0 || ddy < 0
				|| ddx >= map.arr[0].width || ddy >= map.arr[0].height))
			goto endloop;

		for (int i = 0; i < map.npca.size; i++)
			if (map.npca.arr[i].ch.sprite.x == ddx && map.npca.arr[i].ch.sprite.y == ddy)
				goto endloop;
	}
endloop:
	i--;

	ch.dx = ch.sprite.x += dx * i;
	ch.dy = ch.sprite.y += dy * i;

	push_back_IterFNA(&cyfn, &rdr_flsh_scr);

	return 0;
}

char itmcacfn_apply_espr(icac_itemcache* c)
{
	int i;
	for (i = 0; cid_espr[i].id != c->id; i++);

	float sb = ch.elem[ch.elemidx].sprb;

	float m = pow(exp(1), -(2 + cid_espr_decay[i]) * exp(1) * pow((sb / ELEM_SPR_MAX), 2));

	int buf =
		m * cid_espr[i].val
		;

	if ((unsigned short)(ch.elem[ch.elemidx]
		.sprb + buf) < (unsigned short)(ch.elem[ch.elemidx].sprb))
		buf = ELEM_SPR_MAX;
	else
		buf = ch.elem[ch.elemidx].sprb + buf;

	ch.elem[ch.elemidx]
		.spra = ch.elem[ch.elemidx]
		.sprb = buf;

	dec_rm_cache(c);

	return 0;
}

ID_VAL cid_spr[] =
{
	{ 17, SPR_BLOCK, },
	{ 28, SPR_BLOCK / 8, },
};

float cid_spr_decay[] =
{
	0,
	0,
};

char itmcacfn_apply_spr(icac_itemcache* c)
{
	int i;
	for (i = 0; cid_spr[i].id != c->id; i++);

	float x =
		(float)(ch.sprb / SPR_BLOCK)
		/
		(float)(SPR_MAX / SPR_BLOCK);

	float m = pow(exp(1), -(2 + cid_spr_decay[i]) * exp(1) * pow(3 * x, 2));

	unsigned int buf =
		m *
		cid_spr[i].val
		;

	if ((unsigned int)(ch.sprb + buf) > SPR_MAX)
		buf = SPR_MAX;
	else
		buf = ch.sprb + buf;

	ch.spra = ch.sprb = buf;

	dec_rm_cache(c);

	return 0;
}

char itmcacfn_apply_mat(icac_itemcache* c)
{
	if (statusr)
	{
		screen_mode = SCREEN_MODE_TEXT;
		gtidx = 91 - 10;
		t_aidx = 0;
		t_bidx = 0;

		return 1;
	}

	int i;
	for (i = 0;
		(icid_mat[i].id != c->id)
		; i++);

	um_edit_id = i;

	screen_mode = SCREEN_MODE_UM_EDIT_ADD;

	return 0;
}

#include "package.h"

char itmcacfn_apply_pkg(icac_itemcache* c)
{
	int tableidx;
	for (tableidx = 0; pkg_table[tableidx].id != c->id; tableidx++);

	{
		char flag = 1;

		char swp[(sizeof(PATH_PID) - 1) + (sizeof(".swp") - 1) + 1];
		sprintf(swp, "%s%s", PATH_PID, ".swp");

		FILE* fp = fopen(PATH_PID, "rb");
		FILE* fpswp = fopen(swp, "wb");

		pkg_pid_entry buf;
		for (; fread(&buf, sizeof(pkg_pid_entry), 1, fp);)
		{
			if (flag && buf.id == c->id)
			{
				xrseed = buf.seed;
				flag = 0;
			}
			else
				fwrite(&buf, sizeof(pkg_pid_entry), 1, fpswp);
		}

		fclose(fp);
		fclose(fpswp);

		{
			remove(PATH_PID);
			rename(swp, PATH_PID);
		}
	}

	if (pkg_table[tableidx].icache_arr)
	{
		unsigned char cacnum = pkg_table[tableidx].icache_sel;

		for (; cacnum--;)
		{
			int idx = rarity_fn_pkg(xorrand(),
				pkg_table[tableidx].icache_arr - pkg_itemcache,
				pkg_table[tableidx].icache_len
			);

			icac_itemcache buf;
			buf.id = pkg_itemcache[idx].id;
			buf.mul = pkg_itemcache[idx].mul;
			buf.active = 0;

			add_cache(cache, &cache_top, ICACHE_MAX_SIZE, &buf);
		}

		dec_rm_cache(c);

		screen_mode = SCREEN_MODE_TEXT;
		gtidx = 89 - 10;
		t_aidx = 0;
		t_bidx = 0;
	}
	//TODO: implement for element

	return 0;
}