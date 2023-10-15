#pragma once

#include "map.h"
#include "dglobal.h"

#define DM_LENGTH	(16+1)



extern dm_dmapmd* dm_dmmd;
extern dm_dmapmd* dm_dmmd_def;

void comp_dm_cachei();
void comp_dm_cachee();
void comp_dm_all();

void init_dm();
char gen_dyn_map_node(m_map* m, int loadidx);
long file_size(const char* path);
void en_npc_update();
void ch_transition_update();
void set_npc_end_state_dm(n_npc* buf, int ix, int iy, float m);
void set_npc_init_state_dm(n_npc* buf, int ix, int iy, float m);
void rm_npc(n_npc* npc);


void trs_new_load();
extern void (*trs_end_fn)();