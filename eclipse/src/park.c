#pragma once

#include "park.h"

e_element park_elem[] =
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
	{
		45,
		0, 0,
		{17, 18, -1, -1, },
	},
	{
		29,
		0, 0,
		{19, 20, -1, -1, },
	},
};

icac_itemcache park_cac[] =
{
	//{15, 1},
	//{16, 1},
	//{18, 1},
	//{17, 1},
	//{19, 1},
	//{20, 1},

	{20, -1},
	{19, 1},
	{17, 1},
	{18, -1},
	{16, 1},
	{15, -1},
};

short park_elem_i;
char park_elem_l;
short park_elem_sel;

short park_cac_i;
char park_cac_l;
short park_cac_sel[4];

//MAP SEEK:		.\translation\wm3\0000.smd		moon park
int32_t park_seek = 4871068;
short park_ix = 1;
short park_iy = 11;

#include "dglobal.h"

void render_end();
extern void (*trs_end_fn)();

void end_park()
{
	screen_mode = SCREEN_MODE_TEXT;
	t_aidx = 0;
	t_bidx = 0;
	gtidx = 51;

	if (park_elem_sel != -1)
	{
		add_elem(&ch, &park_elem[park_elem_sel]);
		park_elem_sel = -1;
	}

	for (char i = 0; i < PARK_CAC_SEL_LEN && park_cac_sel[i] != -1; i++)
	{
		short buf = park_cac[park_cac_sel[i]].mul;
		int buf0 = park_cac[park_cac_sel[i]].id;
		park_cac[park_cac_sel[i]].mul = abs(buf);

		int top = cache_top;

		if (buf == -1)
			for (top = 0; top < cache_top; top++)
				if (cache[top].id == buf0)
					break;

		if (top >= cache_top)
			add_cache(cache, &cache_top, ICACHE_MAX_SIZE, &park_cac[park_cac_sel[i]]);

		park_cac[park_cac_sel[i]].mul = buf;
	}

	{
		SDL_Event e;
		e.type = !SDL_KEYDOWN;
		SDL_ResetKeyboard();
		for (;
			56 != t_bidx
			||
			e.type != SDL_KEYDOWN; SDL_PollEvent(&e))
			render_end();
	}

	map.tfn.size = 0;

	screen_mode = SCREEN_MODE_DISP_TRS;

	trs_end_fn = &trs_exit_park;
}

void trs_exit_park()
{
	m_mapseek* buf =
		&loadstack.arr[--loadstack.size];

	mapseek = buf->load.seek;

	free_map(&map);
	map = readmapp(PATH_VAR_MAP, mapseek);
	linkmap();
	push_back_mapload(&map.loads, &loadstack.arr[loadstack.size - 1].load);

	ch.sprite.x = ch.dx = buf->load.ox + defr;
	ch.sprite.y = ch.dy = buf->load.oy + defr;
}