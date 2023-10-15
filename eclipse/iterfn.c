#include "iterfn.h"

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

char iter_fn_contains(it_iterfn_a* arr, void(*v)())
{
	for (int i = 0; i < arr->size; i++)
		if (arr->arr[i] == v)
			return 1;
	return 0;
}

it_iterfn_a create_IterFNA(int capacity)
{
	it_iterfn_a buf;

	buf.arr = (void(**)()) malloc(capacity * sizeof(buf.arr));
	buf.size = 0;
	buf.capacity = capacity;
	return buf;
}

void free_IterFNA(it_iterfn_a* array)
{
	free(array->arr);
}

void resize_array_IterFNA(it_iterfn_a* array, int new_capacity)
{
	void(**new_arr)() = (void(**)()) malloc(new_capacity * sizeof(new_arr));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void rm_IterFNA(it_iterfn_a* arr, int idx)
{
	arr->arr[idx] =
		arr->arr[
			arr->size - 1
		];
	arr->size--;
}

void rmf_IterFNA(it_iterfn_a* arr, void(*v)(void))
{
	int i;
	for (i = 0; i < arr->size; i++)
		if (arr->arr[i] == v)
			break;
	if (i < arr->size)
		rm_IterFNA(arr, i);
}

void pushf_IterFNA(it_iterfn_a* arr, void(*v)(void))
{
	int i;
	for (i = 0; i < arr->size; i++)
		if (arr->arr[i] == v)
			break;
	if (i >= arr->size)
		push_back_IterFNA(arr, v);
}

void push_back_IterFNA(it_iterfn_a* array, void(*value)())
{
	if (array->size == array->capacity)
		resize_array_IterFNA(array, array->capacity * 2);

	array->arr[array->size] = value;
	array->size++;
}

#include "dglobal.h"

void itfn_umrefresh_update()
{
	static char state;
	char buf = (tik / (1024 * (2))) % 2;
	if (buf != state)
	{
		state = buf;

		for (int i = 0; i < um_refresha.size; i++)
		{
			if (um_refresha.arr[i].cd <= tik
				&& um_refresha.arr[i].pos != p2idx(ch.dx, ch.dy, map.arr[1].width))
			{
				s_sprite_a* sa = &map.arr[1]
					.tile.arr[um_refresha.arr[i].pos];

				if (-1 == sa->arr[sa->size - 1].id)
				{
					sa->arr[sa->size - 1].id
						= um_tile_id[um_refresha.arr[i].id];
					sa->arr[sa->size - 1].flag
						= um_tile_flag[um_refresha.arr[i].id];

					for (int ii = 0; ii < bthmupdate.size; ii++)
						if (bthmupdate.arr[ii] == um_refresha.arr[i].pos)
						{
							remove_bthmupdatea(&bthmupdate, ii);
							break;
						}

					um_refresha.arr[i] = um_refresha.arr[--um_refresha.size];
				}
			}
		}
	}
}

void itfn_auto_ref()
{
	static char state;
	char buf = (tik / (1024 * (8))) % 2;
	if (buf != state)
	{
		state = buf;

		if (!ch_in_combat())
		{
			ch.spra += ch.sprb / 32;
			if (ch.spra > ch.sprb)
				ch.spra = ch.sprb;

			ch.lifea += ch.lifeb / 32;
			if (ch.lifea > ch.lifeb)
				ch.lifea = ch.lifeb;
		}
	}
}