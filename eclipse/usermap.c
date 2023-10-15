#include "usermap.h"

#include <stdlib.h>
#include <string.h>

#include "dglobal.h"
#include "save.h"
#include "mathfunction.h"

unsigned short um_tile_id[] =
{
	719, 720, 721, 722,
	723, 724, 725, 726,
	727, 728, 729, 730,
	731, 732, 733, 734,
	735, 736,
};

char um_tile_flag[] =
{
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 0,
	2, 1, 2, 1,
	1, 1,
};

unsigned char usermap_bt_id[] =
{
	12, 14,
};

bt_breaktile_min usermap_bt_struct[] =
{
	{1 * BREAKM_BLOCK, 11,},
	{4 * BREAKM_BLOCK, 12,},
};

unsigned char um_edit_x = 0, um_edit_y = 0, um_edit_z = 0;
unsigned char um_edit_id = 0;

unsigned short um_pre_ch_x, um_pre_ch_y;

um_tilerefresh_a um_refresha;

um_tilerefresh_a create_UMRefreshA(int capacity)
{
	um_tilerefresh_a buf;
	buf.arr = (um_tilerefresh*)malloc(capacity * sizeof(um_tilerefresh));
	buf.size = 0;
	buf.capacity = capacity;
	return buf;
}

void free_UMRefreshA(um_tilerefresh_a* array)
{
	free(array->arr);
}

