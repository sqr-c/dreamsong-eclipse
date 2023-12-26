#include "dmtable.h"

#include <stdlib.h>
#include <stdio.h>

dm_dmt_a create_DMTA(int capacity, int32_t seek)
{
	dm_dmt_a array;
	array.arr = (dm_dmt*)malloc(capacity * sizeof(dm_dmt));
	array.size = 0;
	array.capacity = capacity;

	array.inseek = seek;

	return array;
}

void free_DMTA(dm_dmt_a* array)
{
	free(array->arr);
}

void resize_array_DMTA(dm_dmt_a* array, int new_capacity)
{
	dm_dmt* new_arr;
	new_arr = (dm_dmt*)malloc(new_capacity * sizeof(dm_dmt));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_DMTA(dm_dmt_a* array, dm_dmt* value)
{
	if (array->size == array->capacity)
		resize_array_DMTA(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}




dm_tablef_a create_DMTableFA(int capacity)
{
	dm_tablef_a array;
	array.arr = (dm_dmt_a*)malloc(capacity * sizeof(dm_dmt_a));
	array.size = 0;
	array.capacity = capacity;
	return array;
}

void free_DMTableFA(dm_tablef_a* array)
{
	for (int i = 0; i < array->size; i++)
		free_DMTA(&array->arr[i]);
	free(array->arr);
}

void resize_array_DMTableFA(dm_tablef_a* array, int new_capacity)
{
	dm_dmt_a* new_arr;
	new_arr = (dm_dmt_a*)malloc(new_capacity * sizeof(dm_dmt_a));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_DMTableFA(dm_tablef_a* array, dm_dmt_a* value)
{
	if (array->size == array->capacity)
		resize_array_DMTableFA(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}

void push_back_DMTableFA_DMT(dm_tablef_a* array, int32_t inseek, dm_dmt* value)
{
	for (int i = 0; i < array->size; i++)
		if (array->arr[i].inseek == inseek)
		{
			push_back_DMTA(&array->arr[i], value);
			return;
		}

	dm_dmt_a buf = create_DMTA(1, inseek);
	push_back_DMTA(&buf, value);
	push_back_DMTableFA(array, &buf);
}

dm_dmapmd* search_DMTable(dm_tablef_a* dmt, int32_t inseek, int x, int y)
{
	seekxy;

	for (int i = 0; i < dmt->size; i++)
		if (inseek == dmt->arr[i].inseek)
		{
			int mdx = abs(x - dmt->arr[i].arr[0].x);
			int mdy = abs(y - dmt->arr[i].arr[0].y);
			int md = mdx + mdy;
			int mi = 0;

			for (int ii = 1; ii < dmt->arr[i].size; ii++)
			{
				int dx = abs(dmt->arr[i].arr[ii].x - x);
				int dy = abs(dmt->arr[i].arr[ii].y - y);
				int d = dx + dy;

				if (d < md)
				{
					mdx = dx;
					mdy = dy;
					md = d;
					mi = ii;
				}
			}

			return &dmt->arr[i].arr[mi].dmmd;
		}

	return NULL;
}

void writedmt(dm_tablef_a* dmt, const char* path)
{
	FILE* f = fopen(path, "wb");

	fwrite(&dmt->size, sizeof(int), 1, f);

	for (int i = 0; i < dmt->size; i++)
	{
		fwrite(&dmt->arr[i].inseek, sizeof(int32_t), 1, f);
		fwrite(&dmt->arr[i].size, sizeof(int), 1, f);
		fwrite(dmt->arr[i].arr, sizeof(dm_dmt), dmt->arr[i].size, f);
	}

	fclose(f);
}

dm_tablef_a readdmt(const char* path)
{
	dm_tablef_a dmt;

	FILE* f = fopen(path, "rb");

	fread(&dmt.size, sizeof(int), 1, f);
	dmt.capacity = dmt.size;
	dmt.arr = (dm_dmt_a*)malloc(sizeof(dm_dmt_a) * dmt.capacity);

	for (int i = 0; i < dmt.size; i++)
	{
		fread(&dmt.arr[i].inseek, sizeof(int32_t), 1, f);
		fread(&dmt.arr[i].size, sizeof(int), 1, f);
		dmt.arr[i].capacity = dmt.arr[i].size;

		dmt.arr[i].arr = (dm_dmt*)malloc(sizeof(dm_dmt) * dmt.arr[i].capacity);
		fread(dmt.arr[i].arr, sizeof(dm_dmt), dmt.arr[i].size, f);
	}

	fclose(f);

	return dmt;
}