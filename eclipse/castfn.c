#include "castfn.h"

#include "map.h"
#include "dynamicmap.h"

#include <SDL.h>
#include <time.h>

#define TICK_INT_UNIT	100

#define SPRITE_DUST_ID_0	711
#define SPRITE_DUST_ID_1	714
#define SPRITE_DUST_ID_2	717

#define CD_RET(fncd)											\
if (set_cooldown(c, cd, c->ch == &ch, (fncd))) return 1;		\
else if (c->ch->spra <= 0) return 1;							\

#define CD_RET_EX(fncd, gcd)										\
if (set_cooldown_ex(c, cd, c->ch == &ch, (fncd), (gcd))) return 1;	\
else if (c->ch->spra <= 0) return 1;								\

#define CD_DEC_SPR(fncd, fnspr)									\
CD_RET(fncd)													\
dec_spra(c, (fnspr));											\

#define CAS_CD_DEC_FN_0(c, x)	( (c) - (((c) / (4 * 1)) * (x)) )
#define CAS_SPR_DEC_FN_0(s, x)	( (s) - (((s) / (4 * 2)) * (x)) )

#define CAS_BREAK_ADD_FN_0(s, x)	((s) + ((ELEM_SPR_MAX / BREAKM_MAX)* x))

#define CAS_DSP_ADD_FN_0(d, x)	((d) + ((x) / 4.0f) * d)


void dec_spra(c_cast* c, int m)
{
	c->ch->spra -= m;
	if ((int)c->ch->spra < 0) c->ch->spra = 0;
}

void get_ixiy(int* x, int* y, ch_char* c)
{
	*x =
		c->sprite.x
		+
		((c->rot + 1) % 2)
		*
		+1 * ((c->rot / 2)
			? -1
			: +1);

	*y =
		c->sprite.y
		+
		((c->rot) % 2)
		*
		+1 * ((c->rot / 2)
			? -1
			: +1);
}

void get_dixiy(int* x, int* y, ch_char* c)
{
	*x = (c->rot % 2);
	*y = ((1 + c->rot) % 2);
}

void apply_cast(short* cast, int idx, c_cast* v, ch_char* c)
{
	if (cast[idx] != -1)
	{
		castfn[cast[idx]](v);
	}
	else
		c->acttick = c->gcool =
		+500
		+ tik
		;

	set_state(c, c->rot, 2);
	c->sprite.deltatime = tik;
}

void update_acttick(ch_char* ch)
{
	if (ch->state == 2 && ch->acttick <= tik)
		set_state(ch, ch->rot, 0);
}

void itfn_apply_icas()
{
	for (int i = 0; i < intcasts.size; i++)
		if (tik >=
			intcasts
			.arr[i]
			.t_rm)
			rm_IntCast(&intcasts, i);
		else if (0 ==
			(tik / TICK_INT_UNIT) %
			intcasts
			.arr[i]
			.tick)
			intcasts
			.arr[i]
			.fn(&intcasts.arr[i].fnargs);
}

intc_intcast_a create_IntCastA(int capacity)
{
	intc_intcast_a buf;
	buf.arr = (intc_intcast*)malloc(capacity * sizeof(intc_intcast));
	buf.size = 0;
	buf.capacity = capacity;
	return buf;
}

void free_IntCast(intc_intcast_a* array)
{
	free(array->arr);
}

void resize_array_IntCast(intc_intcast_a* array, int new_capacity)
{
	intc_intcast* new_arr = (intc_intcast*)malloc(new_capacity * sizeof(intc_intcast));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_IntCast(intc_intcast_a* array, intc_intcast* value)
{
	if (array->size == array->capacity)
		resize_array_IntCast(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}

void rm_IntCast(intc_intcast_a* array, int idx)
{
	array->arr[idx] =
		array->arr[
			array->size - 1
		];
	array->size--;
}

char set_cooldown_ex(c_cast* c, int* castcd, int cidx, int t, int gcool)
{
	//set cooldown
	c->ch->gcool =
		+tik + gcool;
	gcdflag = 1;

	//act anim cd
	c->ch->acttick = tik + 250 * 3;


	if (castcd[cidx] > (int)tik) { gcdflag = 1; return 1; }

	castcd[cidx] = t + tik;
	return 0;
}

char set_cooldown(c_cast* c, int* castcd, int cidx, int t)
{
	//set cooldown
	c->ch->gcool =
		+tik + t;
	gcdflag = 1;

	//act anim cd
	c->ch->acttick = tik + 4 * t;

	if (castcd[cidx] > (int)tik) { gcdflag = 1; return 1; }
	return 0;
}

char setcascd(int* castcd, c_cast* c, int gcool, int actcool)
{
	int cidx = c->ch != &ch;

	if (set_cooldown(c, castcd, cidx, gcool)) return 1;
	else if (c->ch->spra <= 0) { gcdflag = 1; return 1; }

	//set cd for cast[i]
	castcd[cidx] = tik + actcool;

	//act anim cd
	c->ch->acttick = castcd[cidx];

	return 0;
}

char stdcast(c_cast* c, int a)
{
	if (c->ech != NULL)
	{
		if (c->ech != &ch && ((n_npc*)c->ech)->istate != NPC_STATE_ENEMY)
			return 0;

		c->ech->lifea -= ((a / BREAKM_MAX) * MAX_LIFE);

		return 1;
	}

	//get the sprite at (ix, iy)
	unsigned int idx = p2idx(c->ix, c->iy, map.arr[1].width);
	if (idx >= map.arr[1].tile.size)
		return 0;

	s_sprite* buf = &(map.arr[1].tile.arr[idx].arr[0]);
	char rmbuf = 1;

	//if its cache
	if (buf->flag == SPRITE_FLAG_CACHE)
	{
		for (int i = 0; i < map.cache.size; i++)
		{
			if ((map.cache.arr[i].cd != -1)
				&& (map.cache.arr[i].cd < time(NULL))
				&& (map.cache.arr[i].x == c->ix && map.cache.arr[i].y == c->iy))
			{
				rmbuf = cfn[map.cache.arr[i].fnid](&(map.cache.arr[i]));

				writecid(PATH_CID, &(map.cache.arr[i]));
			}
		}
	}
breakall:
	if (rmbuf && buf->flag == SPRITE_FLAG_CACHE)
	{
		buf->id = -1;
		buf->flag = 0;
	}
	//if its a break tile
	else if (buf->flag == SPRITE_FLAG_BREAK)
	{
		int atbti = p2idx(c->ix, c->iy, map.arr[1].width);
		//get the break tile
		bt_breaktile* atbt = &(map.breaks.arr[atbti]);

		//call its function
		{
			cast_break_value = a;
			char buf;
			if (1 == (buf = btfn[atbt->breakfnid](atbt)))
				return buf;
		}

		//if its unbroken, add to the bthmupdate
		if (atbt->breakma > 0 && (atbt->breakma != atbt->breakmb))
		{
			int i;
			for (i = 0; i < bthmupdate.size; i++)
				if (bthmupdate.arr[i] == atbti)
					break;
			if (i >= bthmupdate.size)
				push_back_bthmupdatea(&bthmupdate, atbti);
		}

		//if its unbroken
		if (atbt->breakma <= 0)
		{
			buf->id = -1;
			buf->flag = 0;
		}
	}

	return 0;
}

void add_dust(float x, float y, float z, int id, int time)
{
	if (!(x < 0 || y < 0 || x >= map.arr[1].width || y >= map.arr[1].height))
	{
		tsc_tscreenobj buf;

		buf.cd = time;

		buf.v.x = x;
		buf.v.y = y;
		buf.v.z = z;
		buf.v.flag = 0;
		buf.v.flip = 0;
		buf.v.id = id;

		buf.pos = p2idx(buf.v.x, buf.v.y, map.arr[1].width);

		push_back_TScreenObj(&tsoa, &buf);
		apply_TScreenObj(&buf);
	}
}

char cast0(c_cast* c)
{
	//set cooldown
	c->ch->gcool = tik + 500;
	if (c->ch == &ch)
		gcdflag = 1;

	//act anim cd
	c->ch->acttick = tik + 500;

	return stdcast(c, BREAKM_BLOCK / 4);
}

char cast1(c_cast* c)
{
	static int castcd = 0;

	//set cooldown
	c->ch->gcool = tik + 100;
	gcdflag = 1;

	//act anim cd
	ch.acttick = tik + 500;

	if (castcd > tik) { gcdflag = 1; return 1; }
	else if (c->ch->spra <= 0) { gcdflag = 1; return 1; }

	c->ch->spra -= SPR_BLOCK * 1.0f / 16;
	stdcast(c, (c->ch->sprb / (int)SPR_BLOCK) * BREAKM_BLOCK);

	//set cd for cast[i]
	castcd = tik + 500;

	return 0;
}

char cast_fume_wave(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(250, SPR_BLOCK / (8));

	//add icast
	intc_intcast v;
	v.fn = &icas_fume_wave;
	v.fnargs = *c;
	v.tick = 10;
	v.t_rm = tik + v.tick * 4 * TICK_INT_UNIT;

	push_back_IntCast(
		&intcasts,
		&v
	);

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_1, v.tick * TICK_INT_UNIT * 4);

	return 0;
}

char cast_flare(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(250, SPR_BLOCK / (8 + 2));

	stdcast(c, BREAKM_BLOCK / (1.0f));


	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 250);

	return 0;
}

