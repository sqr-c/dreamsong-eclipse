#pragma once

#include <stdint.h>

#include "map.h"

#define ID_BWROCK		(SPRITE_TILE_OFFSET + 18)
#define	ID_WHROCK		(SPRITE_TILE_OFFSET + 19)
#define ID_HEAVDIRT		(SPRITE_TILE_OFFSET + 13)
#define ID_PYRBOT		(SPRITE_TILE_OFFSET + 7)
#define ID_PYRTOP		(SPRITE_TILE_OFFSET + 8)
#define ID_ROCKSOIL		(SPRITE_TILE_OFFSET + 51)
#define ID_SHINY		(SPRITE_TILE_OFFSET + 10)
#define ID_MINIROCK		(SPRITE_TILE_OFFSET + 5)

#define DM_LENGTH	(16+1)

#define DM_CAC_MUL	(4)
#define DM_NPC_MUL	(1)


typedef struct
{
	char p[4];
} dm_dpath;

typedef struct
{
	int32_t seek;
	int x, y;
} seekxy;

typedef struct
{
	dm_dpath path;
	char pathl;

	short msrchm;
	short esrchm;
	short npcm; //TODO: obsoleted, remove this from struct
	short smsrchm;

	short cacrand;
	short npcrand;

	short endtick;

	seekxy seek;

	int elemi;
	int eleml;

	int breaktilei;
	int breaktilel;

	int cachei;
	int cachel;

	int npci;
	int npcl;
} dm_dmapmd;

char tile_floor_exists(m_map* dmbufmap, int xx, int yy, int id);
char tile_exists(m_map* dmbufmap, int xx, int yy);
char dm_place_msrch_0(m_map* dmbufmap, int x, int y);
char dm_place_msrch_1(m_map* dmbufmap, int x, int y);
char dm_place_esrch(m_map* dmbufmap, int x, int y, int w, char o, int l);
char dm_place_path_0(m_map* dmbufmap, int x, int y, char o);
char dm_place_path_1(m_map* dmbufmap, int x, int y, char o, int l);
char dm_place_path_2(m_map* dmbufmap, int x, int y, char o, int l, char m);
char dm_place_break(m_map* dmbufmap, int x, int y, int w, int h, char m);
char dm_place_wall(m_map* dmbufmap, int x, int y, char o, int l);
char dm_place_floor(m_map* dmbufmap);
void dm_place_end(m_map* dmbufmap);
char dm_gen_std_wall(m_map* dmbufmap, char mode, int lo, int xo, int yo, char oo);
char dm_gen_db_wall(m_map* dmbufmap, char mode, int xo);
char dm_gen_std_path(m_map* dmbufmap, char mode);
char dm_gen_std_break(m_map* dmbufmap, char mode, char m);
char dm_gen_multi_path(m_map* dmbufmap, char mode, dm_dpath* r, dm_dpath* rr, char m);
char dm_gen_msrch(m_map* dmbufmap, char mode);
char dm_gen_esrch(m_map* dmbufmap, char mode, int esrchl);
void dm_generate(m_map* dmbufmap, dm_dmapmd* dmmd, uint32_t seed);

void place_cache(m_map* m, cac_cache* c);
void place_break(m_map* m, int x, int y, bt_breaktile* bt);

extern bt_breaktile* dmbreaktemplate[];
extern int dmbreaktempi;

extern cac_cache_min* dmcachetemplate[];
extern int dmcachetempi;
extern int dmcacrand;

extern n_npc_min* dmnpctemplate[];
extern int dmnpctempi;
extern int dmnpcrand;

extern e_element* dmelemtemplate[];
extern int dmelemtempi;

#define BREAK_TEMP_LEN	(sizeof(dmbreaktemplate) / sizeof(bt_breaktile*))
#define CACHE_TEMP_LEN	(sizeof(dmcachetemplate) / sizeof(icac_itemcache*))
#define ELEM_TEMP_LEN	(sizeof(dmelemtemplate) / sizeof(e_element*))
#define NPC_TEMP_LEN	(sizeof(dmnpctemplate) / sizeof(n_npc_min*))