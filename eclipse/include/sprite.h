#pragma once

typedef enum SPRITE_FLAG
{
	SPRITE_FLAG_NONE,			// 0
	SPRITE_FLAG_BLOCK,			// 1
	SPRITE_FLAG_BREAK,			// 1
	SPRITE_FLAG_CACHE,			// 1
	SPRITE_FLAG_TFN,			// 0
	SPRITE_FLAG_TERM,			// 1
	SPRITE_FLAG_TEXT,			// 1
	SPRITE_FLAG_DROP,			// 0
	SPRITE_FLAG_DMOUT,			// 0
	SPRITE_FLAG_STCHNG,			// 0
	SPRITE_FLAG_POPLSTACK,		// 0
	SPRITE_FLAG_POPDSTSTACK,	// 0
	SPRITE_FLAG_WATER,			// 1
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