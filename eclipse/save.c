#include "save.h"

#include "dglobal.h"

#include <stdio.h>

ch_char ch;
int cache_top = 0;
int estore_top = 4;
int cstore_top = 4;
icac_itemcache cache[ICACHE_MAX_SIZE];
e_element estore[ELEM_STORE_MAX];
icac_itemcache cstore[ITEM_STORE_MAX];

m_mapseek_a loadstack;

short xsavs, ysavs;
long seeksavs = -1;
short slensavs;

long mapseek = 0;

char um_name_text[UM_MAX_MAP_NAME_TEXT_SIZE * UM_MAX_MAP] = "";

char um_map_index = 0;

void readlas(const char* path)
{
	m_mapseek_a buf;

	FILE* fp;

	fp = fopen(path, "rb");

	if (fp == NULL)
	{
		loadstack = create_mapseek(1);
		writelas(path, &loadstack);
		return;
	}

	fread(
		&buf.size,
		sizeof(int),
		1,
		fp
	);

	buf.arr = (m_mapseek*)malloc(sizeof(m_mapseek) * (buf.capacity = (1 + buf.size)));
	fread(
		buf.arr,
		sizeof(m_mapseek),
		buf.size,
		fp
	);

	fclose(fp);

	loadstack = buf;
}

void writelas(const char* path, m_mapseek_a* arr)
{
	FILE* fp;

	fp = fopen(path, "wb");

	fwrite(
		&arr->size,
		sizeof(int),
		1,
		fp
	);
	fwrite(
		arr->arr,
		sizeof(m_mapseek),
		arr->size,
		fp
	);

	fclose(fp);
}

char exists(const char* path)
{
	FILE* f;
	f = fopen(path, "rb");
	if (f != NULL) fclose(f);
	return f != NULL;
}

void writesed(const char* path)
{
	FILE* f;
	f = fopen(path, "wb");

	fwrite(&(ch.sprite.x), sizeof(float), 1, f);
	fwrite(&(ch.sprite.y), sizeof(float), 1, f);
	fwrite(&(ch.sprite.z), sizeof(float), 1, f);
	fwrite(&(ch.spra), sizeof(int), 1, f);
	fwrite(&(ch.sprb), sizeof(int), 1, f);

	fwrite(&(mapseek), sizeof(long), 1, f);

	fwrite(&(cache), sizeof(icac_itemcache), ICACHE_MAX_SIZE, f);
	fwrite(&(cache_top), sizeof(int), 1, f);

	fwrite(&estore, sizeof(e_element), ELEM_STORE_MAX, f);
	fwrite(&estore_top, sizeof(int), 1, f);

	fwrite(&(ch.elem), sizeof(e_element), ELEM_CACHE_MAX, f);
	fwrite(&(ch.elemidx), sizeof(char), 1, f);

	fwrite(cstore, sizeof(icac_itemcache), ITEM_STORE_MAX, f);
	fwrite(&cstore_top, sizeof(int), 1, f);

	fwrite(&xsavs, sizeof(short), 1, f);
	fwrite(&ysavs, sizeof(short), 1, f);
	fwrite(&seeksavs, sizeof(long), 1, f);
	fwrite(&slensavs, sizeof(short), 1, f);

	fwrite(&ms_mapstate.cidx, sizeof(unsigned short), 1, f);
	fwrite(&ms_mapstate.clen, sizeof(unsigned short), 1, f);
	fwrite(&ms_mapstate.crarfidx, sizeof(unsigned short), 1, f);

	fwrite(&ms_mapstate.eidx, sizeof(unsigned short), 1, f);
	fwrite(&ms_mapstate.elen, sizeof(unsigned short), 1, f);
	fwrite(&ms_mapstate.erarfidx, sizeof(unsigned char), 1, f);

	fwrite(um_name_text, sizeof(char), sizeof(um_name_text) / sizeof(char), f);

	fclose(f);
}

void readsed(const char* path)
{
	FILE* f;
	f = fopen(path, "rb");

	fread(&(ch.sprite.x), sizeof(float), 1, f);
	fread(&(ch.sprite.y), sizeof(float), 1, f);
	fread(&(ch.sprite.z), sizeof(float), 1, f);
	fread(&(ch.spra), sizeof(int), 1, f);
	fread(&(ch.sprb), sizeof(int), 1, f);

	fread(&(mapseek), sizeof(long), 1, f);

	fread(&(cache), sizeof(icac_itemcache), ICACHE_MAX_SIZE, f);
	fread(&(cache_top), sizeof(int), 1, f);

	fread(&estore, sizeof(e_element), ELEM_STORE_MAX, f);
	fread(&estore_top, sizeof(int), 1, f);

	fread(&(ch.elem), sizeof(e_element), ELEM_CACHE_MAX, f);
	fread(&(ch.elemidx), sizeof(char), 1, f);

	fread(cstore, sizeof(icac_itemcache), ITEM_STORE_MAX, f);
	fread(&cstore_top, sizeof(int), 1, f);

	fread(&xsavs, sizeof(short), 1, f);
	fread(&ysavs, sizeof(short), 1, f);
	fread(&seeksavs, sizeof(long), 1, f);
	fread(&slensavs, sizeof(short), 1, f);

	fread(&ms_mapstate.cidx, sizeof(unsigned short), 1, f);
	fread(&ms_mapstate.clen, sizeof(unsigned short), 1, f);
	fread(&ms_mapstate.crarfidx, sizeof(unsigned short), 1, f);

	fread(&ms_mapstate.eidx, sizeof(unsigned short), 1, f);
	fread(&ms_mapstate.elen, sizeof(unsigned short), 1, f);
	fread(&ms_mapstate.erarfidx, sizeof(unsigned char), 1, f);

	fread(um_name_text, sizeof(char), sizeof(um_name_text) / sizeof(char), f);

	fclose(f);

	ch.dx = ch.sprite.x;
	ch.dy = ch.sprite.y;
}

void clearsed()
{
	ch.sprite.x = ch.sprite.y = ch.sprite.z = 0;
	ch.sprb = ch.spra = 0;

	mapseek = 0;

	cache_top = -1;

	writesed(PATH_SED);
}

void transfer_evt(char ofs)
{
#define TRANSFER_ERR_TXT_IDX	(53 - 38 + ofs)
	renderfn = &render;
	gtidx = TRANSFER_ERR_TXT_IDX;
	screen_mode = SCREEN_MODE_TEXT;
	t_aidx = 0;
	t_bidx = 0;

	menu_sel_idx_0 = 0;
	menu_sel_idx_1 = -1;
	menu_sel_idx_2 = -1;
}