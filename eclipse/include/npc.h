#pragma once

#include "character.h"
#include "text.h"
#include "save.h"

#define NPC_MOVE_FREQ	1000

#define collision_char_char(a, b) ((int)((a)->dx) == (int)((b)->sprite.x) && (int)((a)->dy == (b)->sprite.y))

extern float castp;
extern int castc;

extern unsigned int xrseed;

extern int npcidx;
extern int npcai;

typedef enum
{
	NPC_STATE_DEFAULT,
	NPC_STATE_INACTIVE,
	NPC_STATE_ENEMY,
	NPC_STATE_REMOVED = -1,
} N_NPC_STATE;

typedef struct
{
	ch_char ch;

	int ix;
	int iy;
	short walk_d;

	char cix;
	char ciy;

	int textidx;
	int fnidx;

	short move_seed;

	char istate;

	short nsmid;
} n_npc;

typedef struct 
{
	n_npc* arr;
	int size;
	int capacity;
} n_npc_a;

typedef struct
{
	e_element eelem[8];
	float estr;
	int espr;
	int ecycl;

	int sprite_id;
} n_npc_min;

#define XORRAND_OFS 0xC0DE

unsigned int xorrand();
n_npc_a create_NPCA(int capacity);
void free_NPCA(n_npc_a* array);
void resize_array_NPCA(n_npc_a* array, int new_capacity);
void push_back_NPCA(n_npc_a* array, n_npc* value);
n_npc* get_element_NPC(n_npc_a* array, int index);
char rayselect(n_npc_a* arr, ch_char* ch);
void rm_npc(n_npc* npc);
void set_npc_init_state_dm(n_npc* buf, int ix, int iy, float m);

void readnsm(const char* path, n_npc* n);
void writensm(const char* path, n_npc* n);
void clearnsm(const char* path);

void en_npc_update();
void push_en_npc(n_npc_min* npcm);
void push_en_npc_ex(n_npc_min* npcm, int x, int y, char sign);
void init_en_npc(n_npc* buf, n_npc_min* npcm);