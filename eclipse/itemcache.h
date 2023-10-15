#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "text.h"

#define ICACHE_MAX_SIZE	(+16+32)
#define ITEM_MAX_SIZE	(39+18)
//deprecated v
#define ELEM_TEXT_OFFSET	19

#define GEM_LILAC_ID	21

typedef struct
{
	int id;
	short mul;
	char active;
} icac_itemcache;

typedef struct
{
	unsigned short id;
	unsigned char mul;
} icac_itemcache_min;

extern s_spritedata_a itexta[ITEM_MAX_SIZE];
extern const char* ItemText[ITEM_MAX_SIZE];

extern int estore_top;

extern char menu_sel_idx_0;
extern char menu_sel_idx_1;
extern char menu_sel_idx_2;

extern int menu_1_top;
extern int menu_2_top;

void swap_cache(icac_itemcache* a, icac_itemcache* b);
int indexof_cache(icac_itemcache* arr, int length, int findid);
void rm_cache(icac_itemcache* arr, int* length, int idx);
void dec_rm_cache(icac_itemcache* c);
int elem_id_itexta(int id);

#define ASHES_ID_0	24
#define ASHES_ID_1	25
#define ASHES_ID_2	29
#define ASHES_ID_3	30

extern char ashes_table[];