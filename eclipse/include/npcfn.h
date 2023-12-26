#pragma once

#include "npc.h"

char npc_give_item_0(n_npc* npc);
char init_market(int midx);
char npc_init_market_0(n_npc* npc);
char npc_gem_0(n_npc* npc);
char npc_init_arena(n_npc* npc);
char npc_cmp_arena(n_npc* npc);

extern char (*npcfn[])(n_npc*);