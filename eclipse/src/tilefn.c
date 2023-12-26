#include "tilefn.h"

#include <string.h>

char tfn_ch_state_flag;

void rm_tfn(m_tilefn_a* tfna, int idx)
{
	int w = map.arr[1].width;

	map.arr[1]
		.tile.arr[p2idx(tfna->arr[idx].ix, tfna->arr[idx].iy, w)]
		.arr[0]
		.flag = 0;

	tfna->arr[idx] = tfna->arr[tfna->size - 1];
	tfna->size--;
}

#include "dynamicmap.h"
#include "dmapgen.h"

char tfn_dm_init_ennpc(m_tilefn* t)
{
	//break tile can => tfn
	{
		n_npc buf;
		init_en_npc(&buf, dmnpctemplate[dmnpctempi]);

		set_npc_end_state_dm(&buf, t->ix, t->iy, -1);
		push_back_NPCA(&map.npca, &buf);
	}

	rm_tfn(&map.tfn, t - map.tfn.arr);

	map.arr[1].tile.arr
		[p2idx(t->ix, t->iy, map.arr[1].width)]
		.arr[0].id = -1;

	{
		int i;
		for (i = 0; i < cyfn.size; i++)
			if (cyfn.arr[i] == &en_npc_update)
				break;
		if (i >= cyfn.size)
			push_back_IterFNA(&cyfn,
				&en_npc_update
			);
	}

	npcai = map.npca.size - 1;

	return 1;
}

void move_render(ch_char* c)
{
	for (c->state = CH_STATE_WALK; c->state == CH_STATE_WALK;)
	{
		ch_move(c, CH_MOVE_VELOCITY_L);
		render_end();
	}
}

char tfn_park_loadcheck0(m_tilefn* t)
{
	for (int i = 0; i < map.npca.size; i++)
		//todo: use ix iy instead of the first npc. bug with generator not getting these
	{
		n_npc* buf = &map.npca.arr[i];

		if (buf->ch.sprite.x == ch.dx && buf->ch.sprite.y == ch.dy)
		{
			ch.dx = ddx;
			ch.dy = ddy;

			if (ch.state == CH_STATE_IDLE || ch.state == CH_STATE_ACTION)
				ch_set_state(&ch, dr, CH_STATE_IDLE);

			return 1;
		}

		if (ch.sprb < SPR_BLOCK * 4)
		{
			float dx = ch.sprite.x - ch.dx;

			move_render(&ch);

			SDL_Delay(1000);

			ch.dx += dx * 1;
			//ch.dy += dy * 1;

			move_render(&ch);

			buf->ch.dy = ch.sprite.y;

			move_render(&buf->ch);

			SDL_Delay(1000);

			buf->ch.rot = ch.rot;
			buf->ch.state = CH_STATE_WALK;
			ch_set_state(&buf->ch, buf->ch.rot, CH_STATE_IDLE);

			t_aidx = 0;
			t_bidx = 0;
			gtidx = 48;
			screen_mode = SCREEN_MODE_TEXT;
		}
	}

	((Uint8*)keys)[KEY_CH_WALK_XL] = 0;
	((Uint8*)keys)[KEY_CH_WALK_XR] = 0;
	((Uint8*)keys)[KEY_CH_WALK_YL] = 0;
	((Uint8*)keys)[KEY_CH_WALK_YR] = 0;

	return 0;
}

#include "park.h"

char tfn_park_exit(m_tilefn* t)
{
	end_park();
	return 0;
}

char tfnrandsflg[] = { -1, -1, -1, -1, -1, -1, };

void init_tfn_rands(int x, int y)
{
	char flgat;
	if (tfnrandsflg[0] == -1)
		flgat = 0;
	else flgat = 3;

	for (int i = 0; i < 3; i++)
	{
		m_tilefn buf;
		buf.fnid = 3;

		char r0 = 0, r1 = 0;
		for (; !(r0 || r1);)
		{
			r0 = (xorrand() % 3) - 1;
			r1 = (xorrand() % 3) - 1;

			for (int i = 0; (x + r0 == ch.dx && y + r1 == ch.dy); i++)
			{
				r0 = (i + x) % 3;
				r1 = (i + y) % 3;
			}
		}

		buf.ix = x + r0;
		buf.iy = y + r1;

		int idx = p2idx(buf.ix, buf.iy, map.arr[1].width);

		s_sprite_a* at = &map.arr[1].
			tile.arr[idx];

		if (at->size <= 0)
			*at = create_sprarr(1);

		s_sprite* s = &at->arr[0];

		tfnrandsflg[flgat + i] =
			(s->flag == SPRITE_FLAG_TFN ? -2 : s->flag);
		s->flag = SPRITE_FLAG_TFN;

		push_back_tilefna(&map.tfn, &buf);
	}
}