char cast_surge(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(250, SPR_BLOCK / (8 + 2));

	stdcast(c, BREAKM_BLOCK * (1.5f));

	//add icast
	intc_intcast v;
	v.fn = &icas_surge;
	v.fnargs = *c;
	v.tick = 10;
	v.t_rm = tik + v.tick * 4 * TICK_INT_UNIT;

	push_back_IntCast(
		&intcasts,
		&v
	);

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_2, tik + 750 * 3);
	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 750 * 1);

	return 0;
}

char icas_fume_wave(c_cast* c)
{
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	stdcast(c, CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * .125f, esprl));

	if (0 == xorrand() % (8 - ((esprl >= 1) * 2)))
	{
		c_cast buf;
		buf.ch = c->ch;
		buf.ech = c->ch->target;
		buf.ix = c->ix + (((xorrand() % 2) * 2) - 1);
		buf.iy = c->iy + (((xorrand() % 2) * 2) - 1);

		cast_fume_wave(&buf);
	}

	return 0;
}

char icas_surge(c_cast* c)
{
	stdcast(c, BREAKM_BLOCK / 4);

	if (0 == xorrand() % 8)
	{
		c->ch->spra += SPR_BLOCK / 8;
		if (c->ch->spra > c->ch->sprb) c->ch->spra = c->ch->sprb;
	}

	return 0;
}


char cast_shred(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(250, SPR_BLOCK / (8 + 2));

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 250);

	stdcast(c, BREAKM_BLOCK / (2.0f));

	return 0;
}

char digmode = 0;
void dm_apply_dig()
{
	static seekxy seekxy;

	static dm_dmapmd buf =
	{
		{0,0,0,0,},
		2,

		1,1,1,1,

		1,1,

		-1
		,

		{243072L,0,0},

		1,1,
		0,1,
		0,1,
		0,1,
	};

	if (digmode)
	{
		dm_dmmd = search_DMTable(&dmtable, mapseek, ch.sprite.x - defr, ch.sprite.y - defr);
		if (dm_dmmd == NULL)
			dm_dmmd = dm_dmmd_def = &buf;

		seekxy.seek = mapseek;
		seekxy.x = ch.sprite.x;
		seekxy.y = ch.sprite.y;

		//temp code v
		static int foo = 0;

		mapseek = 0;

		fnv1a_hash_str(
			//temp code v
			(char*)&foo, sizeof(int),
			//real code v
			//(char*)&seekxy, sizeof(SeekXY),
			PATH_VAR_DMD + DM_PATH_STR_OFFSET);

		//temp code v
		remove(PATH_VAR_DMD);

		PATH_VAR_MAP = PATH_VAR_DMD;

		if (!exists(PATH_VAR_DMD))
		{
			fclose(mapfp); mapfp = NULL;
			init_dm();
		}
		else
		{
			free_map(&map);
			fclose(mapfp); mapfp = NULL;
			map = readmapp(PATH_VAR_DMD, 0);
			linkmap();
		}

		ch.dx = ch.sprite.x = DM_LENGTH / 2 + defr;
		ch.dy = ch.sprite.y = DM_LENGTH / 2 + defr;
	}
	else
	{
		//remove file
		fclose(mapfp); mapfp = NULL;
		remove(PATH_VAR_DMD);

		seekxy = dm_dmmd->seek;

		PATH_VAR_MAP = PATH_SMD;

		free_map(&map);
		if (mapfp != NULL) { fclose(mapfp); mapfp = NULL; }
		map = readmapp(PATH_VAR_MAP, mapseek = seekxy.seek);
		linkmap();

		ch.dx = ch.sprite.x = seekxy.x;
		ch.dy = ch.sprite.y = seekxy.y;
	}
}

char cast_dig(c_cast* c)
{
	c->ch->acttick = tik + 1000;
	c->ch->gcool = tik + 1000;

	if (ch_in_combat()) return 1;

	if (!digmode || map.arr[0].tile.arr[p2idx(ch.sprite.x, ch.sprite.y, map.arr[0].width)].arr[0].flag == SPRITE_FLAG_DMOUT)
	{
		digmode++; digmode %= 2;

		screen_mode = SCREEN_MODE_DISP_TRS;
		trs_end_fn = &dm_apply_dig;
	}

	bthmupdate.size = 0;

	return 0;
}

int casttik0 = 0;

char icas_disperse(c_cast* c)
{
	int m = 6;

	if (casttik0 < m / 2)
		for (int i = 0; i < 2; i++)
			add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + TICK_INT_UNIT * 2);
	else
	{
		if (casttik0 == m / 2)
			stdcast(c, BREAKM_BLOCK * 1);
		add_dust(c->ix, c->iy, .125f, SPRITE_DUST_ID_0, tik + TICK_INT_UNIT * 2);
	}

	if (casttik0 >= m) { casttik0 = 0; }
	casttik0++;

	return 0;
}

char cast_disperse(c_cast* c)
{
	static int cd[2] = { 0, 0, };

	int ccdidx = c->ch != &ch;

	CD_DEC_SPR(250, SPR_BLOCK / (8 + 2));

	casttik0 = 0;

	//add icast
	intc_intcast v;
	v.fn = &icas_disperse;
	v.fnargs = *c;
	v.tick = 4;
	v.t_rm = tik + v.tick * 6 * TICK_INT_UNIT;

	push_back_IntCast(
		&intcasts,
		&v
	);

	icas_disperse(c);

	return 0;
}

char cast_return(c_cast* c)
{
	static int cd[2] = { 0, 0, };
	int cidx = c->ch != &ch;

	CD_DEC_SPR(250, SPR_BLOCK / (8 + 2));

	int* inc = &((&c->ix)[(1 + c->ch->rot) % 2]);
	int buf = *inc;
	for (int i = -1; i <= 1; i++)
	{
		*inc = buf + i;

		stdcast(c, BREAKM_BLOCK * 1.5f);

		add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 250);
	}

	return 0;
}

char cast_dsp_illuminate(c_cast* c)
{
	static int cd[2] = { 0, 0, };
	int cidx = c->ch != &ch;

	CD_RET(100);

	disps[cidx]
		.dspbrk = SPR_BLOCK / (8 + 2);
	disps[cidx]
		.dsplif = SPR_BLOCK / (16 + 2);

	//add dust
	add_dust(c->ch->sprite.x, c->ch->sprite.y, 1.5f, 714 + 0 * 3, tik + 250 * 4);

	return 0;
}

