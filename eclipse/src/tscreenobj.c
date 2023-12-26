#include "tscreenobj.h"
#include "mathfunction.h"

#include <stdlib.h>

extern int tik;

tsc_tscreenobj_a tsoa;

void apply_TScreenObj(tsc_tscreenobj* t)
{
	push_back_sprarr(&map.arr[1]
		.tile
		.arr[t->pos], &t->v);
}

void apply_TScreenObjA(tsc_tscreenobj_a* arr)
{
	for (int i = 0; i < arr->size; i++)
		push_back_sprarr(&map.arr[1]
			.tile
			.arr[arr->arr[i].pos], &arr->arr[i].v);
}

void rm_TScreenObjA(tsc_tscreenobj_a* arr, int i)
{
	map.arr[1]
		.tile
		.arr[arr->arr[i].pos]
		.size--;

	arr->arr[i] = arr->arr[arr->size - 1];
	arr->size--;
}

void clear_TScreenObjA(tsc_tscreenobj_a* arr)
{
	for (; arr->size;) rm_TScreenObjA(arr, 0);
}

void update_TScreenObjA(tsc_tscreenobj_a* arr)
{
	for (int i = 0; i < arr->size; i++)
		if (arr->arr[i].cd < tik
			&& arr->arr[i].cd)
			rm_TScreenObjA(arr, i);
}

tsc_tscreenobj_a create_TScreenObjA(int capacity)
{
	tsc_tscreenobj_a buf;
	buf.arr = (tsc_tscreenobj*)malloc(capacity * sizeof(tsc_tscreenobj));
	buf.size = 0;
	buf.capacity = capacity;
	return buf;
}

void free_TScreenObj(tsc_tscreenobj_a* array)
{
	free(array->arr);
}

void resize_array_TScreenObj(tsc_tscreenobj_a* array, int new_capacity)
{
	tsc_tscreenobj* new_arr = (tsc_tscreenobj*)malloc(new_capacity * sizeof(tsc_tscreenobj));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_TScreenObj(tsc_tscreenobj_a* array, tsc_tscreenobj* value)
{
	if (array->size == array->capacity)
		resize_array_TScreenObj(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}