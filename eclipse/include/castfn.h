#pragma once

#include "cast.h"

#include "cachefn.h"
#include "breaktilefn.h"

typedef struct 
{
	char (*fn)(c_cast*);
	c_cast fnargs;
	int tick;
	int t_rm;
} intc_intcast;

typedef struct 
{
	intc_intcast* arr;
	int size;
	int capacity;
} intc_intcast_a;

extern intc_intcast_a intcasts;

void get_ixiy(int* x, int* y, ch_char* c);
void apply_cast(short* cast, int idx, c_cast* v, ch_char* c);
void update_acttick(ch_char* ch);
intc_intcast_a create_IntCastA(int capacity);
void free_IntCast(intc_intcast_a* array);
void resize_array_IntCast(intc_intcast_a* array, int new_capacity);
void push_back_IntCast(intc_intcast_a* array, intc_intcast* value);
void rm_IntCast(intc_intcast_a* array, int idx);

extern char (*castfn[])(c_cast* c);
#define NUM_CASTFN	(sizeof(castfn)/sizeof(castfn[0]))
#define NUM_ICASFN	(sizeof(icasfn)/sizeof(icasfn[0]))

char cast0(c_cast* c);
char cast1(c_cast* c);
char cast_flare(c_cast* c);
char cast_disperse(c_cast* c);
char cast_suspension(c_cast* c);

char cast_dsp_illuminate(c_cast* c);

char icas_fume_wave(c_cast* c);
char icas_surge(c_cast* c);
char icas_gloomyhaze(c_cast* c);
char icas_sunder_cast(c_cast* c);

char cast_debug_rm(c_cast* c);

void itfn_apply_icas();

void dm_apply_dig();

extern char digmode;

#define CAST_DIVE_ID	69