char cast_gloomyhaze(c_cast* c)
{
	static int cd[2] = { 0, 0, };

	CD_DEC_SPR(500, SPR_BLOCK / 8);

	//add dust
	{
		add_dust(c->ix, c->iy, 1.5f, 714 + +1 * 3, tik + 250 * 4);
		add_dust(c->ix, c->iy, 0.5f, 714 + +0 * 3, tik + 250 * 4);
		add_dust(c->ix, c->iy, 0.0f, 714 + -1 * 3, tik + 250 * 1);
	}

	//1 cast
	stdcast(c,
		BREAKM_BLOCK * 2.0f);

	//push back icas, cast icas
	intc_intcast v;
	v.fn = &icas_gloomyhaze;
	v.fnargs = *c;
	v.tick = 8;
	v.t_rm = tik + v.tick * 8 * TICK_INT_UNIT;

	push_back_IntCast(
		&intcasts,
		&v
	);

	return 0;
}

char icas_gloomyhaze(c_cast* c)
{
	static unsigned char state = 0;

	add_dust(c->ix, c->iy, 1.5f - .75f * (state), 714 + (-1) * 3, tik + 250 * 4);

	state++;
	state %= 3;

	stdcast(c, 1 * BREAKM_BLOCK * 1.5f);

	return 0;
}

char cast_suspension(c_cast* c)
{
	static int cd[2] = { 0, 0, };
	int cidx = c->ch != &ch;

	CD_DEC_SPR(250, SPR_BLOCK / 8);

	c->ix += xorrand() % 3 - 1;
	c->iy += xorrand() % 3 - 1;

	add_dust(c->ix, c->iy, 0, 711, tik + 250 * 1);

	stdcast(c, BREAKM_BLOCK * 1.0f);

	return 0;
}

char cast_pursue(c_cast* c)
{
	static int cd = 0;

	c->ch->gcool = c->ch->acttick = tik + 500;

	if (cd > tik || c->ch->spra != c->ch->sprb)
		return 1;
	cd = tik + 1000;

	if (seeksavs == -1)
	{
		for (int i = 0; i < sizeof(map.adj) / sizeof(*map.adj); i++)
			if (map.adj[i] != -1)
				goto endfn;

		seeksavs = mapseek;
		xsavs = ch.sprite.x;
		ysavs = ch.sprite.y;
		slensavs = loadstack.size;
	}
	else
	{
		mapseek = seeksavs;
		ch.dx = ch.sprite.x = xsavs;
		ch.dy = ch.sprite.y = ysavs;
		loadstack.size = slensavs;

		free_map(&map);
		map = readmapp(PATH_VAR_MAP, mapseek);
		linkmap();

		for (int i = 0; i < loadstack.size; i++)
			if (loadstack.arr[i].seekin == mapseek)
				push_back_mapload(
					&map.loads,
					&loadstack.arr[i].load
				);

		seeksavs = -1;
	}

endfn:
	c->ch->spra = 0;
	return 0;
}

char cast_stonefall(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	CD_DEC_SPR(CAS_CD_DEC_FN_0(500, esprl), CAS_SPR_DEC_FN_0((SPR_BLOCK / 8), esprl));

	if (esprl >= 1)
	{
		for (int i = 0; i < 3; i++)
			add_dust(c->ix, c->iy, i * .5f, SPRITE_DUST_ID_2, tik + 100 * exp(i));
	}

	for (int i = 0; i < 3; i++)
		add_dust(c->ix, c->iy, i * .5f, SPRITE_DUST_ID_0, tik + 100 * exp(i));

	stdcast(c, CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * 2, esprl));

	return 0;
}

char cast_ancwis(c_cast* c)
{
	static int cd[2];

	char cidx = c->ch != &ch;

	CD_RET(250);

	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++)
			if (!(x == 0 && y == 0))
			{
				char m = abs(x) == 1 ^ abs(y) == 1;

				add_dust(
					c->ch->sprite.x + x, c->ch->sprite.y + y, 0, SPRITE_DUST_ID_0 + 6 - 3 * m, tik + 500);
			}

	disps[cidx]
		.dspbrk = SPR_BLOCK / 2;
	disps[cidx]
		.dsplif = SPR_BLOCK / 4;

	return 0;
}

char cast_cascade(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	CD_DEC_SPR(500, SPR_BLOCK / 8);

	add_dust(c->ix, c->iy, .75f * 0, SPRITE_DUST_ID_0 + 0, tik + 500 * 2);
	add_dust(c->ix, c->iy, .75f * 1, SPRITE_DUST_ID_1, tik + 500);
	add_dust(c->ix, c->iy, .75f * 2, SPRITE_DUST_ID_2, tik + 500);

	stdcast(c, CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * 1.5f, esprl));

	return 0;
}

char cast_sspore(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	char cidx = c->ch == &ch;

	CD_RET(500);

	for (int i = 0; i < 3; i++)
		add_dust(c->ch->sprite.x, c->ch->sprite.y, i * .5f, SPRITE_DUST_ID_0 + 3 * 1, tik + 500);
	add_dust(c->ch->sprite.x, c->ch->sprite.y, 3 * .5f, SPRITE_DUST_ID_0 + 3 * 2, tik + 500);

	disps[cidx]
		.dspbrk = CAS_DSP_ADD_FN_0(SPR_BLOCK / 2, esprl);
	disps[cidx]
		.dsplif = CAS_DSP_ADD_FN_0(SPR_BLOCK / 4, esprl);

	return 0;
}

char cast_tangle(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(500, SPR_BLOCK / 6);

	short mx = c->ix - c->ch->sprite.x;
	short my = c->iy - c->ch->sprite.y;

	add_dust(c->ix + mx * 0, c->iy + my * 0, 0, SPRITE_DUST_ID_0, tik + 500);
	add_dust(c->ix + mx * 1, c->iy + my * 1, 0, SPRITE_DUST_ID_0 + 3, tik + 500);

	stdcast(c, BREAKM_BLOCK * 1.5f);
	c->ix += mx;
	c->iy += my;
	stdcast(c, BREAKM_BLOCK * 1.0f);

	return 0;
}

char cast_growth(c_cast* c)
{
	static int cd[2];

	CD_RET(250);

	char cidx = c->ch == &ch;

	add_dust(c->ch->sprite.x, c->ch->sprite.y, 1.5f, SPRITE_DUST_ID_0 + 3, tik + 250);

	disps[cidx]
		.dspbrk = SPR_BLOCK / 4;
	disps[cidx]
		.dsplif = SPR_BLOCK / 8;

	return 0;
}

char cast_diffusion(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(500, SPR_BLOCK / 6);

	short mx = c->ix - c->ch->sprite.x;
	short my = c->iy - c->ch->sprite.y;

	add_dust(c->ix + mx * 0, c->iy + my * 0, 0, SPRITE_DUST_ID_0, tik + 500);
	stdcast(c, BREAKM_BLOCK * 1.5f);
	if (xorrand() % 2)
	{
		c->ix += mx;
		c->iy += my;
		add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0 + 3, tik + 500);
		stdcast(c, BREAKM_BLOCK * 1.0f);
	}

	return 0;
}

char cast_resonance(c_cast* c)
{
	return cast_growth(c);
}

char cast_debug_rm(c_cast* c)
{
	return stdcast(c, BREAKM_BLOCK * 14);
}

char cast_enigma_song(c_cast* c)
{
	static int cd[2];
	char cidx = c->ch == &ch;
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	CD_DEC_SPR(CAS_CD_DEC_FN_0(500, esprl), CAS_SPR_DEC_FN_0((SPR_BLOCK / (8 + 2)), esprl));

	static char a[] = { 0,0 };

	if (esprl == 1)
		a[cidx] ^= 1;

	for (int i = (-1 - a[cidx]); i != (+1 + a[cidx]) + 1; i++)
	{
		int dx;
		int dy;

		get_dixiy(&dx, &dy, c->ch);

		char ds = -((c->ch->rot / 2) * 2 - 1);

		int x0 = c->ix + i * dx + 0 * dy * ds;
		int y0 = c->iy + i * dy + 0 * dx * ds;
		int x1 = c->ix + i * dx + 1 * dy * ds;
		int y1 = c->iy + i * dy + 1 * dx * ds;

		int tx = c->ix;
		int ty = c->iy;
		{
			c->ix = x0;
			c->iy = y0;

			add_dust(x0, y0, 0, SPRITE_DUST_ID_0, tik + 250);
			stdcast(c, BREAKM_BLOCK * 1);
		}
		{
			c->ix = x1;
			c->iy = y1;

			add_dust(x1, y1, 0, SPRITE_DUST_ID_0 + 3 * 2, tik + 500);
			stdcast(c, BREAKM_BLOCK * 1);
		}

		c->ix = tx;
		c->iy = ty;
	}

	return 0;
}

