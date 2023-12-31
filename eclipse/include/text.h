#pragma once

#define TEXT_DOT_IDX	(SPRITE_TEXT_OFFSET + 71)
#define TEXT_SPACE_IDX	(SPRITE_TEXT_OFFSET + 0)
#define TEXT_QMRK_IDX	(SPRITE_TEXT_OFFSET + 96)

#include "spritearray.h"

#define TEXT_ARROW_R_IDX	(110 + SPRITE_TEXT_OFFSET)
#define TEXT_ARROW_D_IDX	(110 + SPRITE_TEXT_OFFSET+2)

#define CHARSET	" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-=[]\\;',./`~!@#$%^&*()_+{}|:\"\"\"<>?"
#define chtotid(c)	(int)(strchr(CHARSET, c) - CHARSET)

#define GTEXT_SIZE				157
#define CACHE_FOUND_SIZE		16
#define CACHE_FOUND_TEXT_SIZE	12
#define CACHE_FOUND_TOTAL_SIZE	(CACHE_FOUND_SIZE + CACHE_FOUND_TEXT_SIZE)
#define ITEM_FOUND_TXT_IDX		1

#define TEXT_CAST_OFFSET		(157 - 71)

#define TEXT_TRADE0_IDX			(33)
#define TEXT_TRADE1_IDX			(83)
#define TEXT_D2NUM_IDX			(84)
#define TEXT_UM_NEWMAP_NAME_IDX	(83 - 10)

#define TEXT_TRADE_EXIT_STD		15

extern char t_bptr_limit;
extern char t_bidx;
extern char t_aidx;
extern char t_width;

extern s_spritedata_a gtexta[GTEXT_SIZE];
extern const char* glob_textstr[GTEXT_SIZE];

void inc_textb(s_spritedata_a* t, int inc);
s_spritedata_a create_SpriteDataArr(int capacity);
void free_SpriteDataArr(s_spritedata_a* array);
char empty_SpriteDataArr(s_spritedata_a* array);
int get_size_SpriteDataArr(s_spritedata_a* array);
int get_capacity_SpriteDataArr(s_spritedata_a* array);
void resize_array_SpriteDataArr(s_spritedata_a* array, int new_capacity);
void push_back_SpriteDataArr(s_spritedata_a* array, s_spritedata** value);
extern s_spritedata sprdata[];
void push_back_sprdataa_str(s_spritedata_a* array, const char* str);
s_spritedata* pop_back_SpriteDataArr(s_spritedata_a* array);
s_spritedata** get_element_SpriteDataArr(s_spritedata_a* array, int index);
void set_element_SpriteDataArr(s_spritedata_a* array, int index, s_spritedata* value);
void set_text_txt(s_spritedata_a* s, s_spritedata_a* s1, int tbufl, int toffs, s_spritedata* tail);
void set_text_char(s_spritedata_a* t, s_spritedata* ch, int a, int b);
void set_trade1_text_num(int num, s_spritedata_a* a, s_spritedata_a* b);