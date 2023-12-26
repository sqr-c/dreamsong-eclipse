#pragma once

#include "sprite.h"

typedef struct
{
	s_sprite v;
	int pos;
	int cd;
} tsc_tscreenobj;

typedef struct
{
	tsc_tscreenobj* arr;
	int size;
	int capacity;
} tsc_tscreenobj_a;

extern tsc_tscreenobj_a tsoa;

void apply_TScreenObj(tsc_tscreenobj* t);
void apply_TScreenObjA(tsc_tscreenobj_a* arr);
void rm_TScreenObjA(tsc_tscreenobj_a* arr, int i);
void clear_TScreenObjA(tsc_tscreenobj_a* arr);
void update_TScreenObjA(tsc_tscreenobj_a* arr);

tsc_tscreenobj_a create_TScreenObjA(int capacity);
void free_TScreenObj(tsc_tscreenobj_a* array);
void resize_array_TScreenObj(tsc_tscreenobj_a* array, int new_capacity);
void push_back_TScreenObj(tsc_tscreenobj_a* array, tsc_tscreenobj* value);