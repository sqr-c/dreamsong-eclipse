#pragma once

#include "map.h"
#include "path.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "build.h"
#include SDL_SDL_INCLUDE

FILE* mapfp = NULL;

const unsigned short defr = MAP_WIDTH;

extern int tik;

void update_npc(n_npc* npc, m_maplayer* l)
{
	short mov = (tik / (int)(32 * 256));

	if ((mov != npc->move_seed))
	{
		npc->move_seed = mov;

		unsigned char r = xorrand() % 2;
		unsigned char mmod = xorrand() % 2;

		int d = (&(npc->ch.dx))[mmod];
		(&(npc->ch.dx))[mmod] = (-1 + (2 * r)) + (int)((&(npc->ch.sprite.x))[mmod]);

		if (abs((&(npc->ix))[mmod] - (&(npc->ch.dx))[mmod]) > npc->walk_d
			|| collision_map_char(&(npc->ch))
			|| collision_char_char(&(npc->ch), &ch))
			(&(npc->ch.dx))[mmod] = d;
	}
}

void writecid(const char* path, cac_cache* c)
{
	FILE* fp;
	fp = fopen(path, "rb+");

	int buf[2] = { c->cid,c->cd };
	int rbuf[2];

	while (!feof(fp))
	{
		fread(rbuf, sizeof(rbuf), 1, fp);
		if (rbuf[0] == c->cid)
		{
			int32_t seek = ftell(fp);
			fclose(fp);
			fp = fopen(path, "rb+");
			fseek(fp, seek - sizeof(rbuf), SEEK_SET);
			fwrite(buf, sizeof(buf), 1, fp);
			fclose(fp);
			return;
		}
	}

	fclose(fp);

	fp = fopen(path, "ab");

	fwrite(buf, sizeof(buf), 1, fp);

	fclose(fp);
}

void readcid(const char* path, cac_cache* c)
{
	FILE* fp;

	fp = fopen(path, "rb");

	int buf[2] = { 0,0 };

	while (!feof(fp))
	{
		fread(buf, sizeof(buf), 1, fp);
		if (buf[0] == c->cid)
		{
			c->cd = buf[1];
			break;
		}
	}

	fclose(fp);
}

void readallcid(const char* path, cac_cache_a* c)
{
	FILE* fp;

	fp = fopen(path, "rb");

	int buf[2] = { 0, 0, };

	for (int i = 0; !feof(fp);)
	{
		fread(buf, sizeof(buf), 1, fp);

		for (int ii = i; ii < c->size; ii++)
			if (buf[0] == c->arr[ii].cid)
				c->arr[ii].cd = buf[1];
	}

	fclose(fp);
}

cac_cache create_cache(int x, int y, int spr, int iid, int fnid, int cid)
{
	return (cac_cache) { x, y, spr, iid, 0, cid, fnid };
}

cac_cache_a create_cachea(int capacity)
{
	cac_cache_a buf;
	buf.arr = (cac_cache*)malloc(capacity * sizeof(cac_cache));
	buf.size = 0;
	buf.capacity = capacity;
	return buf;
}

void free_cache(cac_cache_a* array)
{
	free(array->arr);
}

