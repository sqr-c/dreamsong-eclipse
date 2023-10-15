#pragma once

#include "character.h"

#include "npc.h"

#include <stdlib.h>

typedef struct 
{
	ch_char* ch;
	ch_char* ech;

	int ix;
	int iy;

	char state;
} c_cast;

typedef struct 
{
	c_cast* arr;
	int size;
	int capacity;
} c_cast_a;

c_cast_a create_CastA(int capacity);
void free_CastA(c_cast_a* array);
void resize_array_CastA(c_cast_a* array, int new_capacity);
void push_back_CastA(c_cast_a* array, c_cast* value);

char assign_cast(e_element* e, int idx);