#include "npcfn.h"
#include "dglobal.h"
#include "arena.h"

char npc_give_item_0(n_npc* npc)
{
	static int swaptidx = -1;

	int cacidx = indexof_cache(cache, cache_top, 0);

	//set npc text id to null
	//set global text to n
	//keep incing til n gets to new num
	//<close npc event>

	readnsm(PATH_NSM, npc);

	if (-1 != cacidx)
	{
		t_aidx = 0;
		t_bidx = 0;

		if (npc->textidx != -1)
		{
			gtidx = 19;
			xswap(swaptidx, npc->textidx);
			screen_mode = SCREEN_MODE_TEXT;
		}
		else if (gtidx == 19)
			gtidx = 20;
		else if (gtidx == 20)
			gtidx = 21;
		else if (gtidx == 21)
			gtidx = 22;
		else
		{
			xswap(swaptidx, npc->textidx);
			screen_mode = SCREEN_MODE_NORMAL;

			//remove crate, give spirit
			rm_cache(
				cache,
				&cache_top,
				cacidx
			);

			ch.spra += 1 + SPR_BLOCK * 1;
			ch.sprb += 1 + SPR_BLOCK * 1;

			//give user spirit cast
			ch.defcast[1] = 1;
			//make the default cast spirit cast
		}
	}
	else
		screen_mode = SCREEN_MODE_NORMAL;

	return 0;


	if (npc->istate == 0)
	{
		npc->istate = 1;

		writensm(PATH_NSM, npc);

		npc->textidx = 19;

		return 0;
	}
	else if (npc->istate == 1)
	{
		npc->textidx = -1;

		gtidx = 0;

		npc->istate = 2;

		writensm(PATH_NSM, npc);

		return 0;
	}
	else if (npc->istate == 2)
	{
		gtidx = 1;

		npcidx = -1;
		screen_mode = SCREEN_MODE_NORMAL;

		return 0;
	}

	return 0;
}

char init_market(int midx)
{
	static char tflag = 1;
	if (tflag ^= 1) { screen_mode = SCREEN_MODE_NORMAL; return 0; }

	markitidx = midx;
	dctrl_zero_menu_select();
	screen_mode = SCREEN_MODE_DISP_MRKT;

	return 1;
}

extern void (*trs_end_fn)();
extern char rewflag;
extern int advidx;

char npc_enter_arena(n_npc* npc)
{
	advidx = npc - map.npca.arr;

	rewflag = npc->istate != 0;

	screen_mode = SCREEN_MODE_DISP_TRS;

	arena_temp_npc_idx = npc - map.npca.arr;

	trs_end_fn = &init_arena;

	for (int i = 0; i < cache_top; i++)
		if (cache[i].id == ARENA_TICKET_ID)
		{
			dec_rm_cache(&cache[i]);
			break;
		}

	return 0;
}

extern int arena_temp_idx;

char init_arena_npc(n_npc* npc, int idx)
{
	static char state = 0;

	t_aidx = 0;
	t_bidx = 0;

	gtidx++;

	int i;
	for (i = 0; i < cache_top; i++)
		if (cache[i].id == ARENA_TICKET_ID)
			break;

	if (state == 1)
	{
		advidx = -1;

		menu_sel_idx_2 = 0;
		arena_temp_idx = idx;
		trs_end_fn = &trs_ent_arena;
		screen_mode = SCREEN_MODE_MENU_CNFRM;
	}

	if (i >= cache_top)
	{
		screen_mode = SCREEN_MODE_NORMAL;
		state = 0;
		return 1;
	}
	else
	{
		gtidx = 47 - 10;
		state = 1;
	}

	return 1;
}

typedef struct 
{
	mst_mapstate* id;
	short val;
} mapstate_idx;

mapstate_idx mapstate_arenastate_table[] =
{
	{
		&pub_map_state[3],
		0,
	},
	{
		&pub_map_state[6],
		1,
	},
};

char npc_init_arena(n_npc* npc)
{
	char i;

	for (i = 0
		;
		!(0 == memcmp(mapstate_arenastate_table[i].id, &ms_mapstate, sizeof(mst_mapstate)))
		; i++);

	return init_arena_npc(npc, mapstate_arenastate_table[i].val);
}