char cast_eternal_mirage(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	CD_RET(250);

	char cidx = c->ch == &ch;

	disps[cidx]
		.dspbrk = CAS_DSP_ADD_FN_0(SPR_BLOCK / 4, esprl);
	disps[cidx]
		.dsplif = CAS_DSP_ADD_FN_0(SPR_BLOCK / 8, esprl);

	{
		for (char x = -1; x <= 1; x++)
			for (char y = -1; y <= 1; y++)
			{
				if (x == 0 && y == 0) continue;

				unsigned char mod = ((x + y) % 2);

				if (esprl >= 1)
				{
					add_dust(
						c->ch->sprite.x
						+ x,
						c->ch->sprite.y
						+ y,
						.25f,
						SPRITE_DUST_ID_2, tik + 250 / (!mod + 1));
				}

				add_dust(
					c->ch->sprite.x
					+ x,
					c->ch->sprite.y
					+ y,
					0,
					SPRITE_DUST_ID_0, tik + 500);

				if (mod)
				{
					add_dust(c->ch->sprite.x
						+ x, c->ch->sprite.y
						+ y, 1.0f, SPRITE_DUST_ID_0, tik + 500);
					add_dust(c->ch->sprite.x
						+ x, c->ch->sprite.y
						+ y, 1.5f, SPRITE_DUST_ID_0 + 3 * 2, tik + 500);
				}
			}
	}

	return 0;
}

char cast_flurry(c_cast* c)
{
	static int cd[2];

	CD_RET(500);

	for (char i = 0; i < 3; i++)
		add_dust(c->ch->sprite.x, c->ch->sprite.y, 1.75f / 3 * i, SPRITE_DUST_ID_0 + 3 * 1, tik + 500);

	char cidx = c->ch == &ch;

	disps[cidx]
		.dspbrk = SPR_BLOCK / 8;
	disps[cidx]
		.dsplif = SPR_BLOCK / 16;

	return 0;
}

char cast_debug(c_cast* c)
{
	int d = 8;

	int dx = ROTDIR((ch.rot + 1) % 4);
	int dy = ROTDIR((ch.rot) % 4);

	int i;
	for (i = 0; i < d; i++)
	{
		int ddx = ch.dx + dx * i;
		int ddy = ch.dy + dy * i;

		if (map.arr[1].tile.arr
			[p2idx(ddx, ddy, map.arr[1].width)]
			.arr[0].flag != 0
			)
			goto endloop;

		for (int i = 0; i < map.npca.size; i++)
			if (map.npca.arr[i].ch.sprite.x == ddx && map.npca.arr[i].ch.sprite.y == ddy)
				goto endloop;
	}
endloop:
	i--;

	ch.dx = ch.sprite.x += dx * i;
	ch.dy = ch.sprite.y += dy * i;

	push_back_IterFNA(&cyfn, &rdr_flsh_scr);

	return 0;
}

char cast_sunder_cast(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(500, SPR_BLOCK / (8 + 2));

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 500);
	stdcast(c, BREAKM_BLOCK * 1);

	//push back icas, cast icas
	intc_intcast v;
	v.fn = &icas_sunder_cast;
	v.fnargs = *c;
	v.tick = 4;
	v.t_rm = tik + v.tick * 4 * TICK_INT_UNIT;

	push_back_IntCast(
		&intcasts,
		&v
	);

	return 0;
}

char icas_sunder_cast(c_cast* c)
{
	add_dust(c->ix, c->iy, 0.5f, SPRITE_DUST_ID_0, tik + 500);

	stdcast(c, BREAKM_BLOCK / 4);

	return 0;
}

char icas_torrent(c_cast* c)
{
	static unsigned char mod = 0;

	add_dust(c->ix, c->iy, (mod++ % 2) * .5f + .5f, SPRITE_DUST_ID_0 + 3 * 0, tik + 500 / 2);

	stdcast(c, BREAKM_BLOCK / 8);

	return 0;
}

char cast_torrent(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(250, SPR_BLOCK / (8 + 2));

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0 + 3 * 2, tik + 3 * 500);
	stdcast(c, BREAKM_BLOCK / 2);

	intc_intcast v;
	v.fn = &icas_torrent;
	v.fnargs = *c;
	v.tick = 2;
	v.t_rm = tik + v.tick * 8 * TICK_INT_UNIT;

	push_back_IntCast(
		&intcasts,
		&v
	);

	return 0;
}

char cast_tidal_surge(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(250, SPR_BLOCK / (8 + 2));

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0 + 3 * 0, tik + 3 * 500);
	stdcast(c, BREAKM_BLOCK / 2);

	static unsigned char mod[2] = { 0,0 };

	if (0 == ((mod[c->ch == &ch]++) % 2))
	{
		int x, y;
		get_dixiy(&x, &y, c->ch);

		for (int i = 0; i < 2; i++)
		{
			int tx = c->ix;
			int ty = c->iy;

			c->ix = c->ix + x * (i * 2 - 1);
			c->iy = c->iy + y * (i * 2 - 1);

			add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0 + 3 * 0, tik + 3 * 500);
			stdcast(c, (BREAKM_BLOCK / 2) / 1);

			c->ix = tx;
			c->iy = ty;
		}
	}

	return 0;
}

char cast_ancient_mirage(c_cast* c)
{
	static int cd[2];
	char cidx = c->ch == &ch;

	CD_RET(125);

	disps[cidx]
		.dspbrk = SPR_BLOCK / 8;
	disps[cidx]
		.dsplif = SPR_BLOCK / 16;

	for (char i = 0; i < 3; i++)
		add_dust(c->ch->sprite.x, c->ch->sprite.y, i * .5f, SPRITE_DUST_ID_0 + 3 * 1, tik + 250);

	return 0;
}

char cast_cyclone(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(250, SPR_BLOCK / 8);

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 500);
	add_dust(c->ix, c->iy, .75f, SPRITE_DUST_ID_0 + 3 * 2, tik + 500);
	stdcast(c, BREAKM_BLOCK / 2);

	return 0;
}

char cast_sonic_wave(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(250, SPR_BLOCK / 8);

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 500);
	stdcast(c, BREAKM_BLOCK / (2 + 1));

	static unsigned char mod[2] = { 0,0 };

	if (0 == ((mod[c->ch == &ch]++) % 2))
	{
		int x, y;
		get_dixiy(&x, &y, c->ch);

		for (int i = 0; i < 2; i++)
		{
			int tx = c->ix;
			int ty = c->iy;

			c->ix = c->ix + x * (i * 2 - 1);
			c->iy = c->iy + y * (i * 2 - 1);

			add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0 + 3 * 1, tik + 3 * 500);
			stdcast(c, (BREAKM_BLOCK / 2) / 1);

			c->ix = tx;
			c->iy = ty;
		}
	}

	return 0;
}

char cast_beryl_song(c_cast* c)
{
	static int cd[2];
	char cidx = c->ch == &ch;

	CD_RET(125);

	for (char i = 0; i < 2; i++)
		add_dust(c->ch->sprite.x, c->ch->sprite.y, i * (1.75f / 2), SPRITE_DUST_ID_0 + 3 * (i + 1), tik + 250);

	disps[cidx]
		.dspbrk = SPR_BLOCK / 10;
	disps[cidx]
		.dsplif = SPR_BLOCK / 18;

	return 0;
}