//possible bug in this fn
char tfn_ennpc_init(m_tilefn* t)
{
	unsigned short rr = rar_fn
		[ms_mapstate.nrarfidx](
			rand_state
			, ms_mapstate.nidx
			, ms_mapstate.nlen);

	push_en_npc_ex(pub_ennpc[rr], t->ix, t->iy, 1);

	{
		for (int i = 0; i < 6; i++)
		{
			m_tilefn* at = &map.tfn.arr
				[map.tfn.size - 1];

			int idx = p2idx(at->ix, at->iy, map.arr[1].width);

			if (tfnrandsflg[6 - 1 - i] != -2)
			{
				s_sprite_a* at = &map.arr[1].tile.arr[idx];

				at->arr[0].flag = tfnrandsflg[6 - 1 - i];

				if (at->arr[0].flag == 4 || at->arr[0].flag < 0)
					at->arr[0].flag = 0;
			}

			map.tfn.size--;
		}

		memset(tfnrandsflg, -1, sizeof(tfnrandsflg));

		{
			map.arr[1].
				tile.arr[p2idx(ch.dx, ch.dy, map.arr[1].width)]
				.arr[0]
				.flag = SPRITE_FLAG_NONE;

			map.arr[1].
				tile.arr[p2idx(ch.dx, ch.dy, map.arr[1].width)]
				.arr[0]
				.id = -1;
		}
	}

	return 0;
}

icac_itemcache tfn_block_inv_cac;

char templ_tfn_block_inv(m_tilefn* t)
{
	for (int i = 0; i < cache_top; i++)
		if (cache[i].id == tfn_block_inv_cac.id
			&& cache[i].mul >= tfn_block_inv_cac.mul)
		{
			cache[i].mul -= tfn_block_inv_cac.mul;
			if (cache[i].mul <= 0) rm_cache(cache, &cache_top, &cache[i] - cache);

			for (int i = 0; i < map.tfn.size; )
				if (map.tfn.arr[i].fnid == t->fnid)
				{
					s_sprite* s = &map.arr[1]
						.tile.arr[p2idx(
							map.tfn.arr[i].ix,
							map.tfn.arr[i].iy,
							map.arr[1].width)]
						.arr[0];

					s->flag = SPRITE_FLAG_NONE;
					s->id = -1;

					map.tfn.arr[i] = map.tfn.arr[map.tfn.size - 1];
					map.tfn.size--;
					i = 0;
				}
				else i++;

			return 0;
		}

	ch.dx = ch.sprite.x;
	ch.dy = ch.sprite.y;

	return 1;
}

char tfn_block_inv_0(m_tilefn* t)
{
	tfn_block_inv_cac.id = ICAC_GEM_LILAC_ID;
	tfn_block_inv_cac.mul = 8;

	char buf = templ_tfn_block_inv(t);

	dr = ch.rot = 0;

	return buf;
}

