#pragma once

#include "sprite.h"
#include "map.h"
#include "spritearray.h"

#include "npc.h"

char collision_npca_char(n_npc_a* npca, ch_char* ch)
{
	for (int i = 0; i < npca->size; i++)
		if (collision_char_char(ch, &(npca->arr[i].ch)))
			return 1;
	return 0;
}

point pt_world_screen(float x, float y, float z)
{
	return (point)
	{
		x - y, x + y + z
	};
};

unsigned int fnv1a_hash(char* key, int size)
{
	unsigned int hash = 2166136261;
	const unsigned int fnv_prime = 16777619;

	for (; size--;)
	{
		hash ^= (unsigned int)(*key++);
		hash *= fnv_prime;
	}

	return hash;
}

//set a str of length 4 to hash'd hex's
void fnv1a_hash_str(char* key, int size, char* str)
{
	static const char* hex = "0123456789ABCDEF";
	unsigned int buf = fnv1a_hash(key, size);
	for (int i = 0; i < 4; i++)
		str[(4 - 1) - i] = hex[(buf / (int)pow(16, i)) % 16];
}