char cast_radiant_crest(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(250, SPR_BLOCK / 9);

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0 + 3, tik + 500);
	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0 + 3 * 2, tik + 500);

	stdcast(c, BREAKM_BLOCK / 2);

	return 0;
}

char cast_thicket(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(250, SPR_BLOCK / 8);

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 500);
	stdcast(c, BREAKM_BLOCK / (2 + 1));

	static unsigned char mod[2] = { 0,0 };

	if (0 == ((mod[c->ch == &ch]++) % 2))
	{
		int x, y;
		get_dixiy(&x, &y, c->ch);

		for (int i = 0; i < 2; i++)
		{
			int tx = c->ix;
			int ty = c->iy;

			c->ix = c->ix + x * (i * 2 - 1);
			c->iy = c->iy + y * (i * 2 - 1);

			add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0 + 3 * 1, tik + 3 * 500);
			stdcast(c, (BREAKM_BLOCK / 2) * 2);

			c->ix = tx;
			c->iy = ty;
		}
	}

	return 0;
}

char cast_leaf_dance(c_cast* c)
{
	static int cd[2];
	char cidx = c->ch == &ch;

	CD_RET(125);

	for (char i = 0; i < 3; i++)
		add_dust(c->ch->sprite.x, c->ch->sprite.y, i * (1.75f / 3), SPRITE_DUST_ID_0 + 3 * 1, tik + 250);

	disps[cidx]
		.dspbrk = SPR_BLOCK / 12;
	disps[cidx]
		.dsplif = SPR_BLOCK / 20;

	return 0;
}

char cast_pin_whirl(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(250, SPR_BLOCK / 9);

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0 + 3 * 0, tik + 500);
	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0 + 3 * 1, tik + 500);

	stdcast(c, BREAKM_BLOCK / (2 + 1));

	return 0;
}

char cast_whirl(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(250, SPR_BLOCK / 8);

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 500);
	stdcast(c, BREAKM_BLOCK / (2 + 1));

	static unsigned char mod[2] = { 0,0 };

	if (!((++mod[c->ch == &ch]) % 3))
	{
		int x, y;
		get_dixiy(&x, &y, c->ch);

		for (int i = 0; i < 2; i++)
		{
			int tx = c->ix;
			int ty = c->iy;

			c->ix = c->ix + x * (i * 2 - 1);
			c->iy = c->iy + y * (i * 2 - 1);

			add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 3 * 500);
			stdcast(c, (BREAKM_BLOCK / 2) * 3);

			c->ix = tx;
			c->iy = ty;
		}
	}

	return 0;
}

char cast_bell_song(c_cast* c)
{
	static int cd[2];
	char cidx = c->ch == &ch;

	CD_RET(125);

	add_dust(c->ch->sprite.x, c->ch->sprite.y, 0, SPRITE_DUST_ID_0 + 3 * 1, tik + 250);

	disps[cidx]
		.dspbrk = SPR_BLOCK / (12 + 1);
	disps[cidx]
		.dsplif = SPR_BLOCK / (20 + 1);

	return 0;
}

char cast_hidden_power(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(250, SPR_BLOCK / (8 + 2));

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 500);
	add_dust(c->ix, c->iy, .25f, SPRITE_DUST_ID_0 + 3 * 2, tik + 500);

	stdcast(c, BREAKM_BLOCK / (1.5f));

	return 0;
}

char cast_downrush(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(250, SPR_BLOCK / 8);

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 500);
	stdcast(c, BREAKM_BLOCK / 2);

	int x, y;
	get_dixiy(&x, &y, c->ch);

	for (int i = 0; i < 2; i++)
	{
		int tx = c->ix;
		int ty = c->iy;

		c->ix = c->ix + x * (i * 2 - 1);
		c->iy = c->iy + y * (i * 2 - 1);

		add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 3 * 500);
		stdcast(c, (BREAKM_BLOCK / 2));

		c->ix = tx;
		c->iy = ty;
	}

	return 0;
}

char cast_cryptic_symbol(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;
	char cidx = c->ch == &ch;

	CD_RET(125);

	if (esprl >= 1)
		for (int i = 0; i < 2; i++)
			add_dust(c->ch->sprite.x, c->ch->sprite.y, i * (1.75f / 2), SPRITE_DUST_ID_2, tik + 250 / 2);

	for (int i = 0; i < 3; i++)
		add_dust(c->ch->sprite.x, c->ch->sprite.y, i * (1.75f / 3), SPRITE_DUST_ID_1, tik + 250);

	disps[cidx]
		.dspbrk = CAS_DSP_ADD_FN_0(SPR_BLOCK / (8 + 2), esprl);
	disps[cidx]
		.dsplif = CAS_DSP_ADD_FN_0(SPR_BLOCK / (12 + 2), esprl);

	return 0;
}

char cast_roll(c_cast* c)
{
	static int cd[2];

	CD_DEC_SPR(250, SPR_BLOCK / (8 + 2));

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 500);
	add_dust(c->ix, c->iy, .25f, SPRITE_DUST_ID_0, tik + 500);

	stdcast(c, BREAKM_BLOCK / (2.0f));

	return 0;
}

char cast_harmonic_cast(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	CD_DEC_SPR(CAS_CD_DEC_FN_0(1000, esprl), CAS_SPR_DEC_FN_0((SPR_BLOCK / (8 + 6)), esprl));

	add_dust(c->ix, c->iy, 0,
		SPRITE_DUST_ID_0,
		tik + 1000);

	if (esprl >= 1)
		for (int i = 0; i < 2; i++)
			add_dust(c->ix, c->iy,
				i * .75f + .75f,
				SPRITE_DUST_ID_2,
				tik + 125);

	for (int i = 0; i < 2; i++)
		add_dust(c->ix, c->iy,
			i * .5f + .25f,
			SPRITE_DUST_ID_1,
			tik + 500);

	stdcast(c,
		CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * 2, esprl)
	);

	return 0;
}

char cast_radiant_surge(c_cast* c)
{
	static int cd[2];
	CD_DEC_SPR(1000, SPR_BLOCK / (8 + 6));

	int x, y;
	get_dixiy(&x, &y, c->ch);

	for (int i = -1; i <= +1; i++)
	{
		int tx = c->ix;
		int ty = c->iy;

		c->ix = c->ix + x * i;
		c->iy = c->iy + y * i;

		add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 250);
		stdcast(c, BREAKM_BLOCK);

		c->ix = tx;
		c->iy = ty;
	}

	return 0;
}

char cast_dream_barrier(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;
	char cidx = c->ch == &ch;

	CD_RET(1000);

	disps[cidx]
		.dspbrk = CAS_DSP_ADD_FN_0(SPR_BLOCK / (8 + 2), esprl);
	disps[cidx]
		.dsplif = CAS_DSP_ADD_FN_0(SPR_BLOCK / (12 + 2), esprl);

	float x = c->ch->sprite.x;
	float y = c->ch->sprite.y;

	short id[] = { SPRITE_DUST_ID_0, SPRITE_DUST_ID_1, };

	for (int a = 0; a < 2; a++)
		for (int b = 0; b < 2; b++)
		{
			for (int i = 0; i < 1 + (esprl >= 1); i++)
				add_dust(0
					+ x
					+ ((b * 2) - 1)
					* ((a + 0) % 2)
					, 0
					+ y
					+ ((b * 2) - 1)
					* ((a + 1) % 2)
					, 0, id[i], tik + 250);
		}

	for (int i = 0; i < 3; i++)
		for (int ii = 0; ii < 1 + (esprl >= 1); ii++)
			add_dust(x, y, i * (1.75f / 3) * ((1 + ii) * 0.5f), id[1 - ii], tik + 250 * 2);

	return 0;
}

char cast_echo(c_cast* c)
{
	static int cd[2];
	char cidx = c->ch == &ch;

	//do not add macro to replace v
	if (set_cooldown(c, cd, cidx, 100)) return 0;
	cd[cidx] = tik + 1024 * 64;

	c->ch->spra = c->ch->sprb;

	float x = c->ch->sprite.x;
	float y = c->ch->sprite.y;

	for (int a = -1; a <= +1; a++)
		for (int b = -1; b <= +1; b++)
			add_dust(
				x + a,
				y + b,
				0,
				SPRITE_DUST_ID_1,
				tik + 500);

	return 0;
}

