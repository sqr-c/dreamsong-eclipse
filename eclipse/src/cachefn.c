#include "cachefn.h"

#define CFN_NULL_FN_ID	7

#include "dynamicmap.h"

void rm_cac_a(cac_cache_a* arr, int idx)
{
	arr->arr[idx] = arr->arr[--arr->size];
}

void set_gtext_itemfound(int iid)
{
	gtidx = ITEM_FOUND_TXT_IDX;
	screen_mode = SCREEN_MODE_TEXT;

	t_aidx = 0;
	t_bidx = 0;

	for (int i = 0; i < itexta[iid].size; i++)
		gtexta[gtidx].arr[i + gtexta->size - CACHE_FOUND_SIZE + 1] =
		itexta[iid].arr[i];

	gtexta[gtidx].size = CACHE_FOUND_TEXT_SIZE + itexta[iid].size + 1;

	gtexta[gtidx].arr[gtexta[gtidx].size - 1] = &(sprdata[chtotid('!') + SPRITE_TEXT_OFFSET]);

	ch_action_pause();
}

char cache_std_ex(cac_cache* c)
{
	set_gtext_itemfound(c->iid);

	ch.sprb += c->spr;
	ch.spra += c->spr;

	for (int i = 0; i < cache_top; i++)
		if (cache[i].id == c->iid)
		{
			cache[i].mul++;
			return 1;
		}

	cache[cache_top].id = c->iid;
	cache[cache_top].mul = 1;
	cache_top++;

	c->cd = 0;

	return 1;
}

char add_cache(icac_itemcache* cac, unsigned char* top, int l, icac_itemcache* ic)
{
	for (int i = 0; i < *top; i++)
		if (cac[i].id == ic->id)
		{
			cac[i].mul += ic->mul;
			return 1;
		}

	if (*top < l)
	{
		cac[(*top)++] = *ic;
		return 1;
	}

	return 0;
}

char cache_iswp0_ex(cac_cache* c)
{
	char flag = 0;
	int i;

	for (i = 0; i < cache_top; i++)
		if (cache[i].id == 2 && cache[i].active)
		{
			flag = 1;
			break;
		}

	if (!flag)
	{
		gtidx = 0;
		screen_mode = SCREEN_MODE_TEXT;

		t_aidx = 0;
		t_bidx = 0;

		return 0;
	}

	set_gtext_itemfound(c->iid);

	cache[i].mul--;
	if (cache[i].mul == 0)
		for (; i < cache_top; i++)
		{
			xswap(cache[i].id, cache[i + 1].id);
			xswap(cache[i].mul, cache[i + 1].mul);
			cache_top--;
		}

	ch.sprb += c->spr;
	ch.spra += c->spr;

	for (int i = 0; i < cache_top; i++)
		if (cache[i].id == c->iid)
		{
			cache[i].mul++;
			return 1;
		}

	cache[cache_top].id = c->iid;
	cache[cache_top].mul++;
	cache_top++;

	c->cd = time(NULL) + (TU(.5f));

	return 1;
}

char cache_prm(cac_cache* c)
{
	c->cd = -1;

	ch.sprb += c->spr;
	ch.spra += c->spr;

	if (c->iid != -1)
	{
		icac_itemcache v;
		v.id = c->iid;
		v.active = 0;
		v.mul = 1;

		add_cache(
			cache,
			&cache_top,
			ICACHE_MAX_SIZE,
			&v
		);

		set_gtext_itemfound(c->iid);
	}

	return 1;
}

char cache_std_rm(cac_cache* c)
{
	c->cd = time(0) + TU(1);

	icac_itemcache v;
	v.active = 0;
	v.id = c->iid;
	v.mul = 1;

	ch.sprb += c->spr;
	ch.spra += c->spr;

	add_cache(
		cache,
		&cache_top,
		ICACHE_MAX_SIZE,
		&v
	);

	set_gtext_itemfound(c->iid);

	rm_cac_a(&map.cache, c - map.cache.arr);

	return 1;
}

char cache_tdrop(cac_cache* c)
{
	s_sprite* buf = &map.arr[1].tile
		.arr[p2idx(c->x, c->y, map.arr[1].width)]
		.arr[0];

	buf->id = TILE_SHINY_ID;
	buf->flag = SPRITE_FLAG_DROP;

	return 0;
}