void resize_array_cache(cac_cache_a* array, int new_capacity)
{
	cac_cache* new_arr = (cac_cache*)malloc(new_capacity * sizeof(cac_cache));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_cache(cac_cache_a* array, cac_cache* value)
{
	if (array->size == array->capacity)
		resize_array_cache(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}

m_maplayer default_map_layer(int w, int h, int fill, int ox, int oy)
{
	s_sprite_aa r = create_spraa(w * h);

	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
		{
			s_sprite sbuf = (s_sprite){ (float)x, (float)y, 0.0f, fill, 0, 0 };

			s_sprite_a buf = create_sprarr(1);

			push_back_sprarr(&buf, &sbuf);
			push_back_spraa(&r, &buf);
		}

	return (m_maplayer)
	{
		w, h, r, ox, oy,
	};
}

m_maplayer create_map_layer(int width, int height)
{
	m_maplayer buf;
	buf.ox = 0;
	buf.oy = 0;
	buf.width = width;
	buf.height = height;
	buf.tile = create_spraa(width * height);
	return buf;
}

point2 idxp2(int i, int w)
{
	return (point2) { i% w, i / w };
}

int p2idx(int x, int y, int w)
{
	return x + y * w;
}

void free_map_layer(m_maplayer* map)
{
	free_spraa(&(map->tile));
}

m_tilefn_a create_tilefna(int capacity)
{
	m_tilefn_a buf;
	buf.arr = (m_tilefn*)malloc(capacity * sizeof(m_tilefn));
	buf.size = 0;
	buf.capacity = capacity;
	return buf;
}

void free_tilefna(m_tilefn_a* array)
{
	free(array->arr);
}

void resize_array_tilefna(m_tilefn_a* array, int new_capacity)
{
	m_tilefn* new_arr = (m_tilefn*)malloc(new_capacity * sizeof(m_tilefn));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_tilefna(m_tilefn_a* array, m_tilefn* value)
{
	if (array->size == array->capacity)
		resize_array_tilefna(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}

m_map create_map(int capacity)
{
	m_map buf;
	buf.arr = (m_maplayer*)malloc(capacity * sizeof(m_maplayer));
	buf.size = 0;
	buf.capacity = capacity;
	return buf;
}

void free_map(m_map* array)
{
	free(array->breaks.arr);
	free(array->tfn.arr);
	free(array->npca.arr);
	free(array->loads.arr);
	free(array->cache.arr);
	free(array->texts.arr);

	for (int i = 0; i < array->size; i++)
		for (int ii = 0; ii < array->arr[i].tile.size; ii++)
			free(array->arr[i].tile.arr[ii].arr);
	for (int i = 0; i < array->size; i++)
		free(array->arr[i].tile.arr);

	free(array->arr);
}

char empty_map(m_map* array)
{
	return array->size == 0;
}

int get_size_map(m_map* array)
{
	return array->size;
}

int get_capacity_map(m_map* array)
{
	return array->capacity;
}

void resize_array_map(m_map* array, int new_capacity)
{
	m_maplayer* new_arr = (m_maplayer*)malloc(new_capacity * sizeof(m_maplayer));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_map(m_map* array, m_maplayer* value)
{
	if (array->size == array->capacity)
		resize_array_map(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}

inline void initmap(m_map* m)
{
	for (int i = 0; i < m->cache.size; i++)
	{
		cac_cache* at = &(m->cache.arr[i]);

		if (at->cd > time(NULL))
			for (int ii = 1; ii < m->size; ii++)
			{
				int ox = m->arr[ii].ox;
				int oy = m->arr[ii].oy;

				s_sprite_a* ata = &(m->arr[ii].tile.arr[
					p2idx(at->x - ox, at->y - oy, m->arr[i].width)
				]);

				for (int ii = 0; ii < ata->size; ii++)
				{
					ata->arr[ii].id = -1;
					ata->arr[ii].flag = 0;
				}
			}
	}
}

m_map readmapp(const char* path, int32_t offset)
{
	if (mapfp == NULL)
		mapfp = fopen(path, "r");

	fseek(mapfp, offset, SEEK_SET);

	m_map mapbuf;

	//layer num
	fread(&(mapbuf.size), sizeof(int), 1, mapfp);
	mapbuf.capacity = mapbuf.size;
	mapbuf.arr = (m_maplayer*)malloc(mapbuf.capacity * sizeof(m_maplayer));

	//each layer
	for (int i = 0; i < mapbuf.size; i++)
	{
		//layer wid, hei, ox, oy
		fread(&(mapbuf.arr[i].width), sizeof(int), 1, mapfp);
		fread(&(mapbuf.arr[i].height), sizeof(int), 1, mapfp);
		fread(&(mapbuf.arr[i].ox), sizeof(int), 1, mapfp);
		fread(&(mapbuf.arr[i].oy), sizeof(int), 1, mapfp);

		//layer num tiles
		fread(&(mapbuf.arr[i].tile.size), sizeof(int), 1, mapfp);
		mapbuf.arr[i].tile.capacity = mapbuf.arr[i].tile.size;
		mapbuf.arr[i].tile.arr = (s_sprite_a*)malloc(mapbuf.arr[i].tile.capacity * sizeof(s_sprite_a));

		//each tile
		for (int ii = 0; ii < mapbuf.arr[i].tile.size; ii++)
		{
			//number of stacks
			fread(&(mapbuf.arr[i].tile.arr[ii].size), sizeof(int), 1, mapfp);
			//stack[]
			mapbuf.arr[i].tile.arr[ii].capacity = mapbuf.arr[i].tile.arr[ii].size;
			mapbuf.arr[i].tile.arr[ii].arr = (s_sprite*)malloc(mapbuf.arr[i].tile.capacity * sizeof(s_sprite));
			fread(mapbuf.arr[i].tile.arr[ii].arr, sizeof(s_sprite), mapbuf.arr[i].tile.arr[ii].size, mapfp);
		}
	}

	//num cache
	fread(&(mapbuf.cache.size), sizeof(int), 1, mapfp);
	//cache[]
	mapbuf.cache.capacity = mapbuf.cache.size;
	mapbuf.cache.arr = (cac_cache*)malloc(mapbuf.cache.capacity * sizeof(cac_cache));
	fread(mapbuf.cache.arr, sizeof(cac_cache), mapbuf.cache.size, mapfp);

	//num load
	fread(&(mapbuf.loads.size), sizeof(int), 1, mapfp);
	//load[]
	mapbuf.loads.capacity = mapbuf.loads.size;
	mapbuf.loads.arr = (m_maploader*)malloc(mapbuf.loads.capacity * sizeof(m_maploader));
	fread(mapbuf.loads.arr, sizeof(m_maploader), mapbuf.loads.size, mapfp);

	//num npc
	fread(&(mapbuf.npca.size), sizeof(int), 1, mapfp);
	//npc[]
	mapbuf.npca.capacity = mapbuf.npca.size;
	mapbuf.npca.arr = (n_npc*)malloc(mapbuf.npca.capacity * sizeof(n_npc));
	fread(mapbuf.npca.arr, sizeof(n_npc), mapbuf.npca.size, mapfp);

	//num tile evt
	fread(&(mapbuf.tfn.size), sizeof(int), 1, mapfp);
	//tile evt[]
	mapbuf.tfn.capacity = mapbuf.tfn.size;
	mapbuf.tfn.arr = (m_tilefn*)malloc(mapbuf.tfn.capacity * sizeof(m_tilefn));
	fread(mapbuf.tfn.arr, sizeof(m_tilefn), mapbuf.tfn.size, mapfp);

	//num break hash
	fread(&(mapbuf.breaks.size), sizeof(int), 1, mapfp);
	//break hash[]
	mapbuf.breaks.capacity = mapbuf.breaks.size;
	mapbuf.breaks.arr = (bt_breaktile*)malloc(mapbuf.breaks.capacity * sizeof(bt_breaktile));
	fread(mapbuf.breaks.arr, sizeof(bt_breaktile), mapbuf.breaks.size, mapfp);

	mapbuf.breaks.w = mapbuf.arr[0].width;
	mapbuf.breaks.h = mapbuf.arr[0].height;

	//num texts
	fread(&(mapbuf.texts.size), sizeof(int), 1, mapfp);

	//texts[]
	mapbuf.texts.capacity = mapbuf.texts.size;
	mapbuf.texts.arr = (m_texttile*)malloc(mapbuf.texts.capacity * sizeof(m_texttile));
	fread(mapbuf.texts.arr, sizeof(m_texttile), mapbuf.texts.size, mapfp);

	//adjacents
	fread(mapbuf.adj, sizeof(int32_t), MAP_ADJ_LEN, mapfp);

	return mapbuf;
}

void writemapp(m_map* map, const char* path)
{
	//layer num
	fwrite(&(map->size), sizeof(int), 1, mapfp);

	//each layer
	for (int i = 0; i < map->size; i++)
	{
		//layer wid, hei, ox, oy
		fwrite(&(map->arr[i].width), sizeof(int), 1, mapfp);
		fwrite(&(map->arr[i].height), sizeof(int), 1, mapfp);
		fwrite(&(map->arr[i].ox), sizeof(int), 1, mapfp);
		fwrite(&(map->arr[i].oy), sizeof(int), 1, mapfp);

		//layer num tiles
		fwrite(&(map->arr[i].tile.size), sizeof(int), 1, mapfp);

		//each tile
		for (int ii = 0; ii < map->arr[i].tile.size; ii++)
		{
			//number of stacks
			fwrite(&(map->arr[i].tile.arr[ii].size), sizeof(int), 1, mapfp);
			//stack[]
			fwrite(map->arr[i].tile.arr[ii].arr, sizeof(s_sprite), map->arr[i].tile.arr[ii].size, mapfp);
		}
	}

	//num cache
	fwrite(&(map->cache.size), sizeof(int), 1, mapfp);
	//cache[]
	fwrite(map->cache.arr, sizeof(cac_cache), map->cache.size, mapfp);

	//num load
	fwrite(&(map->loads.size), sizeof(int), 1, mapfp);
	//load[]
	fwrite(map->loads.arr, sizeof(m_maploader), map->loads.size, mapfp);

	//num npc
	fwrite(&(map->npca.size), sizeof(int), 1, mapfp);
	//npc[]
	fwrite(map->npca.arr, sizeof(n_npc), map->npca.size, mapfp);

	//num tile evt
	fwrite(&(map->tfn.size), sizeof(int), 1, mapfp);
	//tile evt[]
	fwrite(map->tfn.arr, sizeof(m_tilefn), map->tfn.size, mapfp);

	//num break hash
	fwrite(&(map->breaks.size), sizeof(int), 1, mapfp);
	//break hash[]
	fwrite(map->breaks.arr, sizeof(bt_breaktile), map->breaks.size, mapfp);

	//num texts
	fwrite(&(map->texts.size), sizeof(int), 1, mapfp);
	//texts[]
	fwrite(map->texts.arr, sizeof(m_texttile), map->texts.size, mapfp);

	//adjacents
	fwrite(map->adj, sizeof(int32_t), MAP_ADJ_LEN, mapfp);
}

void writemapex(m_map* map, const char* path, const char* mode)
{
	FILE* fp;
	fp = fopen(path, mode);

	//layer num
	fwrite(&(map->size), sizeof(int), 1, fp);

	//each layer
	for (int i = 0; i < map->size; i++)
	{
		//layer wid, hei, ox, oy
		fwrite(&(map->arr[i].width), sizeof(int), 1, fp);
		fwrite(&(map->arr[i].height), sizeof(int), 1, fp);
		fwrite(&(map->arr[i].ox), sizeof(int), 1, fp);
		fwrite(&(map->arr[i].oy), sizeof(int), 1, fp);

		//layer num tiles
		fwrite(&(map->arr[i].tile.size), sizeof(int), 1, fp);

		//each tile
		for (int ii = 0; ii < map->arr[i].tile.size; ii++)
		{
			//number of stacks
			fwrite(&(map->arr[i].tile.arr[ii].size), sizeof(int), 1, fp);
			//stack[]

			for (int iii = 0; iii < map->arr[i].tile.arr[ii].size; iii++)
				map->arr[i].tile.arr[ii].arr[iii].deltatime = 0;

			fwrite(map->arr[i].tile.arr[ii].arr, sizeof(s_sprite), map->arr[i].tile.arr[ii].size, fp);
		}
	}

	//num cache
	fwrite(&(map->cache.size), sizeof(int), 1, fp);
	//cache[]
	fwrite(map->cache.arr, sizeof(cac_cache), map->cache.size, fp);

	//num load
	fwrite(&(map->loads.size), sizeof(int), 1, fp);
	//load[]
	fwrite(map->loads.arr, sizeof(m_maploader), map->loads.size, fp);

	//num npc
	fwrite(&(map->npca.size), sizeof(int), 1, fp);
	//npc[]
	fwrite(map->npca.arr, sizeof(n_npc), map->npca.size, fp);

	//num tile evt
	fwrite(&(map->tfn.size), sizeof(int), 1, fp);
	//tile evt[]
	fwrite(map->tfn.arr, sizeof(m_tilefn), map->tfn.size, fp);

	//num break hash
	fwrite(&(map->breaks.size), sizeof(int), 1, fp);
	//break hash[]
	fwrite(map->breaks.arr, sizeof(bt_breaktile), map->breaks.size, fp);

	//num texts
	fwrite(&(map->texts.size), sizeof(int), 1, fp);
	//texts[]
	fwrite(map->texts.arr, sizeof(m_texttile), map->texts.size, fp);

	//adjacents
	fwrite(map->adj, sizeof(int32_t), MAP_ADJ_LEN, fp);

	fclose(fp);
}

m_map readmapex(const char* path)
{
	m_map mapbuf;

	FILE* fp;
	fp = fopen(path, "rb");

	//layer num
	fread(&(mapbuf.size), sizeof(int), 1, mapfp);
	mapbuf.capacity = mapbuf.size;
	mapbuf.arr = (m_maplayer*)malloc(mapbuf.capacity * sizeof(m_maplayer));

	//each layer
	for (int i = 0; i < mapbuf.size; i++)
	{
		//layer wid, hei, ox, oy
		fread(&(mapbuf.arr[i].width), sizeof(int), 1, mapfp);
		fread(&(mapbuf.arr[i].height), sizeof(int), 1, mapfp);
		fread(&(mapbuf.arr[i].ox), sizeof(int), 1, mapfp);
		fread(&(mapbuf.arr[i].oy), sizeof(int), 1, mapfp);

		//layer num tiles
		fread(&(mapbuf.arr[i].tile.size), sizeof(int), 1, mapfp);
		mapbuf.arr[i].tile.capacity = mapbuf.arr[i].tile.size;
		mapbuf.arr[i].tile.arr = (s_sprite_a*)malloc(mapbuf.arr[i].tile.capacity * sizeof(s_sprite_a));

		//each tile
		for (int ii = 0; ii < mapbuf.arr[i].tile.size; ii++)
		{
			//number of stacks
			fread(&(mapbuf.arr[i].tile.arr[ii].size), sizeof(int), 1, mapfp);
			//stack[]
			mapbuf.arr[i].tile.arr[ii].capacity = mapbuf.arr[i].tile.arr[ii].size;
			mapbuf.arr[i].tile.arr[ii].arr = (s_sprite*)malloc(mapbuf.arr[i].tile.capacity * sizeof(s_sprite));
			fread(mapbuf.arr[i].tile.arr[ii].arr, sizeof(s_sprite), mapbuf.arr[i].tile.arr[ii].size, mapfp);
		}
	}

	//num cache
	fread(&(mapbuf.cache.size), sizeof(int), 1, mapfp);
	//cache[]
	mapbuf.cache.capacity = mapbuf.cache.size;
	mapbuf.cache.arr = (cac_cache*)malloc(mapbuf.cache.capacity * sizeof(cac_cache));
	fread(mapbuf.cache.arr, sizeof(cac_cache), mapbuf.cache.size, mapfp);

	//num load
	fread(&(mapbuf.loads.size), sizeof(int), 1, mapfp);
	//load[]
	mapbuf.loads.capacity = mapbuf.loads.size;
	mapbuf.loads.arr = (m_maploader*)malloc(mapbuf.loads.capacity * sizeof(m_maploader));
	fread(mapbuf.loads.arr, sizeof(m_maploader), mapbuf.loads.size, mapfp);

	//num npc
	fread(&(mapbuf.npca.size), sizeof(int), 1, mapfp);
	//npc[]
	mapbuf.npca.capacity = mapbuf.npca.size;
	mapbuf.npca.arr = (n_npc*)malloc(mapbuf.npca.capacity * sizeof(n_npc));
	fread(mapbuf.npca.arr, sizeof(n_npc), mapbuf.npca.size, mapfp);

	//num tile evt
	fread(&(mapbuf.tfn.size), sizeof(int), 1, mapfp);
	//tile evt[]
	mapbuf.tfn.capacity = mapbuf.tfn.size;
	mapbuf.tfn.arr = (m_tilefn*)malloc(mapbuf.tfn.capacity * sizeof(m_tilefn));
	fread(mapbuf.tfn.arr, sizeof(m_tilefn), mapbuf.tfn.size, mapfp);

	//num break hash
	fread(&(mapbuf.breaks.size), sizeof(int), 1, mapfp);
	//break hash[]
	mapbuf.breaks.capacity = mapbuf.breaks.size;
	mapbuf.breaks.arr = (bt_breaktile*)malloc(mapbuf.breaks.capacity * sizeof(bt_breaktile));
	fread(mapbuf.breaks.arr, sizeof(bt_breaktile), mapbuf.breaks.size, mapfp);

	mapbuf.breaks.w = mapbuf.arr[0].width;
	mapbuf.breaks.h = mapbuf.arr[0].height;

	//num texts
	fread(&(mapbuf.texts.size), sizeof(int), 1, mapfp);

	//texts[]
	mapbuf.texts.capacity = mapbuf.texts.size;
	mapbuf.texts.arr = (m_texttile*)malloc(mapbuf.texts.capacity * sizeof(m_texttile));
	fread(mapbuf.texts.arr, sizeof(m_texttile), mapbuf.texts.size, mapfp);

	//adjacents
	fread(mapbuf.adj, sizeof(int32_t), MAP_ADJ_LEN, mapfp);

	return mapbuf;
}

void writecache(cac_cache_a* c, const char* path, int32_t cacheoffset)
{
	fseek(mapfp, cacheoffset, SEEK_SET);
	fwrite(&(c->size), sizeof(int), 1, mapfp);
	fwrite(c->arr, sizeof(cac_cache), c->size, mapfp);
}

void writenpcaex(n_npc_a* c, const char* path, int32_t npcoffset)
{
	fseek(mapfp, npcoffset, SEEK_SET);
	fwrite(&(c->size), sizeof(int), 1, mapfp);
	fwrite(c->arr, sizeof(n_npc), c->size, mapfp);
}

inline void writenpca(n_npc_a* c, const char* path, int32_t npcoffset)
{
	for (int i = 0; i < c->size; i++)
	{
		c->arr[0].ch.dx = c->arr[0].ch.sprite.x = c->arr[0].ix;
		c->arr[0].ch.dy = c->arr[0].ch.sprite.y = c->arr[0].iy;

		c->arr[0].ch.state = CH_STATE_IDLE;
	}

	writenpcaex(c, path, npcoffset);
}

void seekin_push_map_seek()
{
	for (int i = 0; i < loadstack.size; i++)
		if (mapseek == loadstack.arr[i].seekin)
		{
			push_back_mapload(
				&map.loads,
				&loadstack.arr[i].load);

			return;
		}
}

char load_mapload(int i)
{
	char flag = 0;

	if ((map.loads.arr[i].seek == -1)
		&& ((flag = tile_load_map_spec(&map, i)) == 1))
		return 0;

	ch.sprite.x = map.loads.arr[i].ox + defr;
	ch.sprite.y = map.loads.arr[i].oy + defr;

	int seek = map.loads.arr[i].seek;

	m_mapseek buf;
	buf.load.seek = mapseek;
	buf.seekin = seek;
	buf.load.ix = map.loads.arr[i].ox;
	buf.load.iy = map.loads.arr[i].oy;
	buf.load.ox = map.loads.arr[i].ix;
	buf.load.oy = map.loads.arr[i].iy;

	mapseek = seek;

	map_joinmv();

	free_bthmupdatea(&bthmupdate);
	bthmupdate = create_bthmupdatea(1);

	npcai = -1;
	npcidx = -1;

	if (flag)
		return 0;

	if (loadstack.size - 1 >= 0 &&
		(buf.load.ox == loadstack.arr[loadstack.size - 1].load.ix &&
			buf.load.oy == loadstack.arr[loadstack.size - 1].load.iy &&
			buf.load.ix == loadstack.arr[loadstack.size - 1].load.ox &&
			buf.load.iy == loadstack.arr[loadstack.size - 1].load.oy &&
			mapseek == loadstack.arr[loadstack.size - 1].load.seek))
	{
		seekin_push_map_seek();
		loadstack.size--;
	}
	else
	{
		buf.load.ix += defr;
		buf.load.iy += defr;
		buf.load.ox += -defr;
		buf.load.oy += -defr;

		push_back_mapseek(
			&loadstack,
			&buf
		);

		push_back_mapload(
			&map.loads,
			&loadstack.arr[loadstack.size - 1].load
		);
	}

	ch.dx = (int)ch.sprite.x;
	ch.dy = (int)ch.sprite.y;

	collision_full();

	return 1;
}

char tile_load_map(float dx, float dy)
{
	for (int i = 0; i < map.loads.size; i++)
		if (map.loads.arr[i].ix == ch.sprite.x &&
			map.loads.arr[i].iy == ch.sprite.y)
			if (map.loads.arr[i].ix == dx && map.loads.arr[i].iy == dy)
				return 0;
			else
				return load_mapload(i);

	return 0;
}

void tile_load_map_ts(float* x, float* y, m_map* map, const char* path)
{
	for (int i = 0; i < map->loads.size; i++)
		if (map->loads.arr[i].ix == *x &&
			map->loads.arr[i].iy == *y)
		{
			*x = map->loads.arr[i].ox;
			*y = map->loads.arr[i].oy;
			int seek = map->loads.arr[i].seek;
			free_map(map);
			*map = readmapp(path, seek);
			return;
		}
}

m_map default_map(int l, int w, int h, int fill)
{
	m_map buf = create_map(l);

	memset(buf.adj, -1, sizeof(buf.adj));

	m_maplayer layer;

	layer = default_map_layer(w, h, fill, 0, 0);
	push_back_map(&buf, &layer);

	for (l--; l; --l)
	{
		layer = default_map_layer(w, h, -1, 0, 0);
		push_back_map(&buf, &layer);
	}

	buf.cache = create_cachea(1);
	buf.loads = create_mapload(1);
	buf.npca = create_NPCA(1);
	buf.tfn = create_tilefna(1);
	buf.breaks = create_breaktileh(w * h);
	buf.breaks.size = buf.breaks.capacity;
	for (int i = 0; i < buf.breaks.size; i++)
		buf.breaks.arr[i].breakfnid = buf.breaks.arr[i].breakmb = buf.breaks.arr[i].breakma = -1;

	buf.texts = create_TextTileA(1);

	return buf;
}

char collision_map_char(ch_char* ch)
{
	char col = 0;

	char flag0[] = { SPRITE_FLAG_BLOCK, SPRITE_FLAG_BREAK, SPRITE_FLAG_CACHE, SPRITE_FLAG_TERM, SPRITE_FLAG_TEXT, SPRITE_FLAG_WATER, };
	char flag1[] = { SPRITE_FLAG_WATER, };

	char xrs[] = { 0, 1, };
	char xr;
	char* flag;
	int l;

	if (ch->state == CH_STATE_DIVE)
	{
		flag = flag1;
		l = sizeof(flag1) / sizeof(*flag1);
		xr = 1;
	}
	else
	{
		flag = flag0;
		l = sizeof(flag0) / sizeof(*flag0);
		xr = 0;
	}

	for (int x = 0; !col && x <= 1; x++)
		for (int y = 0; !col && y <= 1; y++)
		{
			s_sprite* s = &map.arr[1].tile.arr[p2idx(
				x * (ch->dx - ch->sprite.x) + ch->sprite.x,
				y * (ch->dy - ch->sprite.y) + ch->sprite.y,
				map.arr[1].width)].arr[0];

			for (int i = 0; !col && i < l; i++)
				col |= (xr ^ (s->flag == flag[i]));
		}

	return col;
}

bt_breaktile_h create_breaktileh_auto(m_map* map)
{
	bt_breaktile_h buf = create_breaktileh_wh(map->arr[1].width, map->arr[1].height);

	for (int i = 0; i < buf.size; i++)
	{
		buf.arr[i].breakmb = buf.arr[i].breakma = (map->arr[1].tile.arr[i].arr[0].flag == SPRITE_FLAG_BREAK) * 8;
		buf.arr[i].breakfnid = 0;
	}

	return buf;
}

void update_bthm(bt_bthmupdate_a* bthm, m_maplayer* l, int p)
{
	static unsigned char bthmseed = 0;

	for (int i = 0; i < bthm->size; i++)
	{
		float dx = 0;
		float dy = (((i + bthmseed + bthm->arr[i]) % 2) == 0 ? 0 : 1);

#define TILE_SHAKE_MUL	.0625f

		l->tile.arr[bthm->arr[i]].arr[p].x =
			(int)(l->tile.arr[bthm->arr[i]].arr[p].x)
			+ dx * TILE_SHAKE_MUL;

		l->tile.arr[bthm->arr[i]].arr[p].y =
			(int)(l->tile.arr[bthm->arr[i]].arr[p].y)
			+ dy * TILE_SHAKE_MUL;
	}

	bthmseed++;
}

m_texttile_a create_TextTileA(int capacity)
{
	m_texttile_a buf;
	buf.arr = (m_texttile*)malloc(capacity * sizeof(m_texttile));
	buf.size = 0;
	buf.capacity = capacity;
	return buf;
}

void free_TextTile(m_texttile_a* array)
{
	free(array->arr);
}

void resize_array_TextTile(m_texttile_a* array, int new_capacity)
{
	m_texttile* new_arr = (m_texttile*)malloc(new_capacity * sizeof(m_texttile));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_TextTile(m_texttile_a* array, m_texttile* value)
{
	if (array->size == array->capacity)
		resize_array_TextTile(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}

m_map initjm(int w, int h, int ll)
{
	m_map buf = create_map(ll);

	for (int i = 0; i < ll; i++)
	{
		m_maplayer l;
		l.ox = 0;
		l.oy = 0;
		l.width = w;
		l.height = h;
		l.tile = create_spraa(l.width * l.height);
		l.tile.size = l.tile.capacity;

		for (int ii = 0; ii < l.tile.size; ii++) l.tile.arr[ii] = create_sprarr(1);

		push_back_map(&buf, &l);
	}

	buf.npca = create_NPCA(1);
	buf.breaks = create_breaktileh_wh(w, h);
	buf.cache = create_cachea(1);
	buf.loads = create_mapload(1);
	buf.tfn = create_tilefna(1);
	buf.texts = create_TextTileA(1);

	memset(buf.adj, -1, sizeof(buf.adj));

	return buf;
}

unsigned int pm(int a, int b)
{
	return ((a % b) + b) % b;
}

//input, m, output, buf
void joinm(m_map* buf, m_map* m, int xo, int yo)
{
	{
		int w0 = m->arr[0].width, h0 = m->arr[0].height;
		int w1 = buf->arr[0].width, h1 = buf->arr[0].height;

		for (int y = 0; y < h0; y++)
			for (int x = 0; x < w0; x++)
				buf->breaks.arr[p2idx(x + xo, y + yo, w1)]
				= m->breaks.arr[p2idx(x, y, w0)];
	}

	for (int i = 0; i < m->tfn.size; i++)
	{
		push_back_tilefna(
			&(buf->tfn),
			&(m->tfn.arr[i])
		);

		buf->tfn.arr[buf->tfn.size - 1]
			.ix += xo;
		buf->tfn.arr[buf->tfn.size - 1]
			.iy += yo;
	}
	for (int i = 0; i < m->loads.size; i++)
	{
		push_back_mapload(&(buf->loads),
			&(m->loads.arr[i]));

		buf->loads.arr[buf->loads.size - 1]
			.ix += xo;
		buf->loads.arr[buf->loads.size - 1]
			.iy += yo;
	}

	for (int i = 0; i < m->cache.size; i++)
	{
		push_back_cache(&(buf->cache),
			&(m->cache.arr[i]));

		buf->cache.arr[buf->cache.size - 1]
			.x += xo;
		buf->cache.arr[buf->cache.size - 1]
			.y += yo;
	}
	for (int i = 0; i < m->npca.size; i++)
	{
		push_back_NPCA(&(buf->npca), &(m->npca.arr[i]));

		buf->npca.arr[buf->npca.size - 1]
			.ch.sprite.x += xo;
		buf->npca.arr[buf->npca.size - 1]
			.ch.sprite.y += yo;

		buf->npca.arr[buf->npca.size - 1]
			.ix += xo;
		buf->npca.arr[buf->npca.size - 1]
			.iy += yo;

		buf->npca.arr[buf->npca.size - 1]
			.ch.dx += xo;
		buf->npca.arr[buf->npca.size - 1]
			.ch.dy += yo;
	}

	for (int i = 0; i < m->texts.size; i++)
	{
		push_back_TextTile(&buf->texts, &m->texts.arr[i]);

		buf->texts.arr[buf->texts.size - 1]
			.x += xo;
		buf->texts.arr[buf->texts.size - 1]
			.y += yo;
	}

	//deep copy tiles
	for (int i = 0; i < m->size; i++)
	{
		//deep copy every tile from the input map
		for (int ii = 0; ii < m->arr[i].tile.size; ii++)
		{
			for (int iii = 0; iii < m->arr[i].tile.arr[ii].size; iii++)
			{
				m->arr[i].tile.arr[ii].arr[iii].x += m->arr[i].ox + xo;
				m->arr[i].tile.arr[ii].arr[iii].y += m->arr[i].oy + yo;
			}

			int pi = p2idx(
				m->arr[i].tile.arr[ii].arr[0].x,
				m->arr[i].tile.arr[ii].arr[0].y,
				buf->arr[i].width
			) % buf->arr[i].tile.size;

			for (int iii = 0; iii < m->arr[i].tile.arr[ii].size; iii++)
				push_back_sprarr(&(buf->arr[i].tile.arr[pi]), &(m->arr[i].tile.arr[ii].arr[iii]));
		}
	}
}

void init_cache();

//todo: rewrite this fn
void linkmap()
{
	m_map buf = initjm(defr * 3, defr * 3, 2);
	joinm(&buf, &map, defr, defr);

	for (int y = 0; y <= 2; y++)
		for (int x = 0; x <= 2; x++)
		{
			if (x == 1 && y == 1) { continue; }

			int idx = p2idx(x, y, 3);
			idx -= (idx >= (8 / 2));

			int32_t adj = map.adj[idx];

			if (adj == -1) continue;

			m_map mm = readmapp(
				PATH_VAR_MAP,
				adj
			);

			joinm(&buf, &mm, x * defr, y * defr);
			free_map(&mm);
		}

	memcpy(buf.adj, map.adj, sizeof(map.adj));
	free_map(&map);
	map = buf;

	init_cache();
}

int mapfsize(m_map* map)
{
	int buf = 0;

	buf += sizeof(int) * 1;

	for (int i = 0; i < map->size; i++)
	{
		buf +=
			+sizeof(int) * 1
			+ sizeof(int) * 1
			+ sizeof(int) * 1
			+ sizeof(int) * 1
			+ sizeof(int) * 1;

		for (int ii = 0; ii < map->arr[i].tile.size; ii++)
		{
			buf +=
				+sizeof(int) * 1
				+ sizeof(s_sprite) * map->arr[i].tile.arr[ii].size
				;
		}
	}

	buf += sizeof(int) * 1
		+ sizeof(cac_cache) * map->cache.size
		+ sizeof(int) * 1
		+ sizeof(m_maploader) * map->loads.size
		+ sizeof(int) * 1
		+ sizeof(n_npc) * map->npca.size
		+ sizeof(int) * 1
		+ sizeof(m_tilefn) * map->tfn.size
		+ sizeof(int) * 1
		+ sizeof(bt_breaktile) * map->breaks.size
		+ sizeof(int) * 1
		+ sizeof(m_texttile) * map->texts.size
		+ sizeof(int32_t) * MAP_ADJ_LEN;

	return buf;
}

#include "mathfunction.h"
#include "tscreenobj.h"

void rotjoinmap(char dir0, char dir1, m_map m[3])
{
	char xmin = defr * (0), xmax = defr * (3 - 1);

	if (dir0 < 0)
	{
		xswap(xmin, xmax);
	}

	//offset the tiles
	for (int i = 0; i < 2; i++)
		for (int y = 0; y < defr * 3; y++)
			for (int x = xmin; x != xmax; x += dir0)
			{
				int xm0 = (x + defr * 0) % (defr * 3);
				int ym0 = y;
				int xm1 = (x + defr * 1) % (defr * 3);
				int ym1 = y;

				if (dir1)
				{
					xswap(xm0, ym0);
					xswap(xm1, ym1);
				}

				int idx0 = p2idx(xm0, ym0, defr * 3);
				int idx1 = p2idx(xm1, ym1, defr * 3);

				if (dir1)
				{
					xswap(xm0, ym0);
					xswap(xm1, ym1);
				}

				s_sprite_a* saa0 = &map.arr[i].tile.arr[idx0];
				s_sprite_a* saa1 = &map.arr[i].tile.arr[idx1];

				s_sprite_a t = *saa0;

				*saa0 = *saa1;
				*saa1 = t;

				{
					for (int i = 0; i < saa0->size; i++)
					{
						float* xm =
							dir1
							? &saa0->arr[i].y
							: &saa0->arr[i].x
							;
						*xm += (xm0 - *xm);
					}

					for (int i = 0; i < saa1->size; i++)
					{
						float* xm =
							dir1
							? &saa1->arr[i].y
							: &saa1->arr[i].x
							;
						*xm += (xm1 - *xm);
					}
				}
			}

	//offset the breaks
	{
		for (int y = 0; y < defr * 3; y++)
			for (int x = xmin; x != xmax; x += dir0)
			{
				int xm0 = (x + defr * 0) % (defr * 3);
				int ym0 = y;
				int xm1 = (x + defr * 1) % (defr * 3);
				int ym1 = y;

				if (dir1)
				{
					xswap(xm0, ym0);
					xswap(xm1, ym1);
				}

				int idx0 = p2idx(xm0, ym0, defr * 3);
				int idx1 = p2idx(xm1, ym1, defr * 3);

				bt_breaktile* ba0 = &map.breaks.arr[idx0];
				bt_breaktile* ba1 = &map.breaks.arr[idx1];

				bt_breaktile t = *ba0;

				*ba0 = *ba1;
				*ba1 = t;
			}
	}

	//offsets
	{
		//offset tscreenobj
		{
			for (int i = 0; i < tsoa.size; i++)
			{
				tsoa.arr[i].pos -= (dir0 * defr) * (
					dir1
					? map.arr[1].width
					: 1
					);
			}
		}

		//offset the texts
		{
			for (int i = 0; i < map.texts.size; i++)
			{
				int* xm =
					dir1
					? &map.texts.arr[i].y
					: &map.texts.arr[i].x
					;
				*xm -= defr * dir0;
			}

			for (int ii = 0; ii < 2; ii++)
				for (int i = 0; i < map.texts.size; i++)
					if (
						map.texts.arr[i].x < 0 ||
						map.texts.arr[i].x >= map.arr[1].width ||
						map.texts.arr[i].y < 0 ||
						map.texts.arr[i].y >= map.arr[1].height
						)
					{
						map.texts.arr[i] = map.texts.arr[--map.texts.size];
						i = 0;
					}
		}

		//offset the tfn
		{
			for (int i = 0; i < map.tfn.size; i++)
			{
				int* xm0 =
					dir1
					? &map.tfn.arr[i].iy
					: &map.tfn.arr[i].ix
					;

				int* xm1 =
					dir1
					? &map.tfn.arr[i].oy
					: &map.tfn.arr[i].ox
					;

				*xm0 -= defr * dir0;
				*xm1 -= defr * dir0;
			}

			for (int ii = 0; ii < 2; ii++)
				for (int i = 0; i < map.tfn.size; i++)
					if (
						map.tfn.arr[i].ix < 0 ||
						map.tfn.arr[i].ix >= map.arr[1].width ||
						map.tfn.arr[i].iy < 0 ||
						map.tfn.arr[i].iy >= map.arr[1].height
						)
					{
						map.tfn.arr[i] = map.tfn.arr[--map.tfn.size];
						i = 0;
					}
		}
		//offset the cache
		{
			for (int i = 0; i < map.cache.size; i++)
			{
				int* x0 =
					dir1
					? &map.cache.arr[i].y
					: &map.cache.arr[i].x
					;
				*x0 -= defr * dir0;
			}

			for (int ii = 0; ii < 2; ii++)
				for (int i = 0; i < map.cache.size; i++)
				{
					if (map.cache.arr[i].x < 0 ||
						map.cache.arr[i].x >= map.arr[1].width ||
						map.cache.arr[i].y < 0 ||
						map.cache.arr[i].y >= map.arr[1].height)
						map.cache.arr[i].fnid = -2;
					else
					{
						char flag = map.arr[1]
							.tile.arr[p2idx(map.cache.arr[i].x, map.cache.arr[i].y, map.arr[1].width)]
							.arr[0].flag;

						if (flag != SPRITE_FLAG_CACHE && flag != SPRITE_FLAG_DROP)
							map.cache.arr[i].fnid = -2;
					}
				}

			for (int ii = 0; ii < 2; ii++)
				for (int i = 0; i < map.cache.size; i++)
					if (map.cache.arr[i].fnid == -2)
					{
						map.cache.arr[i] = map.cache.arr[--map.cache.size];
						i = 0;
					}
		}

		//offset the npcs
		{
			for (int i = 0; i < map.npca.size; i++)
			{
				n_npc* at = &map.npca.arr[i];
				*(dir1 ? &(at->iy) : &(at->ix)) -= defr * dir0;
				*(dir1 ? &(at->ch.dy) : &(at->ch.dx)) -= defr * dir0;
				*(dir1 ? &(at->ch.sprite.y) : &(at->ch.sprite.x)) -= defr * dir0;
			}

			for (int ii = 0; ii < 2; ii++)
				for (int i = 0; i < map.npca.size; i++)
					if (
						map.npca.arr[i].ch.dx < 0 ||
						map.npca.arr[i].ch.dx >= map.arr[1].width ||
						map.npca.arr[i].ch.dy < 0 ||
						map.npca.arr[i].ch.dy >= map.arr[1].height
						)
					{
						map.npca.arr[i] = map.npca.arr[--map.npca.size];
						i = 0;
					}
		}

		//offset the loads
		{
			for (int i = 0; i < map.loads.size; i++)
			{
				m_maploader* at = &map.loads.arr[i];
				*(dir1 ? &(at->iy) : &(at->ix)) -= defr * dir0;
			}

			for (int ii = 0; ii < 2; ii++)
				for (int i = 0; i < map.loads.size; i++)
					if (
						map.loads.arr[i].ix < 0 ||
						map.loads.arr[i].ix >= map.arr[1].width ||
						map.loads.arr[i].iy < 0 ||
						map.loads.arr[i].iy >= map.arr[1].height
						)
					{
						map.loads.arr[i] = map.loads.arr[--map.loads.size];
						i = 0;
					}
		}
	}

	//place in the new maps
	if (m)
	{
		//place in the new tiles
		{
			char
				xmin = (1 + dir0) * defr,
				xmax = (2 + dir0) * defr;

			char xmmin = min(xmin, xmax);

			for (int i = 0; i < map.size; i++)
				for (int y = defr * 0; y < defr * 3; y++)
					for (int x = xmin; x != xmax; x++)
					{
						int xm0 = x;
						int ym0 = y;
						int xm1 = x - xmmin;
						int ym1 = y % defr;

						if (dir1)
						{
							xswap(xm0, ym0);
							xswap(xm1, ym1);
						}

						int idx0 = p2idx(xm0, ym0, defr * 3);
						int idx1 = p2idx(xm1, ym1, defr * 1);

						//swapping will cause memory leaks
						s_sprite_a* sa0 = &map.arr[i].tile.arr[idx0];
						s_sprite_a* sa1 = &m[y / defr].arr[i].tile.arr[idx1];

						free_sprarr(sa0);

						*sa0 = create_sprarr(sa1->size);
						sa0->size = sa1->size;

						memcpy(sa0->arr, sa1->arr, sizeof(s_sprite) * sa1->size);

						for (int i = 0; i < sa0->size; i++)
						{
							sa0->arr[i].x += (xm0 - sa0->arr[i].x);
							sa0->arr[i].y += (ym0 - sa0->arr[i].y);
						}
					}
		}

		//place in the new breaks
		{
			char
				xmin = (1 + dir0) * defr,
				xmax = (2 + dir0) * defr;

			char xmmin = min(xmin, xmax);

			for (int i = 0; i < map.size; i++)
				for (int y = defr * 0; y < defr * 3; y++)
					for (int x = xmin; x != xmax; x++)
					{
						int xm0 = x;
						int ym0 = y;
						int xm1 = x - xmmin;
						int ym1 = y % defr;

						if (dir1)
						{
							xswap(xm0, ym0);
							xswap(xm1, ym1);
						}

						int idx0 = p2idx(xm0, ym0, defr * 3);
						int idx1 = p2idx(xm1, ym1, defr * 1);

						bt_breaktile* bt0 = &map.breaks.arr[idx0];
						bt_breaktile* bt1 = &m[y / defr].breaks.arr[idx1];

						*bt0 = *bt1;
					}
		}

		//adds
		{
			//place in the texts
			for (int i = 0; i < 3; i++)
			{
				m_texttile_a* at = &m[i].texts;
				for (int ii = 0; ii < at->size; ii++)
				{
					m_texttile* t = &at->arr[ii];

					*(dir1 ? &(t->y) : &(t->x)) += defr + defr * dir0;
					*(dir1 ? &(t->x) : &(t->y)) += i * defr;

					push_back_TextTile(&map.texts, t);
				}
			}

			//place in the tfn
			for (int i = 0; i < 3; i++)
			{
				m_tilefn_a* at = &m[i].tfn;
				for (int ii = 0; ii < at->size; ii++)
				{
					m_tilefn* t = &at->arr[ii];

					*(dir1 ? &(t->iy) : &(t->ix)) += defr + defr * dir0;
					*(dir1 ? &(t->ix) : &(t->iy)) += (i)*defr;
					*(dir1 ? &(t->oy) : &(t->ox)) += defr + defr * dir0;
					*(dir1 ? &(t->ox) : &(t->oy)) += (i)*defr;

					push_back_tilefna(&map.tfn, t);
				}
			}

			//place in the cache
			for (int i = 0; i < 3; i++)
			{
				cac_cache_a* at = &m[i].cache;
				for (int ii = 0; ii < at->size; ii++)
				{
					cac_cache* buf = &at->arr[ii];

					*(dir1 ? &(buf->y) : &(buf->x)) += defr + defr * dir0;
					*(dir1 ? &(buf->x) : &(buf->y)) += (i)*defr;

					push_back_cache(&map.cache, buf);
				}
			}

			//place in the npcs
			for (int i = 0; i < 3; i++)
			{
				n_npc_a* at = &m[i].npca;
				for (int ii = 0; ii < at->size; ii++)
				{
					n_npc* buf = &at->arr[ii];

					int xm = defr + defr * dir0;
					int ym = (i)*defr;

					if (dir1) { xswap(xm, ym); }

					buf->ix += xm;
					buf->ch.sprite.x += xm;
					buf->ch.dx += xm;

					buf->iy += ym;
					buf->ch.sprite.y += ym;
					buf->ch.dy += ym;

					push_back_NPCA(&map.npca, buf);
				}
			}

			//place in the loads
			for (int i = 0; i < 3; i++)
			{
				m_maploader_a* at = &m[i].loads;

				for (int ii = 0; ii < at->size; ii++)
				{
					m_maploader* buf = &at->arr[ii];

					*(dir1 ? &(buf->iy) : &(buf->ix)) += defr + defr * dir0;
					*(dir1 ? &(buf->ix) : &(buf->iy)) += (i)*defr;

					push_back_mapload(&map.loads, buf);
				}
			}
		}

		//more generalizable if free map buffer done outside the fn's scope
	}
}

void init_cache();

//TODO: theres a glitch in this function that occurs arround the boundaries
void adjust_map()
{
	char buf[2];
	if (
		(buf[0] =
			(ch.dx < 1 * 16 || ch.dx > 2 * 16))
		|
		(buf[1] =
			(ch.dy < 1 * 16 || ch.dy > 2 * 16))
		)
	{
		m_map m[3];

		char dir0, dir1;
		char idx[3];

		char dx = sign((defr * 3) / 2 - ch.dx);
		char dy = sign((defr * 3) / 2 - ch.dy);

		{
			if (buf[0] && dx == -1)
			{
				dir0 = +1;
				dir1 = 0;
				char idx0[3] = { 2,4,7, };
				memcpy(idx, idx0, sizeof(idx));
			}
			else if (buf[1] && dy == -1)
			{
				dir0 = +1;
				dir1 = 1;
				char idx0[3] = { 5,6,7, };
				memcpy(idx, idx0, sizeof(idx));
			}
			else if (buf[0] && dx == +1)
			{
				dir0 = -1;
				dir1 = 0;
				char idx0[3] = { 0,3,5, };
				memcpy(idx, idx0, sizeof(idx));
			}
			else if (buf[1] && dy == +1)
			{
				dir0 = -1;
				dir1 = 1;
				char idx0[3] = { 0,1,2, };
				memcpy(idx, idx0, sizeof(idx));
			}
			{
				if (map.adj[idx[1]] == -1) return;
				m_map mbuf = readmapp(PATH_VAR_MAP, mapseek = map.adj[idx[1]]);

				for (int i = 0; i < 3; i++)
				{
					if (-1 == mbuf.adj[idx[i]])
					{
						m_map buf = create_map(2);

						buf.cache = create_cachea(1);
						buf.loads = create_mapload(1);
						buf.npca = create_NPCA(1);
						buf.texts = create_TextTileA(1);
						buf.tfn = create_tilefna(1);
						buf.breaks = create_breaktileh(defr * defr);

						for (int i = 0; i < 2; i++)
						{
							m_maplayer lbuf = create_map_layer(defr, defr);

							lbuf.tile.size = lbuf.tile.capacity;

							for (int i = 0; i < lbuf.tile.size; i++)
							{
								s_sprite s;
								memset(&s, 0, sizeof(s));
								s.id = -1;

								lbuf.tile.arr[i] = create_sprarr(1);
								push_back_sprarr(
									&lbuf.tile.arr[i]
									, &s
								);
							}

							buf.arr[i] = lbuf;
						}

						m[i] = buf;
					}
					else
						m[i] = readmapp(PATH_VAR_MAP, mbuf.adj[idx[i]]);
				}

				memcpy(map.adj, mbuf.adj, sizeof(map.adj));
				free_map(&mbuf);
			}
		}

		rotjoinmap(dir0, dir1, m);

		for (int i = 0; i < 3; i++)
			free_map(&m[i]);

		{
			statusr = 1;
			init_cache();
			free_bthmupdatea(&bthmupdate);
			bthmupdate = create_bthmupdatea(1);

			seekin_push_map_seek();
		}

		if (buf[0])
			ddx = ch.dx = (ch.sprite.x += defr * dx);
		else if (buf[1])
			ddy = ch.dy = (ch.sprite.y += defr * dy);
	}
}

#include "dglobal.h"
#include "tilefn.h"

//TODO: this function needs to be rewritten -- anything collision related as well
void collision_full()
{
	{
		{
			int id = map.arr[0].tile.arr[p2idx(ch.dx, ch.dy, map.arr[1].width)].arr[0].id;

			if (id < 0
				|| (ch.dx >= map.arr[1].width || ch.dy >= map.arr[1].height || ch.dx < 0 || ch.dy < 0)
				|| collision_map_char(&ch) || collision_npca_char(&map.npca, &ch))
			{
				if (ch.state != CH_STATE_WALK)
					ch_set_state(&ch, dr, CH_STATE_IDLE);

				ch.dy = ddy;
				ch.dx = ddx;
			}
		}

		{
			int flag = map.arr[1].tile.arr[p2idx(ch.dx, ch.dy, map.arr[1].width)].arr[0].flag;

			{
				if (flag == SPRITE_FLAG_STCHNG)
					tile_set_map_state();

				else if (flag == SPRITE_FLAG_DROP)
				{
					s_sprite* s;

					if (SPRITE_FLAG_DROP == (s = &map.arr[1].tile.arr[p2idx
					(ch.sprite.x, ch.sprite.y, map.arr[1].width)]
						.arr[0])->flag

						|| SPRITE_FLAG_DROP == (s = &map.arr[1].tile.arr[p2idx
						(ch.sprite.x + (ch.rot == 0), ch.sprite.y, map.arr[1].width)]
							.arr[0])->flag

						|| SPRITE_FLAG_DROP == (s = &map.arr[1].tile.arr[p2idx
						(ch.sprite.x - (ch.rot == 2), ch.sprite.y, map.arr[1].width)]
							.arr[0])->flag

						|| SPRITE_FLAG_DROP == (s = &map.arr[1].tile.arr[p2idx
						(ch.sprite.x, ch.sprite.y + (ch.rot == 1), map.arr[1].width)]
							.arr[0])->flag

						|| SPRITE_FLAG_DROP == (s = &map.arr[1].tile.arr[p2idx
						(ch.sprite.x, ch.sprite.y - (ch.rot == 3), map.arr[1].width)]
							.arr[0])->flag
						)
					{
						cac_cache* at = NULL;

						for (int i = 0; i < map.cache.size; i++)
							if (map.cache.arr[i].x == (int)s->x && map.cache.arr[i].y == (int)s->y)
							{
								at = &map.cache.arr[i];
								cfn[at->fnid](at);
								break;
							}

						s->flag = SPRITE_FLAG_NONE;
						s->id = -1;
					}
				}

				else if (flag == SPRITE_FLAG_TFN)
				{
					for (int i = 0; i < map.tfn.size; i++)
					{
						if (map.tfn.arr[i].ix == (int)ch.dx && map.tfn.arr[i].iy == (int)ch.dy)
						{
							tilefn[map.tfn.arr[i].fnid](&(map.tfn.arr[i]));
							break;
						}
					}
				}

				else if (flag == SPRITE_FLAG_POPLSTACK)
				{
					int ox = map.loads.arr[map.loads.size - 1].ox;
					int oy = map.loads.arr[map.loads.size - 1].oy;

					load_mapload(map.loads.size - 1);

					ddx = chdx = ch.dx = ch.sprite.x = ox + defr;
					ddy = chdy = ch.dy = ch.sprite.y = oy + defr;
				}
				else if (flag == SPRITE_FLAG_POPDSTSTACK)
				{
					int mi = 0;
					int md = -1;
					for (int i = 0; i < map.loads.size; i++)
					{
						int d = abs(ch.sprite.x - map.loads.arr[i].ix) + abs(ch.sprite.y - map.loads.arr[i].iy);
						if (md < 0 || md > d)
						{
							md = d;
							mi = i;
						}
					}

					ddx = chdx = ch.dx = ch.sprite.x = map.loads.arr[mi].ox + defr;
					ddy = chdy = ch.dy = ch.sprite.y = map.loads.arr[mi].oy + defr;

					load_mapload(mi);
				}
			}
		}
	}

	return;
}

char ch_in_combat()
{
	for (int i = 0; i < map.npca.size; i++)
		if (map.npca.arr[i].ch.target == &ch && map.npca.arr[i].istate == NPC_STATE_ENEMY)
			return 1;
	return 0;
}

void map_joinmv()
{
	free_map(&map);
	map = readmapp(PATH_VAR_MAP, mapseek);
	linkmap();
	init_cache();
	free_bthmupdatea(&bthmupdate);
	bthmupdate = create_bthmupdatea(1);
}

void m_init_load_map()
{
	readsed(PATH_SED);
	readlas(PATH_LAS);
	map = readmapp(PATH_VAR_MAP, mapseek);
	linkmap();

	if (loadstack.size - 1 >= 0)
		push_back_mapload(&map.loads, &loadstack.arr[loadstack.size - 1].load);
}

char gen_dyn_map_node(m_map* m, int loadidx);
char (*tile_load_map_spec)(m_map* m, int loadidx) = &gen_dyn_map_node;