char cast_moon_surge(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;
	CD_DEC_SPR(500, SPR_BLOCK / 8);

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 1000);
	add_dust(c->ix, c->iy, .25f, SPRITE_DUST_ID_2, tik + 250);

	if (esprl >= 1)
	{
		add_dust(c->ix, c->iy, 1.25f, SPRITE_DUST_ID_2, tik + 250);
	}

	stdcast(c, BREAKM_BLOCK * 2);

	return 0;
}

char cast_frigid_grove(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;
	char cidx = c->ch == &ch;
	CD_DEC_SPR(625, SPR_BLOCK / (8 + 6));

	int x, y;
	get_dixiy(&x, &y, c->ch);

	static unsigned char mod[2] = { 0, 0, };
	++mod[cidx];
	mod[cidx] %= (2 + esprl);

	for (int i = -1; i <= +1; i++)
	{
		int tx = c->ix;
		int ty = c->iy;

		c->ix = c->ix + x * i;
		c->iy = c->iy + y * i;

		add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 250);

		stdcast(c, CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * 1, esprl));

		//c->ix = tx;
		//c->iy = ty;

		if (mod[cidx])
		{
			c->ix = c->ch->sprite.x + x * i + -2 * y * (-1 + 2 * (ch.rot / 2));
			c->iy = c->ch->sprite.y + y * i + -2 * x * (-1 + 2 * (ch.rot / 2));

			add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 250);
			stdcast(c, CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * 1, esprl));

			//c->ix = tx;
			//c->iy = ty;
		}

		c->ix = tx;
		c->iy = ty;
	}

	return 0;
}

char cast_moon_aurora(c_cast* c)
{
	static int cd[2];
	char cidx = c->ch == &ch;

	CD_RET(1000);

	disps[cidx]
		.dspbrk = SPR_BLOCK / (8 + 2);
	disps[cidx]
		.dsplif = SPR_BLOCK / 16;

	float x = c->ch->sprite.x;
	float y = c->ch->sprite.y;

	for (int a = 0; a < 2; a++)
		for (int b = 0; b < 2; b++)
			add_dust(x + ((b * 2) - 1) * ((a + 0) % 2)
				, y + ((b * 2) - 1) * ((a + 1) % 2)
				, 0, SPRITE_DUST_ID_1 - 1, tik + 250 * 2);

	add_dust(x, y, 1.75f, SPRITE_DUST_ID_2, tik + 250 * 3);

	return 0;
}

char cast_focus_stream(c_cast* c)
{
	static int cd[2];
	CD_DEC_SPR(500, SPR_BLOCK / 8);

	for (int i = 0; i < 4; i++)
		add_dust(c->ix, c->iy, i * .25f, SPRITE_DUST_ID_0, tik + 500 - i * (500 / 4));

	stdcast(c, BREAKM_BLOCK * 2);

	return 0;
}

char cast_focus_wind(c_cast* c)
{
	static int cd[2];
	char cidx = c->ch == &ch;

	CD_RET(250);

	disps[cidx]
		.dspbrk = SPR_BLOCK / (8 + 4);
	disps[cidx]
		.dsplif = SPR_BLOCK / (16 + 4);

	for (int i = 0; i < 3; i++)
	{
		add_dust(c->ch->sprite.x, c->ch->sprite.y, i * (1.75f / 3), SPRITE_DUST_ID_1, tik + 500 - i * (1000 / 3));
		add_dust(c->ch->sprite.x, c->ch->sprite.y, i * (1.75f / 3), SPRITE_DUST_ID_0, tik + 500 - i * (500 / 3));
	}

	return 0;
}

char icas_harmonic_tone(c_cast* c)
{
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	int id[] = { SPRITE_DUST_ID_0, SPRITE_DUST_ID_2, };

	for (int ii = 0; ii < 1 + (esprl >= 1); ii++)
		for (int i = 0; i < 4; i++)
			add_dust(c->ix, c->iy, i * .25f * ((1.25f * ii) + 1), id[i % 2], tik + 500);

	stdcast(c, CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * 1.5f, esprl));

	return 0;
}

char cast_harmonic_tone(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	CD_DEC_SPR(CAS_CD_DEC_FN_0(500, esprl), CAS_SPR_DEC_FN_0((SPR_BLOCK / (8 + 4)), esprl));

	int id[] = { SPRITE_DUST_ID_0, SPRITE_DUST_ID_2, };

	for (int ii = 0; ii < 1 + (esprl >= 1); ii++)
		for (int i = -2; i <= 2; i++)
		{
			if ((i + 0) % 4 == 0)
				continue;

			char xx = (i + 0) % 2,
				yy = (i + 1) % 2;

			int x = c->ix + xx,
				y = c->iy + yy;

			if ((x == (int)c->ch->sprite.x)
				&&
				(y == (int)c->ch->sprite.y))
				continue;

			add_dust(x, y, ii * .5f, id[ii], tik + 125);
		}


	stdcast(c, CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * .25f, esprl));

	//add icast
	intc_intcast v;
	v.fn = &icas_harmonic_tone;
	v.fnargs = *c;
	v.tick = 16 * 2;
	v.t_rm = tik + v.tick * 1 * TICK_INT_UNIT;

	push_back_IntCast(
		&intcasts,
		&v
	);

	return 0;
}

char cast_focus_rush(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	CD_DEC_SPR(CAS_CD_DEC_FN_0(250, esprl), CAS_SPR_DEC_FN_0((SPR_BLOCK / (8 + 2)), esprl));

	int x, y;
	get_dixiy(&x, &y, c->ch);

	char o = esprl < 1;

	short id[] = { SPRITE_DUST_ID_0, SPRITE_DUST_ID_2, };

	for (int ii = 0; ii < 1 + !o; ii++)
		for (int i = -1 + o; i <= +1 - o; i++)
		{
			int tx = c->ix;
			int ty = c->iy;

			c->ix = c->ix + x * i;
			c->iy = c->iy + y * i;

			add_dust(c->ix, c->iy, 0, id[ii], tik + 250);
			stdcast(c, CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * 1.25f, esprl));

			c->ix = tx;
			c->iy = ty;
		}

	return 0;
}

char cast_static_affinity(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;
	char cidx = c->ch == &ch;

	CD_RET(500);

	disps[cidx]
		.dspbrk = CAS_DSP_ADD_FN_0(SPR_BLOCK / (8), esprl);
	disps[cidx]
		.dsplif = CAS_DSP_ADD_FN_0(SPR_BLOCK / (12), esprl);

	int cx = c->ch->sprite.x;
	int cy = c->ch->sprite.y;

	short id[] = { SPRITE_DUST_ID_0, SPRITE_DUST_ID_2, SPRITE_DUST_ID_1, };

	for (int i = 0; i < 3; i++)
		add_dust(cx, cy,
			((float)i / 3) * (1.75f)
			, id[i % (sizeof(id) / sizeof(*id))], tik + 500);

	if (esprl >= 1)
		for (int i = -2; i <= +2; i++)
		{
			if (i == 0) continue;

			char xx =
				(i + 0) % 2;
			char yy =
				(i + 1) % 2;

			int x = cx + xx;
			int y = cy + yy;

			add_dust(x, y, 0, SPRITE_DUST_ID_0, tik + 500);
		}

	return 0;
}

char cast_swift_focus(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;
	CD_DEC_SPR(625, SPR_BLOCK / (8 + 6));

	int x, y;
	get_dixiy(&x, &y, c->ch);

	for (int i = -1; i <= +1; i++)
	{
		int tx = c->ix;
		int ty = c->iy;

		c->ix = c->ix + x * i;
		c->iy = c->iy + y * i;

		add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 250);
		if (esprl >= 1)
			add_dust(c->ix, c->iy, .25f, SPRITE_DUST_ID_2, tik + 250);

		stdcast(c, CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * 1.5f, esprl));

		c->ix = tx;
		c->iy = ty;
	}

	return 0;
}

