#include "breaktilefn.h"

#include "cachefn.h"
#include "usermap.h"

typedef struct
{
	unsigned int time;
	unsigned char state;
}bt_data_buffer;

#define BT_DATA_BUFFER_MAX_STATE	127

typedef struct
{
	unsigned char fnid;
	bt_data_buffer buf;
} bt_data_buffer_entry;

void bt_write_bdb(const char* file, bt_data_buffer_entry* bdb)
{
	FILE* f = fopen(file, "rb+");

	bt_data_buffer_entry buf;

	for (; fread(&buf, sizeof(bt_data_buffer_entry), 1, f);)
		if (buf.fnid == bdb->fnid)
		{
			fseek(f, -(int)(sizeof(bdb->buf)), SEEK_CUR);
			fwrite(&bdb->buf, sizeof(bdb->buf), 1, f);

			goto endloop;
		}
	fwrite(bdb, sizeof(bt_data_buffer_entry), 1, f);

endloop:
	fclose(f);
}

void bt_read_bdb(const char* file, unsigned char fnid, bt_data_buffer* buf)
{
	FILE* f = fopen(file, "rb");

	bt_data_buffer_entry buf0;

	for (; fread(&buf0, sizeof(bt_data_buffer_entry), 1, f);)
		if (buf0.fnid == fnid)
			*buf = buf0.buf;
}

void bdb_std_spr_increase(char id, float m, bt_data_buffer* bdb)
{
	if (0 == bdb->time)
		bt_read_bdb(PATH_BDB, id, bdb);

	int t1 = time(0);
	int t0 = bdb->time;
	bdb->time = t1;

	//dt bounds [0, ...]
	float dt = (t1 - t0) / ((float)(60 * 60));

	{
		float bdby = exp(1) * ((pow(exp(1), (pow(exp(1), (exp(1) * (dt - (1.0f / 2))))))) - exp(1));

		if (bdb->state + bdby < 0)
			bdb->state = 0;
		else if (bdb->state + bdby > BT_DATA_BUFFER_MAX_STATE)
			bdb->state = BT_DATA_BUFFER_MAX_STATE;
		else
			bdb->state = bdb->state + bdby;

		float mbuf = bdb->state / (float)BT_DATA_BUFFER_MAX_STATE;

		if (ch.elemidx == -1)
			return;

		unsigned short* sprb = &ch.elem[ch.elemidx].sprb;
		unsigned short* spra = &ch.elem[ch.elemidx].spra;

		float sprx = ((*sprb / (float)ELEM_SPR_BLOCK) / (exp(4.0f / 3))) + ((1 - mbuf) / (exp(1)));
		float spry = exp((-exp(2) * (sprx)));
		spry *= ELEM_SPR_BLOCK * m;

		unsigned short sprb0 = *sprb;

		*sprb += spry;
		*spra += spry;

		if ((int)(*sprb / (float)ELEM_SPR_BLOCK) - (int)(sprb0 / (float)ELEM_SPR_BLOCK))
		{
			screen_mode = SCREEN_MODE_TEXT;

			gtidx = 92 - 10;
			t_aidx = 0;
			t_bidx = 0;
			t_bptr_limit = 16;
		}
	}

	if (t1 - t0 >= 1)
	{
		bt_data_buffer_entry buf = { id, *bdb };
		bt_write_bdb(PATH_BDB, &buf);
	}
}

short cast_break_value;

void bt_tdrop(cac_cache* c)
{
	s_sprite* buf = &map.arr[1].tile
		.arr[p2idx(c->x, c->y, map.arr[1].width)]
		.arr[0];

#define SHINY_TILE_ID	706

	buf->id = SHINY_TILE_ID;
	buf->flag = SPRITE_FLAG_DROP;

	{
		for (int i = 0; i < bthmupdate.size; i++)
		{
			int x = bthmupdate.arr[i] % map.breaks.w;
			int y = bthmupdate.arr[i] / map.breaks.h;

			if (x == c->x && y == c->y)
			{
				remove_bthmupdatea(&bthmupdate, i);
				break;
			}
		}
	}

	push_back_cache(&map.cache, c);
}

char btfn0(bt_breaktile* t)
{
	t->breakma -= cast_break_value;
	return 0;
}

char empty_btfn(bt_breaktile* t)
{
	return 1;
}

char tfn_dm_init_npc(m_tilefn* t);
char btfn_set_em_npc(bt_breaktile* t)
{
	t->breakma -= cast_break_value;

	int x, y;
	int i;

	for (i = 0; i < map.tfn.size; i++)
	{
		get_ixiy(&x, &y, &ch);

		if (map.tfn.arr[i].ix == x && map.tfn.arr[i].iy == y)
			break;
	}

	ch.dx = x;
	ch.dy = y;

	m_tilefn* buf = &map.tfn.arr[i];

	map.arr[1].tile.arr
		[p2idx(x, y, map.arr[1].width)]
		.arr[0]
		.flag = SPRITE_FLAG_TFN;

	return tfn_dm_init_npc(buf);
}

