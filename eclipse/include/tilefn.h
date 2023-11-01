#pragma once

#include "map.h"

extern m_map map;

extern int ddx;

void rm_tfn(m_tilefn_a* tfna, int idx);

char tile_fn_0(m_tilefn* t);

void init_tfn_rands(int x, int y);

extern char (*tilefn[])(m_tilefn* t);

char ash_ennpc(char ashmin, char ashmax, n_npc_min* arr, int x, int y);
void push_en_npc_ex(n_npc_min* npcm, int x, int y, char sign);