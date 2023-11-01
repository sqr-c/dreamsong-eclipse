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

void set_state(ch_char* ch, char rot, char state)
{
	char dr = rot != ch->rot;
	char ds = state != ch->state;

	if (dr || ds)
	{
		ch->state = state;
		ch->rot = rot;

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

char move(ch_char* ch, int x, int y)
{
	float dx = ch->sprite.x - x;
	float dy = ch->sprite.y - y;

	{
		if (dx < -MOVE_T)
		{
			ch->sprite.x += MOVE_C;
			set_state(ch, 0, 1);
			return 4;
		}
		else if (dx > MOVE_T)
		{
			ch->sprite.x -= MOVE_C;
			set_state(ch, 2, 1);
			return 3;
		}
		else ch->sprite.x = (int)x;
	}

	{
		if (dy < -MOVE_T)
		{
			ch->sprite.y += MOVE_C;
			set_state(ch, 1, 1);
			return 2;
		}
		else if (dy > MOVE_T)
		{
			ch->sprite.y -= MOVE_C;
			set_state(ch, 3, 1);
			return 1;
		}
	}

	{
		ch->sprite.y = y;
		ch->sprite.x = x;
		if (ch->state == 1)
			set_state(ch, ch->rot, 0);
		return 0;
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