#include "park.h"

char btfn_park_elem(bt_breaktile* t)
{
	t->breakma -= cast_break_value;

#define ELEM_PARK_MAX_STATE	8
	static unsigned short state = ELEM_PARK_MAX_STATE;

	if (state-- > 0)
	{
		screen_mode = SCREEN_MODE_DISP_ELEM;

		park_elem_i = 0;
		park_elem_l = 4;

		int r = (xorrand() % RARITY_FN0_MAX) / 1;

		e_element* e = &park_elem
			[park_elem_sel = rarity_fn_0(r, park_elem_i, park_elem_l)];

		disp_elem_idx = e->id;

		ch_action_pause();

		return 0;
	}

	else
	{
		state = ELEM_PARK_MAX_STATE;
		end_park();

		return 0;
	}
}

char btfn_park_cac(bt_breaktile* t)
{
	t->breakma -= cast_break_value;

#define CACHE_PARK_MAX_STATE	8
	static unsigned short state = CACHE_PARK_MAX_STATE;

	if (state-- > 0)
	{
		park_cac_i = 0;
		park_cac_l = 6;

		short r = (xorrand() % RARITY_FN0_MAX);

		icac_itemcache* e = &park_cac
			[r = rarity_fn_0(r, park_cac_i, park_cac_l)];

		int i;
		for (i = 0; i < cache_top; i++)
			if (cache[i].id == e->id
				&& e->mul >= 0
				&& e->mul >= cache[i].mul
				)
				return 1;

		set_gtext_itemfound(e->id);

		park_cac_sel
			[(CACHE_PARK_MAX_STATE - (state + 1)) % (sizeof(park_cac_sel) / sizeof(short))] = r;

		return 0;
	}

	else
	{
		state = CACHE_PARK_MAX_STATE;
		end_park();

		return 0;
	}
}

void init_tfn_rands(int x, int y);

char rfind(bt_breaktile* t)
{
	int i = t - map.breaks.arr;

	cac_cache buf;

	buf.x = i % map.breaks.w;
	buf.y = i / map.breaks.w;

	//if includes 0, the rng will often pattern return 0s. can offset by a constant random
	if (xorrand() % 255 <= ms_mapstate.crandevt)
	{
		rand_state = (xorrand() % RARITY_FN0_MAX);
		rand_state = rar_fn
			[ms_mapstate.crarfidx](
				rand_state
				, ms_mapstate.cidx
				, ms_mapstate.clen);

		{
			for (int i = 0
				; table_item_unique[i];
				i++)
				if (table_item_unique[i] == pub_cache[rand_state].cid)
					for (int i = 0; i < cache_top; i++)
						if (cache[i].id == pub_cache[rand_state].cid)
							return 2;
		}

		buf.fnid = 4;
		buf.iid = pub_cache[rand_state].cid;
		buf.spr = pub_cache[rand_state].spr;

		bt_tdrop(&buf);
		return 1;
	}
	else if (xorrand() % 255 <= ms_mapstate.erandevt)
	{
		//push r into the npc init call
		rand_state = (xorrand() % RARITY_FN0_MAX);
		unsigned short rr = rar_fn
			[ms_mapstate.erarfidx](
				rand_state
				, ms_mapstate.eidx
				, ms_mapstate.elen);

		e_element* e = &pub_elem[rr];

		int i;

		for (i = 0; ch.elem[i].id != -1 && ch.elem[i].id != e->id; i++);

		if (ch.elem[i].id == -1)
		{
			screen_mode = SCREEN_MODE_DISP_ELEM;
			disp_elem_idx = e->id;

			if (map.tfn.capacity <= 0)
				map.tfn = create_tilefna(1);

			init_tfn_rands(ch.dx, ch.dy);
			init_tfn_rands(buf.x, buf.y);

			buf.fnid = 9;
			buf.iid = rr;
			buf.spr = 0;

			bt_tdrop(&buf);
			return 1;
		}
	}

	return 0;
}

char btfn_rfind(bt_breaktile* t)
{
	t->breakma -= cast_break_value;
	if (t->breakma > 0) return 0;

	return rfind(t);
}

short cache_pass_id;

char cache_pass_fail(bt_breaktile* t)
{
	static bt_breaktile* btp = NULL;
	if (btp != t)
	{
		btp = t;

		screen_mode = SCREEN_MODE_TEXT;
		t_aidx = 0;
		t_bidx = 0;
		gtidx = 0;
	}

	return 1;
}
char cache_pass(bt_breaktile* t)
{
	for (int i = 0; i < cache_top; i++)
	{
		if (cache[i].id == cache_pass_id)
		{
			t->breakma = 0;
			return 0;
		}
	}

	return cache_pass_fail(t);
}

