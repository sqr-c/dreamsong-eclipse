#pragma once

#include "maploader.h"
#include "sprite.h"
#include "spritearray.h"
#include "save.h"
#include "npc.h"

#include <stdio.h>

extern FILE* mapfp;
extern long cacheoffset;
extern long npcoffset;

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
	//todo: add layer
	int spr;
	int iid;
	int cd;
	int cid;

	int fnid;
} cac_cache;

typedef struct
{
	int spr;
	int iid;
	int fnid;
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
	long adj[MAP_ADJ_LEN];
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
cac_cache* get_element_cache(cac_cache_a* array, int index);
void print_cache(cac_cache_a* c);
m_maplayer default_map_layer(int w, int h, int fill, int ox, int oy);
m_maplayer create_map_layer(int width, int height);
s_sprite_a* get_tile(m_maplayer* map, int x, int y);
point2 idxp2(int i, int w);
int p2idx(int x, int y, int w);
void free_map_layer(m_maplayer* map);
m_tilefn_a create_tilefna(int capacity);
void free_tilefna(m_tilefn_a* array);
void resize_array_tilefna(m_tilefn_a* array, int new_capacity);
void push_back_tilefna(m_tilefn_a* array, m_tilefn* value);
m_tilefn* get_element_tilefna(m_tilefn_a* array, int index);
void print_map_warps(m_map* map);
void print_sprite(s_sprite* s);
void print_layer_tiles(m_maplayer* l);
void print_layer_tile_flags(m_maplayer* l);
void print_map_tiles(m_map* map);
void print_map(m_map* map);
m_map create_map(int capacity);
void free_map(m_map* array);
char empty_map(m_map* array);
int get_size_map(m_map* array);
int get_capacity_map(m_map* array);
void resize_array_map(m_map* array, int new_capacity);
void push_back_map(m_map* array, m_maplayer* value);
m_maplayer pop_back_map(m_map* array);
m_maplayer* get_element_map(m_map* array, int index);
void set_element_map(m_map* array, int index, m_maplayer value);
m_map readmapp(const char* path, long offset);
void writemapp(m_map* map, const char* path);
void writemapex(m_map* map, const char* path, const char* mode);
m_map readmapex(const char* path);
void writecache(cac_cache_a* c, const char* path, long cacheoffset);
void writenpcaex(n_npc_a* c, const char* path, long npcoffset);
void seekin_push_map_seek();
char load_mapload(int i);
char tile_load_map(float dx, float dy);
void tile_load_map_ts(float* x, float* y, m_map* map, const char* path);
m_map default_map(int l, int w, int h, int fill);
m_texttile_a create_TextTileA(int capacity);
void free_TextTile(m_texttile_a* array);
void resize_array_TextTile(m_texttile_a* array, int new_capacity);
void push_back_TextTile(m_texttile_a* array, m_texttile* value);

char collision_map_char(const ch_char* ch);
char collision_map(m_maplayer* map, int x, int y);

m_map initjm(int w, int h, int ll);
void joinm(m_map* buf, m_map* m, int xo, int yo);
void linkmap();
void joinmv();
void joinmv_f();

void adjust_map();
void rotjoinmap(char dir0, char dir1, m_map m[3]);

int mapfsize(m_map* map);

void collision_full();

extern char (*tile_load_map_spec)(m_map* m, int loadidx);

char ch_in_combat();