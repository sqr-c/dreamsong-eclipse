#pragma once

#include "npc.h"
#include <SDL.h>
#include <string.h>

int npcidx = -1;

#define INIT_SEED	0x5E0B7007
unsigned int xrseed = INIT_SEED;

unsigned int xorrand()
{
	xrseed ^= (xrseed << 13);
	xrseed ^= (xrseed >> 17);
	xrseed ^= (xrseed << 5);
	return xrseed + XORRAND_OFS;
}

n_npc create_npc(int x, int y, int cid, short walk_d, int textidx, int fnidx, short cix, short ciy, int spra, int sprb, int elemidx, int elemtop)
{
	n_npc buf;

	buf.ch = create_character(x, y, cid, spra, sprb, elemidx);
	buf.textidx = textidx;
	buf.fnidx = fnidx;
	buf.walk_d = walk_d;

	buf.ix = x;
	buf.iy = y;

	buf.move_seed = 0;
	buf.istate = 0;

	buf.cix = cix;
	buf.ciy = ciy;

	return buf;
}

n_npc_a create_NPCA(int capacity)
{
	n_npc_a buf;
	buf.arr = (n_npc*)malloc(capacity * sizeof(n_npc));
	buf.size = 0;
	buf.capacity = capacity;
	return buf;
}

void free_NPCA(n_npc_a* array)
{
	free(array->arr);
}

