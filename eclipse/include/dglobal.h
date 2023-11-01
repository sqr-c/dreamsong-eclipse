#pragma once

#include "build.h"
#include SDL_SDL_INCLUDE

#include <stdint.h>

#define NUM_SPRITE	(737)

#define TERM_MENU_MAX	7

#define LS_DISPLAY_MAX	8
#define SPRITE_OFFSET (NUM_SPRITE - 30)
#define CHAR_OFFSET	1*14
#define TEXT_OFFSET	98
#define BORD_OFFSET	(223)
#define FLAT_OFFSET	(BORD_OFFSET+(6*2))
#define ELEM_OFFSET	(238)

#define BORD_IDX_SIZE	6
#define BORD_SIZE	16
#define TEXT_WIDTH	16
#define TEXT_HEIGHT	16
#define TEXT_X (TEXT_OFFSET - 74)
#define TEXT_1 (53 + TEXT_OFFSET)
#define TEXT_0 (TEXT_1+9)
#define TEXT_SPACE_INDEX	TEXT_OFFSET
#define CHARSET_LENGTH	125
#define CHARSET_END	(TEXT_OFFSET + CHARSET_LENGTH - 1)

#define SPRITE_MARKER_OFFSET	(658)

#define ELEM_AREA	128
#define ELEM_DIV	2
#define ELEM_W	(ELEM_AREA/ELEM_DIV)
#define ELEM_H	ELEM_W

#define RENDER_CYCLE_DELAY	100

#define ARENA_TICKET_ID	13
#define PARK_TICKET_ID	14

extern char gcdflag;

#include "display.h"
#include "sprite.h"
#include "map.h"
#include "character.h"
#include "spritearray.h"
#include "keybinding.h"
#include "mathfunction.h"
#include "npc.h"
#include "text.h"
#include "itemcache.h"
#include "save.h"
#include "breaktile.h"
#include "cast.h"
#include "castfn.h"
#include "market.h"
#include "iterfn.h"
#include "tscreenobj.h"
#include "dmtable.h"
#include "arena.h"
#include "park.h"
#include "usermap.h"
#include "package.h"

#include "itemcachefn.h"

char display_loadmedia();
void display_close();
void dctrl_init_texts();
void render();
void render_select_elem();
void render_elem_cache();
void dctrl_zero_menu_select();
char dctrl_menu_up_down_curs_set(SDL_Keycode sym, int max);
char dctrl_menu_1_up_down_curs_set(SDL_Keycode sym, int max);
char dctrl_menu_2_up_down_curs_set(SDL_Keycode sym, int max);
void dctrl_exit_menu_set_tidx(int tidx);
void dctrl_toggle_elem_screen();
void touch(const char* file);
void dctrl_init_all();
void dctrl_free_all();
void map_joinmv();
char add_elem(ch_char* c, e_element* e);
int main(int argc, char* args[]);

extern char sel_elem_menu_ofs;

extern void(*renderfn)();
extern int calibrator;
extern int ls_ptr;
extern char status_bar_mode;
extern char statusr;

extern int gtidx;

extern char screen_mode;
extern char seltik;
extern char blit_tick_elem_cache;

#define WINDOW_TITLE	"ECLIPSE"

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	(SCREEN_WIDTH / (1 + (1.0f / 3)))
#define SCREEN_CX		((SCREEN_WIDTH) / 2)
#define SCREEN_CY		((SCREEN_HEIGHT) / 2)

#define T_BPTR_LIMIT_DEFAULT	2
extern char t_bptr_limit;
extern char t_bidx;
extern char t_aidx;
extern char t_width;

extern int tileo;

extern const Uint8* keys;
extern SDL_Rect screenrect;
extern SDL_Rect surfacerect;
extern SDL_Rect tilerect;
extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern unsigned short tilew, tileh;
extern SDL_Texture* tile;

extern char deltakey;
extern int chdx, chdy;

extern int npcai;

extern s_spritedata sprdata[NUM_SPRITE];
extern m_map map;

extern int s_mapw;
extern int s_maph;

extern int tik;

#define S_MAP_X MAP_SPLIT_WIDTH
#define S_MAP_Y S_MAP_X

#define CH_MOVE_BOUND(x) \
if (-1 != map.adj[(x)]) \
{ \
	mapseek = map.adj[(x)]; \
	map_joinmv(); \
}

#define ROTDIR(x)	(-((x) % 2) * (2 * ((x) / 2) - 1))

//debug code v
extern int debug_ch_collison;

extern void* trade_in;
extern void* trade_out;

#define EXIT_MENU()		screen_mode = SCREEN_MODE_NORMAL; menu_sel_idx_0 = 0
#define EXIT_MENU_ID(x)	screen_mode = SCREEN_MODE_NORMAL; menu_sel_idx_0 = (x); menu_sel_idx_1 = (x); menu_sel_idx_2 = (x)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*arr))

void sm_exit_disp_cache();

typedef enum
{
	SCREEN_MODE_NORMAL,
	SCREEN_MODE_DISP_CACHE,
	SCREEN_MODE_TEXT,
	SCREEN_MODE_REND_CACHE,
	SCREEN_MODE_DISP_TERM,
	SCREEN_MODE_DISP_MRKT,
	SCREEN_MODE_DISP_TRADE,
	SCREEN_MODE_DISP_ELEM,
	SCREEN_MODE_DISP_TRS,
	SCREEN_MODE_APPLY_CAS,
	SCREEN_MODE_MENU_CNFRM,
	SCREEN_MODE_UM_EDIT_ADD,
	SCREEN_MODE_UM_EDIT_RM,
	SCREEN_MODE_UM_EDIT_MENU,
	SCREEN_MODE_UM_EDIT_NEWMAP,
} SCREENMODE;

typedef struct
{
	int dspbrk;
	int dsplif;
} disp_displacement;

extern disp_displacement disps[2];

extern int disp_elem_idx;

char trade_item_elem();
extern char(*tradefn)();

extern it_iterfn_a cyfn;

extern DMTableFA dmtable;

int lifec(ch_char* c);

#define RAR_FN0_ADJ_DEF 65536	//PRE:	(pow(2, 16))
extern int rar_fn0_adj;

#define RARITY_FN0_MAX	((int)pow(2, 8))
unsigned short rarity_fn_0(short x, int idx, int len);
unsigned short rarity_ash_fn0(short x, int idx, int len);
unsigned short rarity_fn_pkg(short x, int idx, int len);

extern unsigned short (*rar_fn[])(short x, int idx, int len);

void rdr_flsh_scr();

void render_end();
void ch_action_pause();
extern void(*disp_elem_close)();

void init_cache();

typedef struct
{
	unsigned short cidx;
	unsigned short clen;
	unsigned char crarfidx;

	unsigned short eidx;
	unsigned short elen;
	unsigned char erarfidx;

	unsigned short nidx;
	unsigned short nlen;
	unsigned char nrarfidx;

	unsigned char crandevt;
	unsigned char erandevt;
} mst_mapstate;

typedef struct
{
	short x, y;
	int32_t seek;
	mst_mapstate* state;
} mst_tilemapstate;

typedef struct
{
	char cid;
	int	spr;
} CIDSPR;

extern char table_item_unique[];

extern CIDSPR pub_cache[];
extern e_element pub_elem[];
extern n_npc_min pub_ennpc[];
extern mst_mapstate pub_map_state[];

extern mst_tilemapstate tile_map_state[];

extern mst_mapstate ms_mapstate;
extern unsigned short rand_state;

void tile_set_map_state();

extern void (*select_none_fn)();