#include "itemcache.h"

#include "character.h"

s_spritedata_a itexta[ITEM_MAX_SIZE];

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
* C* DIVE					54
* GEM (WATER)				55

* 00 M: TREE (SMALL)			31
* 01 M: GOLDEN BERRY			32
* 02 M: BORDER (WOOD)			33
* 03 M: BORDER (FABRIC)			34
* 04 M: CORNER (FABRIC)			35
* 05 M: BORDER (ROCK)			36
* 06 M: CHAIR (BASIC)			37
* 07 M: POLES (BASIC)			38
* 08 M: CORNER (WOOD)			39
* 09 M: TREE (BERRY BUSH)		40
* 10 M: BLANK					41
* 11 M: FLOOR (GRASS)			42
* 12 M: PLANT (BASIC)			43
* 13 M: ROCK (BASIC)			44
* 14 M: PLANT (RUSTIC)			45
* 15 M: GOLD TREASURE			46
* 16 M: RARE PILLAR				47
* 17 M: LUSH PILLOW				48

* 18 M: BORDER (LAND)			56
* 19 M: BORDER (LAND SEG)		57
* 20 M: BORDER (PLANK)			58
* 21 M: CORNER (PLANK)			59
* 22 M: PLANT (BLOOMING)		60
* 23 M: POLES (METAL)			61
* 24 M: BORDER (WOOD PLANK)		62
* 25 M: BLUE TREE				63
* 26 M: FLOOR (DIRT A)			64
* 27 M: FLOOR (DIRT B)			65
* 28 M: FLOOR (DIRT C)			66

* 18 P: (PLANTS)				49
* 19 P: (BORDERS)				50
* 20 P: (OTHERS)				51
* 21 P: (LANDS)					52
* 22 P: (RARE)					53
*/

const char* itemtext[ITEM_MAX_SIZE] =
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

	"M: TREE (SMALL)",
	"M: GOLDEN BERRY",
	"M: BORDER (WOOD)",
	"M: BORDER (FABRIC)",
	"M: CORNER (FABRIC)",
	"M: BORDER (ROCK)",
	"M: CHAIR (BASIC)",
	"M: POLES (BASIC)",
	"M: CORNER (WOOD)",
	"M: TREE (BERRY BUSH)",
	"M: BLANK",
	"M: FLOOR (GRASS)",
	"M: PLANT (BASIC)",
	"M: ROCK (BASIC)",
	"M: PLANT (RUSTIC)",
	"M: GOLD TREASURE",
	"M: RARE PILLAR",
	"M: LUSH PILLOW",
	"P: (PLANTS)",
	"P: (BORDERS)",
	"P: (OTHERS)",
	"P: (LANDS)",
	"P: (RARE)",

	"C* DIVE",
	"GEM (WATER)",

	"M: BORDER (LAND)",
	"M: BORDER (LAND SEG)",
	"M: BORDER (PLANK)",
	"M: CORNER (PLANK)",
	"M: PLANT (BLOOMING)",
	"M: POLES (METAL)",
	"M: BORDER (WOOD PLANK)",
	"M: BLUE TREE",
	"M: FLOOR (DIRT A)",
	"M: FLOOR (DIRT B)",
	"M: FLOOR (DIRT C)",

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