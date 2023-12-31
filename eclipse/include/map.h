#pragma once

#include <stdint.h>

#include "maploader.h"
#include "sprite.h"
#include "spritearray.h"
#include "save.h"
#include "npc.h"

#include <stdio.h>

extern FILE* mapfp;

#define writemap(map, path) writemapex(map, path, "wb")
#define appendmap(map, path) writemapex(map, path, "ab")
#define readmap(path) readmapex(path)

#define MAP_WIDTH	16
#define MAP_SPLIT_WIDTH	MAP_WIDTH
extern const unsigned short defr;

extern char statusr;

typedef struct
{
	int x, y;

	int spr;
	int iid;
	int cd;
	int cid;

	int fnid;
} cac_cache;

typedef struct
{
	int spr;
	short iid;
	short fnid;
} cac_cache_min;

typedef struct
{
	cac_cache* arr;
	int size;
	int capacity;
} cac_cache_a;

typedef struct
{
	int width;
	int height;
	s_sprite_aa tile;

	int ox;
	int oy;
} m_maplayer;

typedef struct
{
	int x; int y;
} point2;

typedef struct
{
	int ix, iy;
	int ox, oy;
	int fnid;
} m_tilefn;

typedef struct
{
	m_tilefn* arr;
	int size;
	int capacity;
} m_tilefn_a;

typedef struct
{
	int x;
	int y;
	int tidx;
} m_texttile;

typedef struct
{
	m_texttile* arr;
	int size;
	int capacity;
} m_texttile_a;

#include "breaktile.h"

typedef struct
{
	m_maplayer* arr;
	int size;
	int capacity;

	cac_cache_a cache;
	m_maploader_a loads;
	n_npc_a npca;
	m_tilefn_a tfn;
	bt_breaktile_h breaks;
	m_texttile_a texts;

#define MAP_ADJ_LEN	8
	int32_t adj[MAP_ADJ_LEN];
} m_map;

extern m_map map;

void update_bthm(bt_bthmupdate_a* bthm, m_maplayer* l, int p);

void update_npc(n_npc* npc, m_maplayer* l);

cac_cache create_cache(int x, int y, int spr, int iid, int fnid, int cid);
void writecid(const char* path, cac_cache* c);
void readcid(const char* path, cac_cache* c);
void readallcid(const char* path, cac_cache_a* c);
cac_cache_a create_cachea(int capacity);
void free_cache(cac_cache_a* array);
void resize_array_cache(cac_cache_a* array, int new_capacity);
void push_back_cache(cac_cache_a* array, cac_cache* value);
m_maplayer default_map_layer(int w, int h, int fill, int ox, int oy);
m_maplayer create_map_layer(int width, int height);
point2 idxp2(int i, int w);
int p2idx(int x, int y, int w);
void free_map_layer(m_maplayer* map);
m_tilefn_a create_tilefna(int capacity);
void free_tilefna(m_tilefn_a* array);
void resize_array_tilefna(m_tilefn_a* array, int new_capacity);
void push_back_tilefna(m_tilefn_a* array, m_tilefn* value);
m_map create_map(int capacity);
void free_map(m_map* array);
char empty_map(m_map* array);
int get_size_map(m_map* array);
int get_capacity_map(m_map* array);
void resize_array_map(m_map* array, int new_capacity);
void push_back_map(m_map* array, m_maplayer* value);
m_map readmapp(const char* path, int32_t offset);
void writemapp(m_map* map, const char* path);
void writemapex(m_map* map, const char* path, const char* mode);
m_map readmapex(const char* path);
void writecache(cac_cache_a* c, const char* path, int32_t cacheoffset);
void writenpcaex(n_npc_a* c, const char* path, int32_t npcoffset);
void seekin_push_map_seek();
char load_mapload(int i);
char tile_load_map(float dx, float dy);
void tile_load_map_ts(float* x, float* y, m_map* map, const char* path);
m_map default_map(int l, int w, int h, int fill);
m_texttile_a create_TextTileA(int capacity);
void free_TextTile(m_texttile_a* array);
void resize_array_TextTile(m_texttile_a* array, int new_capacity);
void push_back_TextTile(m_texttile_a* array, m_texttile* value);
char collision_map_char(ch_char* ch);
m_map initjm(int w, int h, int ll);
void joinm(m_map* buf, m_map* m, int xo, int yo);
void linkmap();
void map_joinmv();
void m_init_load_map();

void adjust_map();
void rotjoinmap(char dir0, char dir1, m_map m[3]);

int mapfsize(m_map* map);

void collision_full();

extern char (*tile_load_map_spec)(m_map* m, int loadidx);

char ch_in_combat();