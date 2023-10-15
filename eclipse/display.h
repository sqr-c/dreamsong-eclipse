#pragma once

#include "spritearray.h"
#include "itemcache.h"
#include "character.h"
#include "npc.h"

#define UPDATE_TICK()	(seltik = ((tik / (int)(256 * 1.5f))) % 2)

#define blit_ctg_menu_crn(idx, h, w) blit_ctg_menu_crn_ex(idx, h, w, menu_sel_idx_0, 0)

#define STATUS_BAR_LF_MAX	(24)

typedef struct 
{
	int e0;
	int e1;

	int s0;
	int sp0;

	int s1;
	int sp1;

	int l0;
	int l1;
} stat_bar_dsp;

void blit_ch(ch_char* ch);
void blit_npc(n_npc* npc, float x, float y, float z);
void isoblit_flip(s_spritedata* sd, float x, float y);
void quadblit(s_spritedata* sd, float x, float y);
void isoblit(s_spritedata* sd, float x, float y);
void isoblit_z_flip(s_spritedata* sd, float x, float y, float z);
void isoblit_z(s_spritedata* sd, float x, float y, float z);

void blit_ls(icac_itemcache* cache, int cache_top, int cursor);
void blit_textbox_std(s_spritedata_a* t, int x, int y, int w, int h);
void blit_elem(int idx, int xp, int yp);
void blit_column(int id, int x, int y0, int y1, char flip);
void blit_border(int id, int x0, int y0, int x1, int y1);
void blit_status_bar(stat_bar_dsp* p);
void blit_elem_cache(e_element* cache, int sprb, int token);
int* blit_transition();
void blit_ls_cursor(s_spritedata* at, int x0, int y0);

void blit_text_cursor(int x, int y);
void blit_ctg_menu_txt(int idx, int h, int w);
void blit_ctg_menu_cnt(int idx, int h, int w);
void blit_ctg_menu_crn_ex(int idx, int h, int w, int curs, int offs);
void blit_text_str_cnt(char* str, char blocksize);
void blit_cast_menu(short* casts);
void blit_confirm_menu();
void blit_text(s_spritedata_a* txt, int x0, int y0, int lo, int to);
void blit_fill_rect(unsigned char lum);
void blit_delay();