void resize_array_NPCA(n_npc_a* array, int new_capacity)
{
	n_npc* new_arr = (n_npc*)malloc(new_capacity * sizeof(n_npc));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_NPCA(n_npc_a* array, n_npc* value)
{
	if (array->size == array->capacity)
		resize_array_NPCA(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}

n_npc* get_element_NPC(n_npc_a* array, int index)
{
	if (index < 0 || index >= array->size)
		exit(1);
	return &(array->arr[index]);
}

char rayselect(n_npc_a* arr, ch_char* ch)
{
	for (int i = 0; i < arr->size; i++)
	{
		int ry = ch->rot % 2;
		int rx = !ry;

		char s = -1 + ((ch->rot <= 1) * 2);

		rx *= s;
		ry *= s;

		for (int ii = 0; ii < 8; ii++)
			if ((int)(arr->arr[i].ch.sprite.x) == (int)(ch->sprite.x + rx * ii)
				&&
				(int)(arr->arr[i].ch.sprite.y) == (int)(ch->sprite.y + ry * ii)
				)
			{
				npcai = i;
				return 0;
			}
	}

	npcai = -1;

	return 1;
}

void clearnsm(const char* path)
{
	FILE* fp;
	fp = fopen(path, "wb");

	char buf = 0;

	for (int i = 0; i < 25; i++)
		fwrite(&buf, 1, 1, fp);

	fclose(fp);
}

void readnsm(const char* path, n_npc* n)
{
	FILE* fp;
	fp = fopen(path, "rb");

	char buf[(sizeof(char) + sizeof(short)) / sizeof(char)];

	int i;

	for (; i = fread(buf, sizeof(buf), 1, fp) > 0 && *((short*)(&buf[1])) != n->nsmid;);

	fclose(fp);

	if (*((short*)(&buf[1])) == n->nsmid)
		n->istate = buf[0];
}

void writensm(const char* path, n_npc* n)
{
	FILE* fp;
	fp = fopen(path, "r+b");

	char buf[(sizeof(char) + sizeof(short)) / sizeof(char)];
	memset(buf, -1, sizeof(buf));

	int tik = 0;
	for (; fread(buf, sizeof(buf), 1, fp) > 0 && *((short*)(&buf[1])) != n->nsmid;) ;

	long seek = ftell(fp);

	if (*((short*)(&buf[1])) == n->nsmid)
		seek -= sizeof(buf);
	fseek(fp, seek, SEEK_SET);

	buf[0] = n->istate;
	*((short*)(&buf[1])) = n->nsmid;

	fwrite(buf, sizeof(buf), 1, fp);

	fclose(fp);
}

#include "dglobal.h"
void ch_transition_update();

void init_en_npc(n_npc* buf, n_npc_min* npcm)
{
	memset(buf, 0, sizeof(n_npc));

	buf->ch.spriteo = npcm->sprite_id;

	castp = npcm->estr;
	castc = npcm->ecycl;

	buf->ch.rot = 0;
	buf->ch.state = 1;
	set_state(&buf->ch, buf->ch.rot, 0);

	memcpy(buf->ch.elem
		, npcm->eelem
		, sizeof(npcm->eelem));

	buf->istate = NPC_STATE_ENEMY;
	buf->fnidx = -1;
	buf->textidx = -1;

	buf->ch.spra = buf->ch.sprb = npcm->espr;
	buf->ch.lifea = MAX_LIFE;
	buf->ch.lifeb = MAX_LIFE;

	buf->ch.target = &ch;
}

void rm_npc(n_npc* npc)
{
	map.npca.arr
		[npc - map.npca.arr] =
		map.npca.arr
		[map.npca.size - 1];
	map.npca.size--;
	npcai = -1;
}

void set_npc_init_state_dm(n_npc* buf, int ix, int iy, float m)
{
	m = m * ((ch.rot / 2)
		? -1
		: +1);

	buf->ch.dx = ix
		+
		((ch.rot + 1) % 2)
		*
		m;

	buf->ch.dy = iy
		+
		(ch.rot % 2)
		*
		m;
}

float castp;
int castc;


void push_en_npc(n_npc_min* npcm)
{
	ch.lifea = ch.lifeb = MAX_LIFE;
	ch.spra = ch.sprb;

	n_npc buf;

	init_en_npc(&buf, npcm);

	if (map.npca.size <= 0)
		map.npca = create_NPCA(1);

	push_back_NPCA(&map.npca, &buf);

	npcai = map.npca.size - 1;
}

void push_en_npc_ex(n_npc_min* npcm, int x, int y, char sign)
{
	int i;
	for (i = 0; i < cyfn.size; i++)
		if (cyfn.arr[i] == &en_npc_update)
			break;
	if (i >= cyfn.size)
		push_back_IterFNA(&cyfn, &en_npc_update);

	push_en_npc(npcm);
	{
		ch_char* c = &map.npca.arr[map.npca.size - 1].ch;

		int dx = x - ch.sprite.x;
		int dy = y - ch.sprite.y;

		c->dx = (c->sprite.x =
			x - dx * 3 * sign) + dx;
		c->dy = (c->sprite.y =
			y - dy * 3 * sign) + dy;
	}
}

void en_npc_update()
{
	ch_transition_update();

	static char castmax = -1, castmin = -1;

	for (int i = 0; i < map.npca.size; i++)
	{
		if (map.npca.arr[i].istate == NPC_STATE_ENEMY)
		{
			float r = ((xorrand() % 256) / 256.0f);
			r *= castp;
			float castidx =
				r * (castmin - castmax) + castmax
				;

			castidx = (int)(castidx + ((castidx - (int)castidx) >= .5f ? 1 : 0));

			if (castidx > castmax) castidx = castmax;
			if (castidx < castmin) castidx = castmin;

			if (castmax == -1)
			{
				castmax = 3;
				for (int ii = 0; ii < 4; ii++)
					if (map.npca.arr[i].ch.elem
						[map.npca.arr[i].ch.elemidx]
						.cast[ii + 1] == -1)
					{
						castmax = ii;
						break;
					}

				castmin = 0;

				castidx = castmax;
			}

			if (
				map.npca.arr[i].ch.lifea != map.npca.arr[i].ch.lifeb
				&& lifec(&map.npca.arr[i].ch) <= 0
				)
			{
				map.npca.arr[i].istate = NPC_STATE_REMOVED;

				float m = 2;
				map.npca.arr[i].ch.dx
					+=
					m * (0
						+ map.npca.arr[i].ch.sprite.x
						- ch.sprite.x
						)
					;

				map.npca.arr[i].ch.dy
					+=
					m * (0
						+ map.npca.arr[i].ch.sprite.y
						- ch.sprite.y
						)
					;
			}
			else
			{
				n_npc* at = &map.npca.arr[i];

				//TODO: implement this and elem spr system after mvp

				int dx, dy;

				char db;

				dx = at->ch.dx - ch.dx;
				dy = at->ch.dy - ch.dy;
				db = (dx != 0 && dy != 0);

				if ((dx < -1) ^ (dx == -1 && db))
					at->ch.dx++;
				else if ((dx > 1) ^ (dx == +1 && db))
					at->ch.dx--;
				else if ((dy < -1) ^ (dy == -1 && db))
					at->ch.dy++;
				else if ((dy > 1) ^ (dy == +1 && db))
					at->ch.dy--;

				if (ch.state != 0);
				else if (dx == +1) at->ch.rot = 2;
				else if (dx == -1) at->ch.rot = 0;
				else if (dy == +1) at->ch.rot = 3;
				else if (dy == -1) at->ch.rot = 1;

				c_cast v;
				v.ch = &at->ch;
				v.ech = &ch;

				float mx = ch.sprite.x - at->ch.sprite.x;
				float my = ch.sprite.y - at->ch.sprite.y;

				v.ix = sign(mx) * (ch.sprite.x - at->ch.sprite.x) * (mx == 0 ? 1 : 1 / mx) + at->ch.sprite.x;
				v.iy = sign(my) * (ch.sprite.y - at->ch.sprite.y) * (my == 0 ? 1 : 1 / my) + at->ch.sprite.y;

				short* cast =
					at->ch.elemidx == -1
					? at->ch.defcast
					: at->ch.elem[at->ch.elemidx].cast
					;

				static int tik0 = -1;
				if (tik0 == -1) tik0 = tik;

				if (at->ch.gcool <= tik && (tik0 != tik / castc))
				{
					at->ch.acttick = tik + 500;

					if (lifec(&ch) <= 0)
					{
						ch.lifea = -MAX_LIFE;
						tik0 = 0;
					}
					else
					{
						tik0 = tik / castc;

						apply_cast(cast
							, castidx
							, &v, &at->ch);

						if (at->ch.spra <= 0)
							at->ch.elemidx = -1;
					}
				}
				else
					update_acttick(&at->ch);
			}
		}
		else if (map.npca.arr[i].istate == NPC_STATE_REMOVED &&
			((int)map.npca.arr[i].ch.sprite.x == map.npca.arr[i].ch.dx
				&& (int)map.npca.arr[i].ch.sprite.y == map.npca.arr[i].ch.dy))
		{
			rm_npc(&map.npca.arr[i]);
			castmax = -1;
			castmin = -1;

			int i;
			for (i = 0; i < map.npca.size; i++)
				if (map.npca.arr[i].istate == NPC_STATE_ENEMY)
					break;
			if (i >= map.npca.size)
			{
				rmf_IterFNA(&cyfn, &en_npc_update);
				ch.lifea = ch.lifeb;
			}
		}
	}
}