#pragma once

#include <string.h>

#include "itemcache.h"

typedef struct
{
	icac_itemcache* arr;
	int* cost;
	int size;
	int capacity;
} mar_marketitems;

#define MARKETS_LENGTH	1

extern mar_marketitems markit[MARKETS_LENGTH];
extern int markitidx;

void init_all_market();
void clear_market(int idx);
void free_market();