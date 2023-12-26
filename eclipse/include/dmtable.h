#pragma once

#include <stdint.h>

#include "dmapgen.h"

typedef struct
{
	int x, y;
	dm_dmapmd dmmd;
} dm_dmt;

typedef struct
{
	dm_dmt* arr;
	int32_t inseek;

	int size;
	int capacity;
} dm_dmt_a;

typedef struct
{
	dm_dmt_a* arr;
	int size;
	int capacity;
} dm_tablef_a;

dm_dmt_a create_DMTA(int capacity, int32_t seek);
void free_DMTA(dm_dmt_a* array);
void resize_array_DMTA(dm_dmt_a* array, int new_capacity);
void push_back_DMTA(dm_dmt_a* array, dm_dmt* value);

dm_tablef_a create_DMTableFA(int capacity);
void free_DMTableFA(dm_tablef_a* array);
void resize_array_DMTableFA(dm_tablef_a* array, int new_capacity);
void push_back_DMTableFA(dm_tablef_a* array, dm_dmt_a* value);

void writedmt(dm_tablef_a* dmt, const char* path);
dm_tablef_a readdmt(const char* path);

void push_back_DMTableFA_DMT(dm_tablef_a* array, int32_t inseek, dm_dmt* value);
dm_dmapmd* search_DMTable(dm_tablef_a* dmt, int32_t inseek, int x, int y);