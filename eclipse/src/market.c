#include "market.h"

mar_marketitems markit[MARKETS_LENGTH];
int markitidx = 0;

void init_all_market()
{
	mar_marketitems buf;

	icac_itemcache arr[] =
	{
		(icac_itemcache) {1, 10, 0, },
		(icac_itemcache) { 2, 8, 0, },
		(icac_itemcache) { 3, 4, 0, },
		(icac_itemcache) { 4, 1, 0, },

		(icac_itemcache) { 5, 12, 0, },
		(icac_itemcache) { 6, 10, 0, },
		(icac_itemcache) { 7, 4, 0, },
		(icac_itemcache) { 8, 2, 0, },
	};

	int cost[] =
	{
		2,
		3,
		6,
		10,

		2,
		3,
		6,
		10,
	};

	buf.size = sizeof(arr) / sizeof(*arr);
	buf.capacity = buf.size;
	buf.arr = (icac_itemcache*)malloc(buf.size * sizeof(icac_itemcache));
	buf.cost = (int*)malloc(buf.size * sizeof(int));

	memcpy(buf.arr, arr, buf.size * sizeof(icac_itemcache));
	memcpy(buf.cost, cost, buf.size * sizeof(int));

	markit[0] = buf;
}

void clear_market(int idx)
{
	for (int i = 0; i < markit[idx].size; i++)
		markit[idx].arr[i].active = 0;
}

void free_market()
{
	for (int i = 0; i < MARKETS_LENGTH; i++)
	{
		free(markit[i].arr);
		free(markit[i].cost);
	}
}