#include "breaktile.h"
#include "mathfunction.h"

#include <stdlib.h>

bt_bthmupdate_a bthmupdate;

bt_breaktile_h create_breaktileh_wh(int w, int h)
{
	bt_breaktile_h buf;

	buf.w = w;
	buf.h = h;

	buf.size = buf.capacity = w * h;
	buf.arr = (bt_breaktile*)malloc(sizeof(bt_breaktile) * buf.capacity);

	return buf;
}

bt_breaktile_h create_breaktileh(int capacity)
{
	bt_breaktile_h buf;
	buf.arr = (bt_breaktile*)malloc(capacity * sizeof(bt_breaktile));
	buf.size = 0;
	buf.capacity = capacity;
	return buf;
}

void free_breaktileh(bt_breaktile_h* array)
{
	free(array->arr);
}

void resize_breaktileh(bt_breaktile_h* array, int new_capacity)
{
	bt_breaktile* new_arr = (bt_breaktile*)malloc(new_capacity * sizeof(bt_breaktile));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_breaktileh(bt_breaktile_h* array, bt_breaktile* value)
{
	if (array->size == array->capacity)
		resize_breaktileh(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}

void resize_array_bthmupdatea(bt_bthmupdate_a* array, int new_capacity)
{
	int* new_arr = (int*)malloc(new_capacity * sizeof(int));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void remove_bthmupdatea(bt_bthmupdate_a* array, int idx)
{
	xswap(array->arr[idx], array->arr[array->size - 1]);
	array->size--;
}

void push_back_bthmupdatea(bt_bthmupdate_a* array, int value)
{
	if (array->size == array->capacity)
		resize_array_bthmupdatea(array, array->capacity * 2);

	array->arr[array->size] = value;
	array->size++;
}

bt_bthmupdate_a create_bthmupdatea(int capacity)
{
	bt_bthmupdate_a buf;
	buf.arr = (int*)malloc(capacity * sizeof(int));
	buf.size = 0;
	buf.capacity = capacity;
	return buf;
}

void free_bthmupdatea(bt_bthmupdate_a* buf)
{
	free(buf->arr);
}