char cast_swift_pulse(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;
	CD_DEC_SPR(625, SPR_BLOCK / (8 + 8));

	for (int i = 0; i < 2; i++)
		add_dust(c->ix, c->iy, (.25f + ((esprl >= 1) / 8.0f)) * (2 * i + 0), SPRITE_DUST_ID_0, tik + 250);

	if (esprl >= 1)
		for (int i = 0; i < 2; i++)
			add_dust(c->ix, c->iy, (.25f + ((esprl >= 1) / 8.0f)) * (2 * i + 1), SPRITE_DUST_ID_2, tik + 250);

	stdcast(c, CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * 2.0f, esprl));

	return 0;
}

char cast_generative_pulse(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;
	char cidx = c->ch == &ch;
	CD_DEC_SPR(500, SPR_BLOCK / (8 + 8));

	int x, y;
	get_dixiy(&x, &y, c->ch);

	static unsigned char mod[2] = { 0, 0, };

	for (int i = -1; i <= +1; i++)
	{
		int tx = c->ix;
		int ty = c->iy;

		if (i == 0 && ++mod[cidx] % 2)
		{
			char s = ((ch.rot / 2) * 2 - 1);
			add_dust(c->ix, c->iy, .25f, SPRITE_DUST_ID_2, tik + 250);

			int spr = c->ch->spra;
			cast_swift_pulse(c);
			c->ch->spra = spr;

			continue;
		}

		c->ix = c->ix + x * i;
		c->iy = c->iy + y * i;

		add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 250);
		if (esprl >= 1)
			add_dust(c->ix, c->iy, .25f, SPRITE_DUST_ID_2, tik + 250);

		stdcast(c, CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * 1.25f, esprl));

		c->ix = tx;
		c->iy = ty;
	}

	return 0;
}

char cast_generative_essence(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;
	char cidx = c->ch == &ch;

	CD_RET(1500);

	disps[cidx]
		.dspbrk = CAS_DSP_ADD_FN_0(SPR_BLOCK / (8 - 1), esprl);
	disps[cidx]
		.dsplif = CAS_DSP_ADD_FN_0(SPR_BLOCK / (8 + 2), esprl);

	short id[] = { SPRITE_DUST_ID_0, SPRITE_DUST_ID_2, SPRITE_DUST_ID_1, };

	for (int i = 0; i < 3; i++)
		add_dust(c->ch->sprite.x, c->ch->sprite.y,
			((float)i / 3) * (1.75f)
			, id[i % (sizeof(id) / sizeof(*id))], i == 0 ? tik + 1000 : tik + 500);

	return 0;
}

char cast_font_gale(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;
	char cidx = c->ch == &ch;

	CD_RET(1000);

	disps[cidx]
		.dspbrk = CAS_DSP_ADD_FN_0(SPR_BLOCK / (8 + 4), esprl);
	disps[cidx]
		.dsplif = CAS_DSP_ADD_FN_0(SPR_BLOCK / (8 + 4), esprl);

	for (int i = 0; i < 3; i++)
		add_dust(c->ch->sprite.x, c->ch->sprite.y,
			((float)i / 3) * (1.75f)
			, SPRITE_DUST_ID_0, tik + 250);

	return 0;
}

char cast_invoke_font(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;
	CD_DEC_SPR(500, SPR_BLOCK / (8 + 8));

	add_dust(c->ix, c->iy, 0, SPRITE_DUST_ID_0, tik + 250);

	for (int i = 0; i < 2; i++)
		add_dust(c->ix, c->iy, (.25f + ((esprl >= 1) / 8.0f)) * (2 * i + 1), SPRITE_DUST_ID_1, tik + 250 * 4);

	if (esprl >= 1)
		for (int i = 0; i < 2; i++)
			add_dust(c->ix, c->iy, (.25f + ((esprl >= 1) / 8.0f)) * (2 * i + 1), SPRITE_DUST_ID_2, tik + 250);

	stdcast(c, CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * (2.0f + .0625f), esprl));

	if (xorrand() % (2))
	{
		char s = (c->ch->rot / 2) * 2 - 1;
		int x, y;
		get_dixiy(&x, &y, c->ch);

		c->ix -= (xorrand() % 2) * (y * s + (x * ((xorrand() % 3) - 1)));
		c->iy -= (xorrand() % 2) * (x * s + (y * ((xorrand() % 3) - 1)));

		cast_invoke_font(c);
	}

	return 0;
}

char cast_secret_charge(c_cast* c)
{
	static int cd[2];
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;
	CD_DEC_SPR(500, SPR_BLOCK / (8 + 8));

	int x, y;
	get_dixiy(&x, &y, c->ch);

	for (int ii = -1; ii <= +1; ii++)
		add_dust(c->ix + x * ii, c->iy + y * ii, 0, SPRITE_DUST_ID_0, tik + 250);

	for (int ii = -1; ii <= +1; ii++)
		for (int i = 0; i < 2; i++)
			add_dust(c->ix + x * ii, c->iy + y * ii, (.25f + ((esprl >= 1) / 8.0f)) * (2 * i + 1), SPRITE_DUST_ID_1, tik + 250 * 4);

	if (esprl >= 1)
		for (int ii = -1; ii <= +1; ii++)
			for (int i = 0; i < 2; i++)
				add_dust(c->ix + x * ii, c->iy + y * ii, (.25f + ((esprl >= 1) / 8.0f)) * (2 * i + 1), SPRITE_DUST_ID_2, tik + 250);

	for (int ii = -1; ii <= +1; ii++)
	{
		int xx = c->ix;
		int yy = c->iy;

		c->ix = c->ix + x * ii;
		c->iy = c->iy + y * ii;
		stdcast(c, CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * 1.25f, esprl));
		c->ix = xx;
		c->iy = yy;
	}

	return 0;
}

char cast_deep_envelope(c_cast* c)
{
	static int cd[2];
	char cidx = c->ch == &ch;
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	CD_RET_EX(1000 * 2, 500);

	disps[cidx]
		.dspbrk = CAS_DSP_ADD_FN_0(SPR_BLOCK / (8 - 1), esprl);
	disps[cidx]
		.dsplif = CAS_DSP_ADD_FN_0(SPR_BLOCK / (8 + 2), esprl);

	if (esprl >= 1)
		for (int i = 0; i < 3; i++)
			add_dust(c->ch->sprite.x, c->ch->sprite.y, ((1.75f / 3) * (2 * i + 1)) / 2, SPRITE_DUST_ID_2, tik + 250);

	for (int i = 0; i < 3; i++)
		add_dust(c->ch->sprite.x, c->ch->sprite.y, ((1.75f / 3) * (2 * i)) / 2, SPRITE_DUST_ID_0, tik + 250);

	return 0;
}

char cast_riptide(c_cast* c)
{
	static int cd[2];
	char cidx = c->ch == &ch;
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	CD_DEC_SPR(125, SPR_BLOCK / (8));

	float lstate = esprl >= 1;

	for (int i = 0; i < 3; i++)
	{
		add_dust(
			c->ix,
			c->iy,
			((2 * i + 0) * (lstate + 0.75f)) / (2 * 3),
			SPRITE_DUST_ID_1,
			tik + 250
		);
		add_dust(
			c->ix,
			c->iy,
			((2 * i + 1) * (lstate + 0.75f)) / (2 * 3),
			SPRITE_DUST_ID_0,
			tik + 250
		);
	}

	stdcast(c, CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * 2.75f, esprl));

	return 0;
}