void dispe_trade()
{
	npc_init_market_0(&map.npca.arr[0]);
}

char npc_init_market_0(n_npc* npc)
{
	static e_element buf;
	static icac_itemcache* find;

	static char npctidxstate;

	int idx = -1, idx_t;

	if (npc->istate == 0 ||
		npc->istate == 3)
	{
		npc->istate = 1;

		buf.spra = 0;
		buf.sprb = 0;
		buf.cast[1] = 1;
		buf.cast[2] = 0;
		buf.cast[3] = -1;

		if ((idx_t = indexof_cache(cache, cache_top, 9 + 0)) != -1)
		{
			idx = idx_t;
			buf.id = 24;
			buf.cast[0] = 2 + 1;
		}
		else if ((idx_t = (indexof_cache(cache, cache_top, 9 + 1))) != -1)
		{
			idx = idx_t;
			buf.id = 43;
			buf.cast[0] = 2 + 0;
		}
		else if ((idx_t = (indexof_cache(cache, cache_top, 9 + 2))) != -1)
		{
			idx = idx_t;
			buf.id = 41;
			buf.cast[0] = 2 + 2;
		}
		else npc->istate = 0;
		if (idx != -1)
			find = &cache[idx];
	}

#define TRADE_TEXT_IDX	33

	t_aidx = 0;
	t_bidx = 0;

	if (npc->istate == 2)
	{
		npc->textidx = npctidxstate;

		screen_mode = SCREEN_MODE_DISP_TRADE;

		trade_in = find;
		trade_out = &buf;

		menu_sel_idx_2 = 0;
		npc->istate = 3;
	}
	else if (npc->istate == 1)
	{
		static char state = 0;

		if (state == 0)
		{
			state = 1;

			gtidx = TRADE_TEXT_IDX;

			set_text_txt(
				&gtexta[TRADE_TEXT_IDX],
				&itexta[find->id],
				12,
				17,
				&sprdata[TEXT_DOT_IDX]
			);

			set_text_txt(
				&gtexta[TRADE_TEXT_IDX],
				&gtexta[42],
				12,
				17 + 12 + 16 + 1,
				&sprdata[TEXT_QMRK_IDX]
			);
		}
		else if (state == 1)
		{
			screen_mode = SCREEN_MODE_DISP_ELEM;
			disp_elem_idx = buf.id;
			npc->istate = 2;
			state = 0;
			npctidxstate = npc->textidx;
			npc->textidx = 32;
			disp_elem_close = &dispe_trade;
		}
	}

	else if (npc->istate == 0)
		return init_market(0);
}

extern int advidx;

char npc_cmp_arena(n_npc* npc)
{
	static char state[3] = { 0,0,0, };

	gtidx = 50 - 10;

	for (int i = 15; i < 15 + 16; i++)
		gtexta[gtidx]
		.arr[i]
		= &sprdata[TEXT_OFFSET];

	int id;

	t_aidx = 0;
	t_bidx = 0;

	if (rewflag)
		goto END_REW_COND;

	if (state[0] == 0 && arena_template[arena_temp_idx].arr[arena_temp_npc_idx].rew->rew_e.id != -1)
	{
		state[0] = 1;
		id = 52 - 10;

		add_elem(&ch, &arena_template[arena_temp_idx].arr[arena_temp_npc_idx].rew->rew_e);
		set_text_txt(&gtexta[gtidx], &gtexta[id], gtexta[id].size, 15, &sprdata[TEXT_DOT_IDX]);
	}
	else if (state[1] == 0 && (id = arena_template[arena_temp_idx].arr[arena_temp_npc_idx].rew->rew_ic.id) != -1)
	{
		state[1] = 1;

		icac_itemcache* buf = &arena_template[arena_temp_idx].arr[arena_temp_npc_idx].rew->rew_ic;

		if (buf->mul == -1)
		{
			buf->mul = 1;
			int i;
			for (i = 0; i < cache_top; i++)
				if (cache[i].id == buf->id)
					break;

			if (i >= cache_top)
			{
				add_cache(cache, &cache_top, ICACHE_MAX_SIZE, buf);
				set_text_txt(&gtexta[gtidx], &itexta[buf->id], itexta[buf->id].size, 15, &sprdata[TEXT_DOT_IDX]);
			}
			else
			{
				gtidx = 55;
			}

			buf->mul = -1;
		}
		else
		{
			add_cache(cache, &cache_top, ICACHE_MAX_SIZE, buf);
			set_text_txt(&gtexta[gtidx], &itexta[id], itexta[id].size, 15, &sprdata[TEXT_DOT_IDX]);
		}
	}
	else if (state[2] == 0 && arena_template[arena_temp_idx].arr[arena_temp_npc_idx].rew->rew_t != 0)
	{
		state[2] = 1;
		id = 53 - 10;
		ch.token += arena_template[arena_temp_idx].arr[arena_temp_npc_idx].rew->rew_t;
		set_text_txt(&gtexta[gtidx], &gtexta[id], gtexta[id].size, 15, &sprdata[TEXT_DOT_IDX]);
	}
	else
	{
	END_REW_COND:
		memset(state, 0, sizeof(state));

		if (!rewflag && arena_template[arena_temp_idx].arr[arena_temp_npc_idx].rew->rew_e.id != -1)
		{
			screen_mode = SCREEN_MODE_DISP_ELEM;
			disp_elem_idx = arena_template[arena_temp_idx].arr[arena_temp_npc_idx].rew->rew_e.id;
		}
		else
			screen_mode = SCREEN_MODE_NORMAL;

		npc->fnidx = 6;
		npc->textidx = 54 - 10;
	}

	return 0;
}