void resize_array_UMRefreshA(um_tilerefresh_a* array, int new_capacity)
{
	um_tilerefresh* new_arr = (um_tilerefresh*)malloc(new_capacity * sizeof(um_tilerefresh));

	memcpy(new_arr, array->arr, sizeof(*array->arr) * array->size);

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_UMRefreshA(um_tilerefresh_a* array, um_tilerefresh* value)
{
	if (array->size == array->capacity)
		resize_array_UMRefreshA(array, array->capacity * 2);
	array->arr[array->size] = *value;
	array->size++;
}

void keybd_std_delay()
{
	SDL_ResetKeyboard();
	SDL_PumpEvents();
	keys = SDL_GetKeyboardState(NULL);
	SDL_RenderClear(renderer);
	renderfn();
	SDL_RenderPresent(renderer);
	SDL_Delay(16);
}

char um_newmap_init_texts()
{
	int i;
	for (i = 0; i < UM_MAX_MAP && um_name_text[UM_MAX_MAP_NAME_TEXT_SIZE * i]; i++);

	if (i >= UM_MAX_MAP)
		return 1;

	um_map_index = i;

	gtexta[UM_NEWMAP_NAME_IDX].size = 0;

	return 0;
}

void um_blink_tile()
{
	s_sprite_a* sa = &map.arr[1]
		.tile.arr[tsoa.arr[0].pos];

	int* id = &sa->arr[sa->size - 1].id;

	if ((*id < 0) ^ ((tik / 512) % 2))
		*id *= -1;
}

void um_map_save()
{
	//string here
	char file[] = "XXXX.smd";
	fnv1a_hash_str(&um_name_text[UM_MAX_MAP_NAME_TEXT_SIZE * um_map_index], 4, file);

	clear_TScreenObjA(&tsoa);

	for (int i = 0; i < um_refresha.size; i++)
	{
		s_sprite_a* sa = &map.arr[1]
			.tile.arr[um_refresha.arr[i].pos];

		sa->arr[sa->size - 1].id
			= um_tile_id[um_refresha.arr[i].id];
		sa->arr[sa->size - 1].flag
			= um_tile_flag[um_refresha.arr[i].id];

		map.breaks.arr[um_refresha.arr[i].pos].breakma = map.breaks.arr[um_refresha.arr[i].pos].breakmb;
	}

	if (mapfp != NULL)
	{
		fclose(mapfp);
		mapfp = NULL;
	}

	writemap(&map, file);

	for (int i = 0; i < um_refresha.size; i++)
	{
		s_sprite_a* sa = &map.arr[1]
			.tile.arr[um_refresha.arr[i].pos];

		sa->arr[sa->size - 1].id
			= -1;
		sa->arr[sa->size - 1].flag
			= -1;
	}
}

void um_map_remove()
{
	//string here
	char file[] = "XXXX.smd";
	fnv1a_hash_str(&um_name_text[um_map_index * UM_MAX_MAP_NAME_TEXT_SIZE], 4, file);

	remove(file);
	for (;
		um_map_index + 1 < UM_MAX_MAP
		&& um_name_text[(um_map_index + 1) * UM_MAX_MAP_NAME_TEXT_SIZE]
		; um_map_index++)
	{
		char buf[UM_MAX_MAP_NAME_TEXT_SIZE];
		strcpy(buf, &um_name_text[um_map_index * UM_MAX_MAP_NAME_TEXT_SIZE]);

		strcpy(&(um_name_text[um_map_index * UM_MAX_MAP_NAME_TEXT_SIZE]), &(um_name_text[(um_map_index + 1) * UM_MAX_MAP_NAME_TEXT_SIZE]));
		strcpy(&um_name_text[(um_map_index + 1) * UM_MAX_MAP_NAME_TEXT_SIZE], buf);
	}
	memset(&um_name_text[um_map_index * UM_MAX_MAP_NAME_TEXT_SIZE], 0, UM_MAX_MAP_NAME_TEXT_SIZE);
}

void um_map_load()
{
	free_map(&map);

	//string here
	char file[] = "XXXX.smd";
	fnv1a_hash_str(&um_name_text[UM_MAX_MAP_NAME_TEXT_SIZE * um_map_index], 4, file);

	fclose(mapfp);
	mapfp = NULL;

	map = readmapp(file, 0);

	ddx = ch.dx = ch.sprite.x = map.arr[1].width / 2;
	ddy = ch.dy = ch.sprite.y = map.arr[1].height / 2;
}

void um_map_exit()
{
	bthmupdate.size = 0;

	free_map(&map);

	if (mapfp != NULL)
	{
		fclose(mapfp);
		mapfp = NULL;
	}

	map = readmapp(PATH_VAR_MAP, mapseek);

	linkmap();

	ch.sprite.x = ch.dx = um_pre_ch_x;
	ch.sprite.y = ch.dy = um_pre_ch_y;

	screen_mode = SCREEN_MODE_NORMAL;
}

void um_map_in_init()
{
	statusr = 0;
	select_none_fn = &select_mapctrl_fn;
	push_back_IterFNA(&cyfn, &itfn_umrefresh_update);
	um_pre_ch_x = ch.dx;
	um_pre_ch_y = ch.dy;
}

void um_map_out_init()
{
	statusr = 1;
	select_none_fn = NULL;
	rmf_IterFNA(&cyfn, &itfn_umrefresh_update);
}

void um_map_new(unsigned char w, unsigned char h)
{
	free_map(&map);

	map = default_map(2, w, h, MAT_FLOOR_BLANK_ID);

	ch.dx = ch.sprite.x = w / 2;
	ch.dy = ch.sprite.y = h / 2;
}

void um_add_mat()
{
	s_sprite s;
	s.id = um_tile_id[um_edit_id];
	s.flag = um_tile_flag[um_edit_id];
	s.x = um_edit_x;
	s.y = um_edit_y;
	s.z = um_edit_z * UM_Z_DELTA * (s.flag == SPRITE_FLAG_BLOCK);

	s_sprite_a* sa = &map.arr[(s.flag != SPRITE_FLAG_NONE)]
		.tile.arr[p2idx(s.x, s.y, map.arr[1].width)];

	if (s.flag == SPRITE_FLAG_NONE)
		if (sa->arr[0].id == MAT_FLOOR_BLANK_ID)
			sa->arr[0] = s;
		else
			goto place_mat_fail;
	else if (map.arr[0]
		.tile.arr[p2idx(s.x, s.y, map.arr[1].width)]
		.arr[0]
		.id != MAT_FLOOR_BLANK_ID)
	{
		if (s.flag == SPRITE_FLAG_BREAK)
		{
			if (sa->arr[0].id != -1)
				goto place_mat_fail;

			sa->arr[0] = s;

			int i;
			for (i = 0; usermap_bt_id[i] != um_edit_id; i++);

			map.breaks.arr[p2idx(um_edit_x, um_edit_y, map.arr[0].width)]
				.breakfnid = usermap_bt_struct[i].fnid;
			map.breaks.arr[p2idx(um_edit_x, um_edit_y, map.arr[0].width)]
				.breakma = usermap_bt_struct[i].breakm;
			map.breaks.arr[p2idx(um_edit_x, um_edit_y, map.arr[0].width)]
				.breakmb = usermap_bt_struct[i].breakm;
		}
		else
		{
			int i;
			for (i = 0; i < sa->size; i++)
				if (sa->arr[i].id == -1)
					break;
				else if (sa->arr[i].flag == 2)
					goto place_mat_fail;

			if (i < sa->size)
				sa->arr[i] = s;
			else if (i < UM_STACK_MAX)
				push_back_sprarr(sa, &s);
			else
			{
				goto place_mat_fail;
			}
		}
	}
	else
	{
	place_mat_fail:
		screen_mode = SCREEN_MODE_TEXT;

		t_aidx = 0;
		t_bidx = 0;
		gtidx = 62;

		return;
	}

	{
		int id;
		for (id = 0; icid_mat[id].val != um_edit_id; id++);

		int i;
		for (i = 0; cache[i].id != icid_mat[id].id; i++);

		dec_rm_cache(&cache[i]);
	}
}

void um_map_add_cac(s_sprite* s)
{
	int i;

	for (i = 0
		; um_tile_id[icid_mat[i].val] != s->id
		; i++)
		;

	icac_itemcache buf;
	buf.id = icid_mat[i].id;
	buf.mul = 1;
	buf.active = 0;

	add_cache(cache, &cache_top, ICACHE_MAX_SIZE, &buf);
}

void um_rm_all_mat()
{
	um_edit_z = 0;
	for (um_edit_y = 0; um_edit_y < map.arr[1].height; um_edit_y++)
		for (um_edit_x = 0; um_edit_x < map.arr[1].width; um_edit_x++)
			um_rm_stack_mat();
}

void um_rm_stack_mat()
{
	s_sprite* s = &map.arr[0].tile.arr[p2idx(um_edit_x, um_edit_y, map.arr[0].width)].arr[0];

	if (s->id != MAT_FLOOR_BLANK_ID)
	{
		s_sprite_a* sa = &map.arr[1].tile.arr[p2idx(um_edit_x, um_edit_y, map.arr[1].width)];

		for (um_edit_z = 0
			; sa->arr[sa->size - 1].id != -1
			; um_rm_mat())
			;
		sa->arr[sa->size - 1].flag = SPRITE_FLAG_NONE;

		um_map_add_cac(s);
		s->id = MAT_FLOOR_BLANK_ID;
	}
}

void um_rm_mat()
{
	s_sprite_a* sa = &map.arr[1].tile.arr[p2idx(um_edit_x, um_edit_y, map.arr[1].width)];

	for (int i = um_edit_z; i + 1 < sa->size; i++)
	{
		s_sprite t = sa->arr[i];
		sa->arr[i] = sa->arr[i + 1];
		sa->arr[i + 1] = t;
	}

	sa->size--;
	um_map_add_cac(&sa->arr[sa->size]);

	{
		if (sa->size == 0)
		{
			sa->arr[sa->size].id = -1;
			sa->arr[sa->size].flag = 0;
			sa->size++;
		}
	}
}

void um_edit_rm_mat(char dx, char dy, char dz)
{
	s_sprite_a* sa = &map.arr[1].tile.arr[p2idx(um_edit_x, um_edit_y, map.arr[1].width)];

	if (tsoa.size > 0)
	{
		sa->arr[um_edit_z]
			.id = abs(tsoa.arr[0].v.id);
		clear_TScreenObjA(&tsoa);
	}

	um_edit_z = sa->size == 0 ? 0 : pm(um_edit_z + dz, sa->size);
	um_edit_x = pm(um_edit_x + dx, map.arr[1].width);
	um_edit_y = pm(um_edit_y + dy, map.arr[1].height);

	sa = &map.arr[1].tile.arr[p2idx(um_edit_x, um_edit_y, map.arr[1].width)];

	{
		int x, y;

		for (x = um_edit_x, y = um_edit_y
			;  map.arr[1].tile.arr[
				p2idx(x, y, map.arr[1].width)
			].arr[0].id < 0
					;)
		{
			x = pm(x + dx, map.arr[1].width); y = pm(y + dy, map.arr[1].height);
			if (x % map.arr[1].width == um_edit_x % map.arr[1].width && y % map.arr[1].height == um_edit_y % map.arr[1].height)
				return;
		}

		um_edit_x = x;
		um_edit_y = y;

		sa = &map.arr[1].tile.arr[p2idx(x, y, map.arr[1].width)];
	}

	{
		{
			tsc_tscreenobj buf;

			buf.pos = p2idx(um_edit_x, um_edit_y, map.arr[1].width);
			buf.v = sa->arr[um_edit_z];

			buf.cd = 0;

			push_back_TScreenObj(&tsoa, &buf);
			apply_TScreenObj(&buf);
		}

		{
			int i;
			for (i = 0; i < cyfn.size; i++)
				if (cyfn.arr[i] == &um_blink_tile)
					break;
			if (i >= cyfn.size)
				push_back_IterFNA(&cyfn, &um_blink_tile);
		}
	}

	{
		sa->arr[um_edit_z]
			.id = -1;
	}

	keybd_std_delay();
}

void um_edit_add_position(char dx, char dy, char dz)
{
	um_edit_x = pm(um_edit_x + dx, map.arr[1].width);
	um_edit_y = pm(um_edit_y + dy, map.arr[1].height);
	um_edit_z += dz;

	clear_TScreenObjA(&tsoa);

	tsc_tscreenobj buf;

	buf.v.x = um_edit_x;
	buf.v.y = um_edit_y;

	buf.pos = p2idx(um_edit_x, um_edit_y, map.arr[1].width);

	buf.v.z = um_edit_z * UM_Z_DELTA;
	buf.v.id = um_tile_id[um_edit_id];
	buf.cd = 0;

	push_back_TScreenObj(&tsoa, &buf);
	apply_TScreenObj(&buf);

	int i;
	for (i = 0; i < cyfn.size; i++)
		if (cyfn.arr[i] == &um_blink_tile)
			break;
	if (i >= cyfn.size)
		push_back_IterFNA(&cyfn, &um_blink_tile);

	keybd_std_delay();
}

void select_mapctrl_fn()
{
	menu_sel_idx_0 = pm(menu_sel_idx_0, 4);
	screen_mode = SCREEN_MODE_UM_EDIT_MENU;
}