char cast_dynamic_current(c_cast* c)
{
	ch.elem[0]
		.sprb = ELEM_SPR_BLOCK * 2;

	static int cd[2];
	char cidx = c->ch == &ch;
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	CD_DEC_SPR(125, SPR_BLOCK / (8 + 4));

	int x, y;
	get_dixiy(&x, &y, c->ch);

	float lstate = (esprl >= 1) * 2;

	for (int i = -1; i <= +1; i++)
	{
		int xx = c->ix; c->ix = c->ix + x * i;
		int yy = c->iy; c->iy = c->iy + y * i;

		add_dust(
			c->ix,
			c->iy,
			(1 * (lstate + 0.75f)) / (2 * 3),
			SPRITE_DUST_ID_1,
			tik + 250
		);
		add_dust(
			c->ix,
			c->iy,
			(2 * (lstate + 0.75f)) / (2 * 3),
			SPRITE_DUST_ID_0,
			tik + 250
		);

		stdcast(c, CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * 1.75f, esprl));

		c->ix = xx;
		c->iy = yy;
	}


	return 0;
}

char cast_spirit_element(c_cast* c)
{
	ch.elem[0]
		.sprb = ELEM_SPR_BLOCK * 2;

	static int cd[2];
	char cidx = c->ch == &ch;
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	float lstate = (esprl >= 1) * 2;

	disps[cidx]
		.dspbrk = SPR_BLOCK / (8 + 2);
	disps[cidx]
		.dsplif = SPR_BLOCK / (8 + 4);

	for (int x = -1; x <= +1; x++)
		for (int y = -1; y <= +1; y++)
		{
			if (x == 0 && y == 0) continue;

			add_dust(c->ch->sprite.x + x
				, c->ch->sprite.y + y

				, x * y
				? .25f
				: 0

				, x * y
				? SPRITE_DUST_ID_0
				: SPRITE_DUST_ID_1

				, tik + 250
			);

		}

	return 0;
}

char cast_tempest_mist(c_cast* c)
{
	static int cd[2];
	char cidx = c->ch == &ch;
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	CD_DEC_SPR(125, SPR_BLOCK / (8 + 2));

	float lstate = esprl >= 1;

	for (int i = 0; i < (2 + lstate); i++)
	{
		add_dust(
			c->ix,
			c->iy,
			(2 * i + 0) * .25f,
			SPRITE_DUST_ID_1,
			tik + 250
		);

		add_dust(
			c->ix,
			c->iy,
			(2 * i + 1) * .25f,
			SPRITE_DUST_ID_2,
			tik + 250
		);

		if (lstate != 0 && 1 ^ (i % 2))
			add_dust(
				c->ix,
				c->iy,
				(2 * i + 1) * .25f,
				SPRITE_DUST_ID_0,
				tik + 250
			);
	}

	stdcast(c, CAS_BREAK_ADD_FN_0(BREAKM_BLOCK * (2 + .25f + .125f), esprl));

	return 0;
}

char cast_deep_reserve(c_cast* c)
{
	ch.elem[0].sprb = ELEM_SPR_BLOCK * 2;

	static int cd[2];
	char cidx = c->ch == &ch;
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	CD_RET_EX(1024 * 30, 1024 * 1);

	static unsigned char mod[2] = { 0,0, };

	if ((++(mod[cidx])) % 2)
	{
		for (int i = 2; i < 4; i++)
		{
			add_dust(c->ch->sprite.x,
				c->ch->sprite.y,
				((float)(2 * i + 0) / 3) * 1.25f,
				SPRITE_DUST_ID_1,
				tik + 250
			);
			if (esprl >= 1)
				add_dust(c->ch->sprite.x,
					c->ch->sprite.y,
					((float)((2 * i + 1)) / 3) * 1.25f,
					SPRITE_DUST_ID_0,
					tik + 250
				);
		}

		c->ch->spra += SPR_BLOCK * ((esprl >= 1) + 1);
		if (c->ch->spra > c->ch->sprb)
			c->ch->spra = c->ch->sprb;
	}

	return 0;
}

char cast_elusive_gale(c_cast* c)
{
	ch.elem[0].sprb = ELEM_SPR_BLOCK * 2;

	static int cd[2];
	char cidx = c->ch == &ch;
	char esprl = (c->ch->elem[c->ch->elemidx].sprb + 1) / ELEM_SPR_BLOCK;

	CD_RET_EX(1024 * 4, 256);

	disps[cidx]
		.dspbrk = SPR_BLOCK / (8 + 4);
	disps[cidx]
		.dsplif = SPR_BLOCK / (8 + 4 + 2);

	for (char x = -1; x <= +1; x++)
		for (char y = -1; y <= +1; y++)
		{
			int id[] = { SPRITE_DUST_ID_0, SPRITE_DUST_ID_1, };

			int i = (x == 0) ^ (y == 0);

			float z = i / 4.0f;

			add_dust(
				c->ch->sprite.x + x,
				c->ch->sprite.y + y,
				z,
				id[i],
				tik + 500
			);
		}

	return 0;
}

intc_intcast_a intcasts;

/*ID	CAST
* 15	cascade
* 4		surge
* 9		illuminate
* 7		return
* 10	suspension
* 11	gloomy haze
* 21	debug rm
* 13	stonefall
* 14	ancient winsdom
* 22	enigma song
* 23	eternal mirage
* 17	tangle
* 3		flare
* 24	flurry
* 6		dig
* 25	sunder cast
* 26	debug tp
* 27	torrent
* 28	tidal surge
* 29	ancient mirage
* 30	cyclone
* 31	sonic wave
* 32	beryl song
* 33	radiant crest
* 34	thicket
* 35	leaf dance
* 36	pin whirl
* 37	whirl
* 38	bell song
* 39	hidden power
* 40	downrush
* 41	cryptic symbol
* 42	roll
* 43	harmonic cast
* 44	radiant surge
* 45	dream barrier
* 46	echo
* 47	moon surge
* 48	frigid grove
* 49	moon aurora
* 50	focus stream
* 51	focus wind
* 52	harmonic tone
* 53	focus rush
* 54	static affinity
* 55	swift focus
* 56	swift pulse
* 57	generative pulse
* 58	generative essence
* 59	font gale
* 60	invoke font
* 61	secret charge
* 62	deep envelope
* 63	riptide
* 64	dynamic current
* 65	spirit element
* 66	tempest mist
* 67	deep reserve
* 68	elusive gale
*/

char (*castfn[])(c_cast* c) =
{
	&cast0, &cast1, &cast_fume_wave, &cast_flare, &cast_surge, &cast_shred, &cast_dig, &cast_return,
	&cast_disperse, &cast_dsp_illuminate, &cast_suspension, &cast_gloomyhaze, &cast_pursue, &cast_stonefall, &cast_ancwis, &cast_cascade,
	&cast_sspore, &cast_tangle, &cast_growth, &cast_diffusion, &cast_resonance, &cast_debug_rm, &cast_enigma_song, &cast_eternal_mirage,
	&cast_flurry, &cast_sunder_cast, &cast_debug, &cast_torrent, &cast_tidal_surge, &cast_ancient_mirage, &cast_cyclone, &cast_sonic_wave,
	&cast_beryl_song, &cast_radiant_crest, &cast_thicket, &cast_leaf_dance, &cast_pin_whirl, &cast_whirl, &cast_bell_song, &cast_hidden_power,
	&cast_downrush, &cast_cryptic_symbol, &cast_roll, &cast_harmonic_cast, &cast_radiant_surge, &cast_dream_barrier, &cast_echo, &cast_moon_surge,
	&cast_frigid_grove, &cast_moon_aurora, &cast_focus_stream, &cast_focus_wind, &cast_harmonic_tone, &cast_focus_rush, &cast_static_affinity, &cast_swift_focus,
	&cast_swift_pulse, &cast_generative_pulse, &cast_generative_essence, &cast_font_gale, &cast_invoke_font, &cast_secret_charge, &cast_deep_envelope, &cast_riptide,
	&cast_dynamic_current, &cast_spirit_element,&cast_tempest_mist, &cast_deep_reserve, &cast_elusive_gale, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};

char (*icasfn[])(c_cast* c) = { &icas_fume_wave, &icas_disperse , &icas_gloomyhaze, &icas_sunder_cast, &icas_torrent, &icas_harmonic_tone, };