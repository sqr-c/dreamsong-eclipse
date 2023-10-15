#pragma once

#include <math.h>

#define BREAKM_MAX	32767	//PRE:	(pow(2, 15) - 1)
#define BREAKM_BLOCK	(BREAKM_MAX	/ 16)

typedef struct
{
	unsigned short breakm;
	unsigned char fnid;
} bt_breaktile_min;

typedef struct 
{
	short breakma;
	short breakmb;
	int breakfnid;
} bt_breaktile;

typedef struct
{
	bt_breaktile* arr;
	int size;
	int capacity;

	int w;
	int h;
} bt_breaktile_h;

bt_breaktile_h create_breaktileh_wh(int w, int h);
bt_breaktile_h create_breaktileh(int capacity);
void free_breaktileh(bt_breaktile_h* array);
void resize_breaktileh(bt_breaktile_h* array, int new_capacity);
void push_back_breaktileh(bt_breaktile_h* array, bt_breaktile* value);

#define get_bth_elem(b, x, y) &((b)->arr[p2idx((x), (y), (b)->w)])

typedef struct
{
	int* arr;
	int size;
	int capacity;
} bt_bthmupdate_a;

extern bt_bthmupdate_a bthmupdate;

void resize_array_bthmupdatea(bt_bthmupdate_a* array, int new_capacity);
void remove_bthmupdatea(bt_bthmupdate_a* array, int idx);
void push_back_bthmupdatea(bt_bthmupdate_a* array, int value);
bt_bthmupdate_a create_bthmupdatea(int capacity);
void free_bthmupdatea(bt_bthmupdate_a* buf);