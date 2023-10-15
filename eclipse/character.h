#pragma once

#include "sprite.h"

#include <math.h>

#define SPR_MAX		2147483647	//PRE: ((int)(pow(2,31)-1))
#define SPR_BLOCK	(SPR_MAX/16)

#define MAX_LIFE	(SPR_BLOCK * 8)

#define MOVE_C .25f
#define MOVE_T .0125f

extern int dr;
extern int ddx;
extern int ddy;

#define ELEM_CACHE_MAX	8

#define ELEM_SPR_MAX	32767	//PRE: (pow(2,15)-1)
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

typedef struct 
{
	int spriteo;

	int dx, dy;
	s_sprite sprite;

	char state;
	char rot;
	char flip;

	struct ch_char* target;

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
char move(ch_char* ch, int x, int y);
void set_state(ch_char* ch, char rot, char state);