char tfn_rand_disp_cache(m_tilefn* t)
{
	s_sprite* ss;

	for (int iii = 0; iii < map.cache.size; iii++)
	{
		cac_cache* c = &map.cache.arr[iii];

		for (int ii = 0; ii < 2; ii++)
		{
			int idx0 = p2idx(c->x, c->y, map.arr[1].width);
			s_sprite* s = &map.arr[1].tile.arr[idx0].arr[0];

			char dy = ((xorrand() % 2) * 2 - 1) * ((ii + 1) % 2);
			char dx = ((xorrand() % 2) * 2 - 1) * ((ii + 0) % 2);

			int i;
			for (i = 0;; i++)
				if (map.arr[1].tile.arr[p2idx(c->x + (i + 1) * dx, c->y + (i + 1) * dy, map.arr[1].width)].arr[0]
					.id
					!= s->id)
					break;

			if (i == 0) continue;

			i = (xorrand() % abs(i)) % i;

			c->x += dx * i;
			c->y += dy * i;

			int idx1 = p2idx(c->x, c->y, map.arr[1].width);
			ss = &map.arr[1].tile.arr[idx1].arr[0];

			if (s != ss)
			{
				if (ss->flag == SPRITE_FLAG_BREAK)
				{
					bt_breaktile t = map.breaks.arr[idx0];

					map.breaks.arr[idx0] = map.breaks.arr[idx1];
					map.breaks.arr[idx1] = t;
				}
				xswap(s->flag, ss->flag);
			}
		}
	}

	return 0;
}

char ash_ennpc(char ashmin, char ashmax, n_npc_min* arr, int x, int y)
{
	char a = -1, b = -1;
	char c = -1;

	char idx = 0;

	for (int i = 0
		; (a == -1 || b == -1);
		i++)
		if (ashmin == ashes_table[i]) a = i;
		else if (ashmax == ashes_table[i]) b = i;

	if (cache_top != 0)
	{
		for (c = b; c >= a; c--)
			for (int ii = 0; ii < cache_top; ii++)
				if (cache[ii].id == ashes_table[c])
				{
					idx = c - a;
					goto endloop;
				}
	}

endloop:
	if (c == b)
	{
		return 1;
	}
	else
	{
		push_en_npc_ex(
			&arr[idx]
			, x
			, y
			, -1);

		return 0;
	}
}

char ash_ennpc_rmtfn(m_tilefn* t, char ashmin, char ashmax, n_npc_min* arr, int x, int y)
{
	ash_ennpc(ashmin, ashmax, arr, x, y);
	rm_tfn(&map.tfn, t - map.tfn.arr);
	return 0;
}

char tfn_ash_ennpc0(m_tilefn* t)
{
	return ash_ennpc_rmtfn(t, ASHES_ID_0, ASHES_ID_1, pub_ennpc[1], t->ix, t->iy);
}

char tfn_ash_ennpc1(m_tilefn* t)
{
	return ash_ennpc_rmtfn(t, ASHES_ID_1, ASHES_ID_2, pub_ennpc[1], t->ix, t->iy);
}

char tfn_debug(m_tilefn* t)
{
	return 0;
}

char tfn_ch_state_dive(m_tilefn* t)
{
	int i, ii;

	for (i = 0; i < sizeof(ch.defcast) / sizeof(*ch.defcast); i++)
		if (ch.defcast[i] == CAST_DIVE_ID)
			goto exit_found;

	for (i = 0; -1 != ch.elem[i].id; i++)
		for (ii = 0; ii < sizeof(ch.elem->cast) / sizeof(*ch.elem->cast); ii++)
			if (ch.elem->cast[ii] == CAST_DIVE_ID)
				goto exit_found;

	for (i = 0; i < cache_top; i++)
		if (cache[i].id == ICAC_C_DIVE_ID)
			goto exit_found;

	tfn_ch_state_flag = 0;
	return 1;

exit_found:
	tfn_ch_state_flag = 1;
	return 0;
}

char tfn_cstate_dive_mstate(m_tilefn* t)
{
	tile_set_map_state();
	tfn_ch_state_dive(t);

	rm_tfn(&map.tfn, t - map.tfn.arr);

	return 1;
}

/*
* ID	TILEFN
* 4		block, inventory required
* 5		random disp cache
* 6		ash npc 0
* 7		debug
* 8		ash npc 1
* 9		exists(dive) => tfn state changed
* 10	dive check && set mapstate
*/
char (*tilefn[])(m_tilefn* t) =
{
	&tfn_dm_init_ennpc, &tfn_park_loadcheck0, &tfn_park_exit, &tfn_ennpc_init, &tfn_block_inv_0, &tfn_rand_disp_cache, &tfn_ash_ennpc0, &tfn_debug,
	&tfn_ash_ennpc1, &tfn_ch_state_dive, &tfn_cstate_dive_mstate,
};