char cache_gem0(cac_cache* c)
{
	readcid(PATH_CID, c);

	if (c->cd == -1)
		return !1;
	else if (c->cd == -2)
	{
		c->cd = -1;

		writecid(PATH_CID, c);

		{
			icac_itemcache buf;
			buf.id = c->iid;
			buf.mul = 1;
			buf.active = 0;
			add_cache(cache, &cache_top, ICACHE_MAX_SIZE, &buf);

			set_gtext_itemfound(buf.id);
		}

		return 0;
	}
	else c->cd = -2;

	writecid(PATH_CID, c);

	int i;
	for (i = 0; i < cache_top; i++)
		if (cache[i]
			.id == ICAC_GEM_RED_ID + 0
			|| cache[i]
			.id == ICAC_GEM_RED_ID + 1
			|| cache[i]
			.id == ICAC_GEM_RED_ID + 2)
			break;

	if (i >= cache_top)
		return cache_tdrop(c);
	else return 1;

	for (i = 0; i < cstore_top; i++)
		if (cstore[i]
			.id == ICAC_GEM_RED_ID + 0
			|| cstore[i]
			.id == ICAC_GEM_RED_ID + 1
			|| cstore[i]
			.id == ICAC_GEM_RED_ID + 2)
			break;

	if (i >= cstore_top)
		return cache_tdrop(c);
	else return 1;
}

void dm_overwrite_cache(cac_cache* c)
{
	fclose(mapfp);
	mapfp = NULL;

	m_map buf = readmapp(PATH_VAR_MAP, mapseek);

	int i;
	for (i = 0; i < buf.cache.size; i++)
		if (buf.cache.arr[i].x + defr == c->x && buf.cache.arr[i].y + defr == c->y)
			break;

	buf.cache.arr[i].fnid = CFN_NULL_FN_ID;

	fclose(mapfp);
	mapfp = fopen(PATH_VAR_MAP, "rb+");
	fseek(mapfp, mapseek, SEEK_SET);

	writemapp(&buf, PATH_VAR_MAP);
}

char cache_dm_std_find(cac_cache* c)
{
	set_gtext_itemfound(c->iid);

	ch.sprb += c->spr;
	ch.spra += c->spr;

	icac_itemcache cc;
	cc.active = 0;
	cc.id = c->iid;
	cc.mul = 1;

	add_cache(cache, &cache_top, ICACHE_MAX_SIZE, &cc);
	dm_overwrite_cache(c);

	return 1;
}

char cache_null_fn(cac_cache* c)
{
	return 1;
}

char cache_find_elem_dm(cac_cache* c)
{
	if (-1 != add_elem(&ch,
		dmelemtemplate[dmelemtempi]
	))
	{
		disp_elem_idx = dmelemtemplate[dmelemtempi]->id;
		screen_mode = SCREEN_MODE_DISP_ELEM;

		dm_overwrite_cache(c);

		return 1;
	}
	return 0;
}

#include "save.h"

char cache_dm_std_mat_find(cac_cache* c)
{
#define STD_MAT_FNCD0		((1*60) * 60 * 24)
#define STD_MAT_FNCD1_MAX	(16)

	if (time(0) - fncd[0] >= STD_MAT_FNCD0
		|| fncd[1] < STD_MAT_FNCD1_MAX)
	{
		fncd[0] = time(0) - fncd[0];
		fncd[1]++;

		cache_dm_std_find(c);
	}
}

char cache_find_pub_elem(cac_cache* c)
{
	add_elem(&ch, pub_elem[c->iid]);

	ch.spra += c->spr;
	ch.sprb += c->spr;

	rm_cac_a(&map.cache, c - map.cache.arr);

	return 0;
}

char cache_pushrm_elem(cac_cache* c)
{
	e_element e;
	//CID	ELEM

	//7		PERIL CAVE
	if (c->cid == 7)
		e =
		(e_element)
	{
		38,
		0, 0,
		{ 43, 44, 59, -1, },
	};

	add_elem(&ch, &e);

	disp_elem_idx = e.id;
	screen_mode = SCREEN_MODE_DISP_ELEM;

	c->cd = -1;

	writecid(PATH_CID, c);

	return 1;
}

#include "park.h"

/*
*ID		CFN
*2		perm rm
*3		shiny drop
*4		rm
*5		gem 0
*6		dm, std find
*7		null function
*11		dm, std mat find
*/

char (*cfn[NUM_CACHE_FN])(cac_cache* c) =
{
	&cache_std_ex, &cache_iswp0_ex, &cache_prm, &cache_tdrop, &cache_std_rm, &cache_gem0, &cache_dm_std_find, &cache_null_fn,
	&cache_find_elem_dm, &cache_find_pub_elem, &cache_pushrm_elem, &cache_dm_std_mat_find, //0, 0, 0,
	//0, 0, 0, 0, 0, 0, 0, 0,
};