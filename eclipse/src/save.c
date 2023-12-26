#include "save.h"

#include "dglobal.h"

#include <stdio.h>

ch_char ch;
unsigned char  cache_top = 0;
unsigned char estore_top = 0;
unsigned char cstore_top = 0;
icac_itemcache cache[ICACHE_MAX_SIZE];
e_element estore[ELEM_STORE_MAX];
icac_itemcache cstore[ITEM_STORE_MAX];

m_mapseek_a loadstack;

short xsavs = 0, ysavs = 0;
int32_t seeksavs = -1;
short slensavs = 0;

int32_t mapseek = 0;

char um_name_text[UM_MAX_MAP_NAME_TEXT_SIZE * UM_MAX_MAP] = "";

char um_map_index = 0;

int fncd[] = { 0, 0, };

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

void writesed_npos(const char* path)
{
	FILE* f;
	f = fopen(path, "rb+");

	fseek(f, 0
		+ sizeof(mapseek) * 1
		+ sizeof(ch.sprite.x) * 1
		+ sizeof(ch.sprite.y) * 1
		+ sizeof(ch.sprite.z) * 1
		, SEEK_SET);

	fwrite(&(ch.spra), sizeof(ch.spra), 1, f);
	fwrite(&(ch.sprb), sizeof(ch.sprb), 1, f);

	fwrite(&(ch.defcast), sizeof(ch.defcast), 1, f);

	fwrite(cache, sizeof(*cache), ICACHE_MAX_SIZE, f);
	fwrite(&cache_top, sizeof(cache_top), 1, f);

	fwrite(estore, sizeof(*estore), ELEM_STORE_MAX, f);
	fwrite(&estore_top, sizeof(estore_top), 1, f);

	fwrite(ch.elem, sizeof(*ch.elem), ELEM_CACHE_MAX, f);
	fwrite(&ch.elemidx, sizeof(ch.elemidx), 1, f);

	fwrite(cstore, sizeof(*cstore), ITEM_STORE_MAX, f);
	fwrite(&cstore_top, sizeof(cstore_top), 1, f);

	fwrite(&xsavs, sizeof(xsavs), 1, f);
	fwrite(&ysavs, sizeof(ysavs), 1, f);
	fwrite(&seeksavs, sizeof(seeksavs), 1, f);
	fwrite(&slensavs, sizeof(slensavs), 1, f);

	fwrite(&ms_mapstate.cidx, sizeof(ms_mapstate.cidx), 1, f);
	fwrite(&ms_mapstate.clen, sizeof(ms_mapstate.clen), 1, f);
	fwrite(&ms_mapstate.crarfidx, sizeof(ms_mapstate.crarfidx), 1, f);

	fwrite(&ms_mapstate.eidx, sizeof(ms_mapstate.eidx), 1, f);
	fwrite(&ms_mapstate.elen, sizeof(ms_mapstate.elen), 1, f);
	fwrite(&ms_mapstate.erarfidx, sizeof(ms_mapstate.erarfidx), 1, f);

	fwrite(um_name_text, sizeof(*um_name_text), sizeof(um_name_text) / sizeof(*um_name_text), f);

	fclose(f);
}

void writesed(const char* path)
{
	FILE* f;
	f = fopen(path, "wb");

	fwrite(&(mapseek), sizeof(mapseek), 1, f);

	fwrite(&(ch.sprite.x), sizeof(ch.sprite.x), 1, f);
	fwrite(&(ch.sprite.y), sizeof(ch.sprite.y), 1, f);
	fwrite(&(ch.sprite.z), sizeof(ch.sprite.z), 1, f);
	fwrite(&(ch.spra), sizeof(ch.spra), 1, f);
	fwrite(&(ch.sprb), sizeof(ch.sprb), 1, f);

	fwrite(&(ch.defcast), sizeof(ch.defcast), 1, f);

	fwrite(cache, sizeof(*cache), ICACHE_MAX_SIZE, f);
	fwrite(&cache_top, sizeof(cache_top), 1, f);

	fwrite(estore, sizeof(*estore), ELEM_STORE_MAX, f);
	fwrite(&estore_top, sizeof(estore_top), 1, f);

	fwrite(ch.elem, sizeof(*ch.elem), ELEM_CACHE_MAX, f);
	fwrite(&ch.elemidx, sizeof(ch.elemidx), 1, f);

	fwrite(cstore, sizeof(*cstore), ITEM_STORE_MAX, f);
	fwrite(&cstore_top, sizeof(cstore_top), 1, f);

	fwrite(&xsavs, sizeof(xsavs), 1, f);
	fwrite(&ysavs, sizeof(ysavs), 1, f);
	fwrite(&seeksavs, sizeof(seeksavs), 1, f);
	fwrite(&slensavs, sizeof(slensavs), 1, f);

	fwrite(&ms_mapstate.cidx, sizeof(ms_mapstate.cidx), 1, f);
	fwrite(&ms_mapstate.clen, sizeof(ms_mapstate.clen), 1, f);
	fwrite(&ms_mapstate.crarfidx, sizeof(ms_mapstate.crarfidx), 1, f);

	fwrite(&ms_mapstate.eidx, sizeof(ms_mapstate.eidx), 1, f);
	fwrite(&ms_mapstate.elen, sizeof(ms_mapstate.elen), 1, f);
	fwrite(&ms_mapstate.erarfidx, sizeof(ms_mapstate.erarfidx), 1, f);

	fwrite(um_name_text, sizeof(*um_name_text), sizeof(um_name_text) / sizeof(*um_name_text), f);

	fwrite(fncd, sizeof(*fncd), sizeof(fncd) / sizeof(*fncd), f);

	fclose(f);
}

