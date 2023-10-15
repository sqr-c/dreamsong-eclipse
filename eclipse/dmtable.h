#pragma once

#include "dmapgen.h"

typedef struct
{
	int x, y;
	dm_dmapmd dmmd;
} dm_dmt;

typedef struct
{
	dm_dmt* arr;
	long inseek;

	int size;
	int capacity;
} dm_dmt_a;

typedef struct
{
	dm_dmt_a* arr;
	int size;
	int capacity;
} DMTableFA;

dm_dmt_a create_DMTA(int capacity, long seek);
void free_DMTA(dm_dmt_a* array);
void resize_array_DMTA(dm_dmt_a* array, int new_capacity);
void push_back_DMTA(dm_dmt_a* array, dm_dmt* value);

DMTableFA create_DMTableFA(int capacity);
void free_DMTableFA(DMTableFA* array);
void resize_array_DMTableFA(DMTableFA* array, int new_capacity);
void push_back_DMTableFA(DMTableFA* array, dm_dmt_a* value);

void writedmt(DMTableFA* dmt, const char* path);
DMTableFA readdmt(const char* path);

void push_back_DMTableFA_DMT(DMTableFA* array, long inseek, dm_dmt* value);
dm_dmapmd* search_DMTable(DMTableFA* dmt, long inseek, int x, int y);