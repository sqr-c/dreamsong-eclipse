#pragma once

#include "sprite.h"

#include <math.h>
#include <stdint.h>

#define SPR_MAX		((float)2147483647)	//PRE: ((int)(pow(2,31)-1))
#define SPR_BLOCK	(SPR_MAX/16)

#define CH_MAX_LIFE	(SPR_BLOCK * 8)

#define MOVE_C .25f
#define MOVE_T .0125f

extern int dr;
extern int ddx;
extern int ddy;

#define ELEM_CACHE_MAX	8

#define ELEM_SPR_MAX	((float)32767)	//PRE: (pow(2,15)-1)
#define ELEM_SPR_BLOCK	(ELEM_SPR_MAX / 4)

typedef struct
{
	char id;
	unsigned short spra;
	unsigned short sprb;
	short cast[4];
} e_element;

typedef struct
{
	char id;
	unsigned short sprb;
	short cast[4];
} e_element_min;

typedef enum
{
	CH_STATE_IDLE,
	CH_STATE_WALK,
	CH_STATE_ACTION,
	CH_STATE_DIVE,
} CH_STATE;

typedef struct
{
	int spriteo;

	int dx, dy;
	s_sprite sprite;

	char state;
	char rot;
	char flip;

	uint32_t /*struct ch_char**/ target;

	int spra, sprb;
	int lifea, lifeb;
	int token;
	short defcast[4];

	char elemidx;

	int gcool;

	unsigned int acttick;

	e_element elem[ELEM_CACHE_MAX];
} ch_char;

void swap_elem(e_element* a, e_element* b);
ch_char create_character(float x, float y, int cid, int spra, int sprb, int elemidx);
#define CH_MOVE_VELOCITY_L	(1 * pow(2, -2))
#define CH_MOVE_VELOCITY_H	(+(1 * pow(2, 0)) +(1 * pow(2, -3)))
char ch_move(ch_char* ch, float v);
void ch_set_state(ch_char* ch, char rot, char state);