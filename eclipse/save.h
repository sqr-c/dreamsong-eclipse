#pragma once

#include "character.h"
#include "itemcache.h"
#include "maploader.h"

#define ELEM_STORE_MAX	50
#define ITEM_STORE_MAX	(32+16)

extern m_mapseek_a loadstack;

extern short xsavs, ysavs;
extern long seeksavs;
extern short slensavs;

extern int estore_top;
extern e_element estore[ELEM_STORE_MAX];
extern icac_itemcache cache[ICACHE_MAX_SIZE];
extern int cache_top;
extern ch_char ch;
extern int cstore_top;
extern icac_itemcache cstore[ITEM_STORE_MAX];

extern long mapseek;

extern char screen_mode;
extern void(*renderfn)();
extern int gtidx;

#define UM_MAX_MAP	8
#define UM_MAX_MAP_NAME_TEXT_SIZE	16
extern char um_name_text[UM_MAX_MAP_NAME_TEXT_SIZE * UM_MAX_MAP];

extern char um_map_index;

char exists(const char* path);
void writesed(const char* path);
void readsed(const char* path);
void clearsed();
void readlas(const char* path);
void writelas(const char* path, m_mapseek_a* arr);
void transfer_evt(char ofs);

void render();