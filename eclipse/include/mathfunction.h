#pragma once

#include "sprite.h"
#include "map.h"
#include "spritearray.h"

#include "npc.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define sign(x)	((x) < 0 ? -1 : +1)

#define ceil(x)	((x)+!!((x)-(int)(x)))
#define floor(x)	((int)(x))

#define xswap(a, b) (a) ^= (b); (b) ^= (a); (a) ^= (b)

#define fpart(x)	((x)-((int)(x)))
#define pm(x, m)	( (((x)%(m)) + (m)) % (m) )

#define TU(t)	(((float)t)*64)

char collision_npca_char(n_npc_a* npca, ch_char* ch);

point pt_world_screen(float x, float y, float z);

void fnv1a_hash_str(const char* key, int keysize, char* str);
unsigned int fnv1a_hash(char* key, int size);