char cache_ash_pass(bt_breaktile* t)
{
	//cache_pass_id
	int i;
	for (i = 0 ;; i++)
		if (ashes_table[i] == cache_pass_id)
			break;

	for (int ii = i
		; ashes_table[ii];
		ii++)
		for (int i = 0; i < cache_top; i++)
			if (cache[i].id == ashes_table[ii])
			{
				t->breakma = 0;
				return 0;
			}

	return cache_pass_fail(t);
}

char btfn_ash_pass0(bt_breaktile* t)
{
	cache_pass_id = ASHES_ID_0;
	return cache_ash_pass(t);
}

char btfn_ash_pass1(bt_breaktile* t)
{
	cache_pass_id = ASHES_ID_1;
	return cache_ash_pass(t);
}

#include "dynamicmap.h"

char get_ar_ticket(bt_breaktile* t, char* tik, char rm)
{
	t->breakma -= cast_break_value;

	if (t->breakma <= 0)
	{
		char r = xorrand() % rm;

		if ((*tik != -1 && *tik-- == 0) || 0 == r)
		{
			icac_itemcache buf;
			buf.active = 0;
			buf.mul = 1;

			buf.id = -1;

			char i;
			for (i = 0; i < cache_top && cache[i].id != ARENA_TICKET_ID; i++);
			if (i >= cache_top)
				buf.id = ARENA_TICKET_ID;
			else if (0 == xorrand() % dm_dmmd->cacrand)
			{
				comp_dm_cachei();
				buf.id = dmcachetemplate[dmcachetempi].iid;
			}
			if (buf.id != -1)
			{
				add_cache(cache, &cache_top, sizeof(cache) / sizeof(*cache), &buf);
				set_gtext_itemfound(buf.id);
			}
			return 0;
		}
	}

	return 0;
}

char btfn_get_ar_ticket0(bt_breaktile* t)
{
	static char tik = 12;
	return get_ar_ticket(t, &tik, 12);
}

char btfn_get_ar_ticket1(bt_breaktile* t)
{
	static char tik = -1;
	return get_ar_ticket(t, &tik, 24);
}

float spr_add_fn0_mul = 2.125f;
float spr_add_fn0(float spr, float dim)
{
	return exp(-pow(
		(spr / dim)
		* spr_add_fn0_mul
		, 2));
}

int dim_spr_inflex = SPR_BLOCK * 2;

char btfn_rfind_dimspr0(bt_breaktile* t)
{
	t->breakma -= cast_break_value;
	if (t->breakma > 0) return 0;

	char s0 = ch.sprb / SPR_BLOCK;
	ch.sprb += SPR_BLOCK * spr_add_fn0(ch.sprb, dim_spr_inflex);

	if (s0 != (char)(ch.sprb / SPR_BLOCK))
	{
		ch.spra = ch.sprb;

		screen_mode = SCREEN_MODE_TEXT;

		t_aidx = 0;
		t_bidx = 0;
		t_bptr_limit = 16;
		gtidx = 22;
	}

	return rfind(t);
}

void push_um_refresha(bt_breaktile* t, int cd)
{
	um_tilerefresh buf;
	buf.cd = cd;

	buf.pos = t - map.breaks.arr;

	for (buf.id = 0; map.arr[1]
		.tile.arr[buf.pos]
		.arr[0]
		.id
		!=
		um_tile_id[buf.id];
		buf.id++);

	push_back_UMRefreshA(&um_refresha, &buf);
}

char btfn_um_fn0(bt_breaktile* t)
{
	t->breakma -= cast_break_value;
	if (t->breakma > 0) return 0;

	static bt_data_buffer bdb = { 0, BT_DATA_BUFFER_MAX_STATE, };
	bdb_std_spr_increase(11, 1.0f, &bdb);

	push_um_refresha(t, tik + 1024 * 8);

	return 0;
}

char btfn_um_fn1(bt_breaktile* t)
{
	t->breakma -= cast_break_value;
	if (t->breakma > 0) return 0;

	static bt_data_buffer bdb = { 0, BT_DATA_BUFFER_MAX_STATE, };
	bdb_std_spr_increase(12, 1.0f / 2, &bdb);

	push_um_refresha(t, tik + 1024 * 8);

	return 0;
}

/*	ID		BTFN
*	0		std break
*	6		break on ash0 exists
*	5		std random reward
*	4		std moon park cac
*	3		std moon park elem
*	7		chance of arena ticket 0
*	8		break on ash1 exists
*	9		random reward, log(spr) 0
*	10		chance of arena ticket 1
*	11		user map btfn 0
*	12		user map btfn 1
*/

char (*btfn[BREAK_TILE_FN_SIZE]) (bt_breaktile* t) =
{
	&btfn0, &empty_btfn, &btfn_set_em_npc, &btfn_park_elem, &btfn_park_cac, &btfn_rfind, &btfn_ash_pass0, &btfn_get_ar_ticket0,
	&btfn_ash_pass1, &btfn_rfind_dimspr0, &btfn_get_ar_ticket1, &btfn_um_fn0, &btfn_um_fn1, 0, //0, 0,
	//0,0,0,0,0,0,0,0,
};