char npc_exit_arena(n_npc* npc)
{
	icac_itemcache buf;
	buf.id = ARENA_TICKET_ID;
	buf.mul = 2;
	buf.active = 0;
	add_cache(cache, &cache_top, ICACHE_MAX_SIZE, &buf);

	screen_mode = SCREEN_MODE_DISP_TRS;

	trs_end_fn = &trs_ent_arena;

	return 0;
}

char npc_gem_0(n_npc* npc)
{
	return 1;
}

char npc_random_cast(n_npc* npc)
{
	return 1;
}

#include "park.h"

void dec_rm_cache(icac_itemcache* c);

void trs_load_park()
{
	int i;
	for (i = 0; i < cache_top; i++)
		if (cache[i].id == PARK_TICKET_ID)
			break;

	dec_rm_cache(&cache[i]);

	m_mapseek buf;
	buf.seekin = park_seek;
	buf.load.ix = defr + park_ix;
	buf.load.iy = defr + park_iy;
	buf.load.ox = ch.dx - defr;
	buf.load.oy = ch.dy - defr;
	buf.load.seek = mapseek;

	push_back_mapseek(&loadstack, &buf);

	free_map(&map);
	map = readmapp(PATH_VAR_MAP, mapseek = park_seek);
	linkmap();

	push_back_mapload(&map.loads, &loadstack.arr[loadstack.size - 1].load);

	ch.sprite.x = ch.dx = defr + park_ix + 1;
	ch.sprite.y = ch.dy = defr + park_iy;

	park_elem_sel = -1;
	memset(park_cac_sel, -1, sizeof(park_cac_sel));

	npcai = -1;
	npcidx = -1;
}

char npc_init_park(n_npc* npc)
{
	static char state = 0;

	t_aidx = 0;
	t_bidx = 0;

	gtidx++;

#define PARK_TICKET_ID	14

	int i;
	for (i = 0; i < cache_top; i++)
		if (cache[i].id == PARK_TICKET_ID)
			break;

	if (state == 1)
	{
		menu_sel_idx_2 = 0;
		screen_mode = SCREEN_MODE_MENU_CNFRM;

		trs_end_fn = &trs_load_park;
	}

	if (i >= cache_top)
	{
		screen_mode = SCREEN_MODE_NORMAL;
		state = 0;
		return 1;
	}
	else
	{
		gtidx = 60 - 10;
		state = 1;
	}

	return 1;
}

/*
* ID		NAME
* 8			arena 1
*/

char (*npcfn[])(n_npc*) =
{
	&npc_give_item_0, &npc_init_market_0, &npc_gem_0, &npc_init_arena, &npc_enter_arena, &npc_cmp_arena, &npc_exit_arena, &npc_init_park,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};