void readsed(const char* path)
{
	FILE* f;
	f = fopen(path, "rb");

	fread(&(mapseek), sizeof(mapseek), 1, f);

	fread(&(ch.sprite.x), sizeof(ch.sprite.x), 1, f);
	fread(&(ch.sprite.y), sizeof(ch.sprite.y), 1, f);
	fread(&(ch.sprite.z), sizeof(ch.sprite.z), 1, f);
	fread(&(ch.spra), sizeof(ch.spra), 1, f);
	fread(&(ch.sprb), sizeof(ch.sprb), 1, f);

	fread(&(ch.defcast), sizeof(ch.defcast), 1, f);

	fread(cache, sizeof(*cache), ICACHE_MAX_SIZE, f);
	fread(&cache_top, sizeof(cache_top), 1, f);

	fread(estore, sizeof(*estore), ELEM_STORE_MAX, f);
	fread(&estore_top, sizeof(estore_top), 1, f);

	fread(ch.elem, sizeof(*ch.elem), ELEM_CACHE_MAX, f);
	fread(&ch.elemidx, sizeof(ch.elemidx), 1, f);

	fread(cstore, sizeof(*cstore), ITEM_STORE_MAX, f);
	fread(&cstore_top, sizeof(cstore_top), 1, f);

	fread(&xsavs, sizeof(xsavs), 1, f);
	fread(&ysavs, sizeof(ysavs), 1, f);
	fread(&seeksavs, sizeof(seeksavs), 1, f);
	fread(&slensavs, sizeof(slensavs), 1, f);

	fread(&ms_mapstate.cidx, sizeof(ms_mapstate.cidx), 1, f);
	fread(&ms_mapstate.clen, sizeof(ms_mapstate.clen), 1, f);
	fread(&ms_mapstate.crarfidx, sizeof(ms_mapstate.crarfidx), 1, f);

	fread(&ms_mapstate.eidx, sizeof(ms_mapstate.eidx), 1, f);
	fread(&ms_mapstate.elen, sizeof(ms_mapstate.elen), 1, f);
	fread(&ms_mapstate.erarfidx, sizeof(ms_mapstate.erarfidx), 1, f);

	fread(um_name_text, sizeof(*um_name_text), sizeof(um_name_text) / sizeof(*um_name_text), f);

	fread(fncd, sizeof(*fncd), sizeof(fncd) / sizeof(*fncd), f);

	fclose(f);

	ch.dx = ch.sprite.x;
	ch.dy = ch.sprite.y;
}

void clearsed()
{
	memset(&ch, 0, sizeof(ch));
	memset(cache, 0, sizeof(cache));
	memset(estore, 0, sizeof(estore));
	memset(cstore, 0, sizeof(cstore));
	memset(&ms_mapstate, 0, sizeof(ms_mapstate));
	memset(ch.elem, -1, sizeof(ch.elem));

	ch.sprite.x = 18;
	ch.sprite.y = 20;
	ch.elemidx = -1;
	ch.lifeb = ch.lifea = CH_MAX_LIFE;

	{
		short buf[] = { +0, -1, -1, -1, };
		memcpy(ch.defcast, buf, sizeof(buf));
	}

	mapseek = 0;
	cache_top = 0;

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