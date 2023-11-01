#include "itemcache.h"

#include "character.h"

s_spritedata_a itexta[ITEM_MAX_SIZE];

int elem_id_itexta(int id)
{
	if (id == 40) return ELEM_TEXT_OFFSET + 0;
	else if (id == 43) return ELEM_TEXT_OFFSET + 1;
	else if (id == 41) return ELEM_TEXT_OFFSET + 2;
}

//rarity low to high
char ashes_table[] =
{
	ASHES_ID_0, ASHES_ID_1, ASHES_ID_2, ASHES_ID_3, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};

/*ITEM	IID
* MINI ETHER				01
* ETHER						02
* MINI DRAUGHT				05
* GEM (RED)					09
* GEM (GREEN)				10
* GEM (BLUE)				11
* GEM (LILAC)				21
* C* DIG					22
* C* SUNDER CAST			23
* C* RETURN					12
* ARNEA TICKET				13
* PARK TICKET				14
* MUTED WATER				18
* PALE EARTH				19
* ASHES (BLACK)				24
* ASHES (BLACK, RED)		25
* C* ECHO					26
* ELEM CHARGE (LESSER)		27
* SPR CHARGE (LESSER)		28
* ASHES (RED, GREEN)		29
* ASHES (RED)				30

* 00 MAT: TREE (BASIC)			31
* 01 MAT: GOLDEN BERRY			32
* 02 MAT: BORDER (WOOD)			33
* 03 MAT: BORDER (FABRIC)		34
* 04 MAT: CORNER (FABRIC)		35
* 05 MAT: BORDER (ROCK)			36
* 06 MAT: CHAIR (BASIC)			37
* 07 MAT: POLES (BASIC)			38
* 08 MAT: CORNER (WOOD)			39
* 09 MAT: BERRY BUSH			40
* 10 MAT: BLANK					41
* 11 MAT: FLOOR (GRASS)			42
* 12 MAT: PLANT (BASIC)			43
* 13 MAT: ROCK (BASIC)			44
* 14 MAT: PLANT (RUSTIC)		45
* 15 MAT: AURIC TREASURE		46
* 16 MAT: RARE PILLAR			47
* 17 MAT: LUSH PILLOW			48
* 18 PKG: (PLANTS)				49
* 19 PKG: (BORDERS)				50
* 20 PKG: (OTHERS)				51
* 21 PKG: (LANDS)				52
* 22 PKG: (RARE)				53
*/

const char* ItemText[ITEM_MAX_SIZE] =
{
"SEALED CRATE",
"MINI ETHER", "ETHER", "MASS ETHER", "SKY ETHER",
"MINI DRAUGHT", "DRAUGHT", "MASS DRAUGHT", "SKY DRAUGHT",
"GEM (RED)","GEM (GREEN)","GEM (BLUE)",
"C* RETURN",
"ARENA TICKET",
"PARK TICKET",
"C* PURSUE",
"GEM (PURPLE)",
"MYSTIC POWDER",
"MUTED WATER",
"PALE EARTH",
"C* GLOOMY HAZE",
"GEM (LILAC)",
"C* DIG",
"C* SUNDER CAST",
"ASHES (BLACK)",
"ASHES (BLACK, RED)",
"C* ECHO",
"ELEM CHARGE (L)",
"SPR CHARGE (L)",
"ASHES (RED, GREEN)",
"ASHES (RED)",

"M: TREE (BASIC)",
"M: GOLDEN BERRY",
"M: BORDER (WOOD)",
"M: BORDER (FABRIC)",
"M: CORNER (FABRIC)",
"M: BORDER (ROCK)",
"M: CHAIR (BASIC)",
"M: POLES (BASIC)",
"M: CORNER (WOOD)",
"M: BERRY BUSH",
"M: BLANK",
"M: FLOOR (GRASS)",
"M: PLANT (BASIC)",
"M: ROCK (BASIC)",
"M: PLANT (RUSTIC)",
"M: AURIC TREASURE",
"M: RARE PILLAR",
"M: LUSH PILLOW",
"P: (PLANTS)",
"P: (BORDERS)",
"P: (OTHERS)",
"P: (LANDS)",
"P: (RARE)",

"LANTERN",
"HERBS",
"GATEWAY",
};

char menu_sel_idx_0 = 0;
char menu_sel_idx_1 = -1;
char menu_sel_idx_2 = -1;

void swap_cache(icac_itemcache* a, icac_itemcache* b)
{
	icac_itemcache t = *a;
	*a = *b;
	*b = t;
}

int indexof_cache(icac_itemcache* arr, int length, int findid)
{
	for (int i = 0; i < length; i++)
		if (arr[i].id == findid)
			return i;
	return -1;
}

void rm_cache(icac_itemcache* arr, unsigned char* length, int idx)
{
	for (; idx + 1 < *length; idx++)
		swap_cache(&arr[idx], &arr[idx + 1]);

	(*length)--;
}

#include "save.h"

void dec_rm_cache(icac_itemcache* c)
{
	if (--c->mul <= 0)
		rm_cache(cache, &cache_top, c - cache);
}