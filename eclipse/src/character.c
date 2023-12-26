#include "character.h"

#include <math.h>
#include <string.h>

int dr = 0;
int ddx;
int ddy;

void swap_elem(e_element* a, e_element* b)
{
	e_element t = *a;
	*a = *b;
	*b = t;
}

void ch_set_state(ch_char* ch, char rot, char state)
{
	char dr = rot != ch->rot;
	char ds = state != ch->state;

	if (dr || ds)
	{
		ch->rot = rot;
		if (ch->state == CH_STATE_DIVE) return;
		ch->state = state;

		int id0, id1;

		if (state == 0)
		{
			id0 = 0;
			id1 = 1;
		}
		else if (state == 1)
		{
			id0 = 2;
			id1 = 5;
		}
		else if (state == 2)
		{
			id0 = 8;
			id1 = 11;
		}

		id0 += ch->spriteo;
		id1 += ch->spriteo;

		if (rot == 0)
		{
			ch->flip = 0;
			ch->sprite.id = id0;
		}
		else if (rot == 1)
		{
			ch->flip = 1;
			ch->sprite.id = id0;
		}
		else if (rot == 3)
		{
			ch->flip = 1;
			ch->sprite.id = id1;
		}
		else// if (rot == 2)
		{
			ch->flip = 0;
			ch->sprite.id = id1;
		}

		if (ds)
			ch->sprite.deltatime = SDL_GetTicks();
	}
}

float msign(float x)
{
	return
		x == 0
		? 0
		: (x < 0
			? -1
			: +1);
}

float sign(float x)
{
	return
		(x < 0 ? -1 : +1);
}

#include "save.h"

char ch_move(ch_char* chr, float v)
{
	static char sddx[2] = { 0, 0, };
	static char sddy[2] = { 0, 0, };
	char cidx = chr == &ch;

	char* ddx = &(sddx[cidx]);
	char* ddy = &(sddy[cidx]);

	if (0 == *ddx)
		*ddy = msign(chr->dy - chr->sprite.y);
	if (0 == *ddy)
		*ddx = msign(chr->dx - chr->sprite.x);

	if (*ddx)
	{
		chr->sprite.x += v * *ddx;
		*ddx = msign(chr->dx - chr->sprite.x);
		if (*ddx)
			chr->sprite.y += v * *ddy;
	}

	if (*ddy)
	{
		chr->sprite.y += v * *ddy;
		*ddy = msign(chr->dy - chr->sprite.y);
		if (*ddy)
			chr->sprite.x += v * *ddx;
	}

	{
		char flag = v >= 1;
		if (flag)
		{
			if (-*ddx * chr->sprite.x > -*ddx * chr->dx)
				chr->sprite.x = chr->dx += *ddx * (chr->dx - chr->sprite.x);
			if (-*ddy * chr->sprite.y > -*ddy * chr->dy)
				chr->sprite.y = chr->dy += *ddy * (chr->dy - chr->sprite.y);
		}

		{
			if (*ddx)
				ch_set_state(chr, (2 * (flag ^ (*ddx < 0)) + 0) % 4, CH_STATE_WALK);
			else if (*ddy)
				ch_set_state(chr, (2 * (flag ^ (*ddy < 0)) + 1) % 4, CH_STATE_WALK);
			else if (chr->state != 0 && chr->state != 2 && chr->sprite.x == chr->dx && chr->sprite.y == chr->dy)
				ch_set_state(chr, chr->rot, CH_STATE_IDLE);
		}
	}
}

ch_char create_character(float x, float y, int cid, int spra, int sprb, int elemidx)
{
	ch_char buf;

	buf.lifea = buf.lifeb = SPR_BLOCK * 8;

	memset(&buf, 0, sizeof(ch_char));

	buf.spriteo = cid;

	buf.spra = 4 * SPR_BLOCK;
	buf.sprb = 5 * SPR_BLOCK;

	buf.sprite.id = cid;
	buf.sprite.x = x;
	buf.sprite.y = y;

	buf.dx = x;
	buf.dy = y;

	buf.target = NULL;

	buf.spra = spra;
	buf.sprb = sprb;
	buf.elemidx = elemidx;

	buf.elem[0].id = 0;
	buf.elem[1].id = 0;
	buf.elem[0].spra = 0;
	buf.elem[0].sprb = 0;

	return buf;
}