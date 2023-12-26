#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "spritearray.h"

s_sprite_a create_sprarr(int capacity)
{
	s_sprite_a array;
	array.arr = (s_sprite*)malloc(capacity * sizeof(s_sprite));
	array.size = 0;
	array.capacity = capacity;
	return array;
}

void free_sprarr(s_sprite_a* array)
{
	free(array->arr);
}

void resize_array_sprarr(s_sprite_a* array, int new_capacity)
{
	s_sprite* new_arr;
	new_arr = (s_sprite*)malloc(new_capacity * sizeof(s_sprite));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_sprarr(s_sprite_a* array, s_sprite* value)
{
	if (array->size == array->capacity)
		resize_array_sprarr(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}

s_sprite_aa create_spraa(int capacity)
{
	s_sprite_aa array;
	array.arr = (s_sprite_a*)malloc(capacity * sizeof(s_sprite_a));
	array.size = 0;
	array.capacity = capacity;
	return array;
}

void free_spraa(s_sprite_aa* array)
{
	for (int i = 0; i < array->size; i++)
		free_sprarr(&(array->arr[i]));
}

void resize_array_spraa(s_sprite_aa* array, int new_capacity)
{
	s_sprite_a* new_arr = (s_sprite_a*)malloc(new_capacity * sizeof(s_sprite_a));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_spraa(s_sprite_aa* array, s_sprite_a* value)
{
	if (array->size == array->capacity)
		resize_array_spraa(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}

s_spritedata_a create_sprdataa(int capacity)
{
	s_spritedata_a buf;
	buf.arr = (s_spritedata**)malloc(capacity * sizeof(s_spritedata*));
	buf.size = 0;
	buf.capacity = capacity;
	return buf;
}

void free_sprdata(s_spritedata_a* array)
{
	free(array->arr);
}

void resize_sprdataa(s_spritedata_a* array, int new_capacity)
{
	s_spritedata** new_arr = (s_spritedata**)malloc(new_capacity * sizeof(s_spritedata*));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_sprdataa(s_spritedata_a* array, s_spritedata** value)
{
	if (array->size == array->capacity)
		resize_sprdataa(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}