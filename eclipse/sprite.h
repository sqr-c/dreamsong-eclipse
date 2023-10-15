#pragma once

typedef enum SPRITE_FLAG
{
	SPRITE_FLAG_NONE,
	SPRITE_FLAG_BLOCK,
	SPRITE_FLAG_BREAK,
	SPRITE_FLAG_CACHE,
	SPRITE_FLAG_TFN,
	SPRITE_FLAG_TERM,
	SPRITE_FLAG_TEXT,
	SPRITE_FLAG_DROP,
	SPRITE_FLAG_DMOUT,
	SPRITE_FLAG_STCHNG,
	SPRITE_FLAG_POPLSTACK,
	SPRITE_FLAG_POPDSTSTACK,
} SPRITE_FLAG;

typedef struct
{
	char CX;
	char CY;

	unsigned short OX;
	unsigned short OY;

	unsigned short OW;
	unsigned short OH;

	char FJ;
	unsigned char FD;
} s_spritedata;

#define CHAR_SPRD_SIZE	14

typedef struct
{
	float x, y, z;
	int id;
	char flag;
	char flip;
	int deltatime;
} s_sprite;

typedef struct
{
	float x;
	float y;
} point;