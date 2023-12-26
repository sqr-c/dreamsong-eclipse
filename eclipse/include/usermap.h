#pragma once

#include "breaktile.h"

#define MAT_FLOOR_BLANK_ID	(SPRITE_MAT_OFFSET + 10)
#define UM_STACK_MAX		(4 + 2)
#define UM_Z_DELTA			(.125f)
#define UM_Z_MAX			(32)

typedef struct
{
	unsigned char id;
	unsigned short pos;
	int cd;
} um_tilerefresh;

typedef struct
{
	um_tilerefresh* arr;
	short size;
	short capacity;
} um_tilerefresh_a;

um_tilerefresh_a create_UMRefreshA(int capacity);
void free_UMRefreshA(um_tilerefresh_a* array);
void resize_array_UMRefreshA(um_tilerefresh_a* array, int new_capacity);
void push_back_UMRefreshA(um_tilerefresh_a* array, um_tilerefresh* value);

char um_newmap_init_texts();
void um_blink_tile();
void um_map_save();
void um_map_remove();
void um_map_load();
void um_map_exit();
void um_map_in_init();
void um_map_out_init();
void um_map_new(unsigned char w, unsigned char h);
void um_rm_all_mat();
void um_rm_stack_mat();
void um_rm_mat();
void um_add_mat();
void um_edit_rm_mat(char dx, char dy, char dz);
void um_edit_add_position(char dx, char dy, char dz);

void select_mapctrl_fn();

extern unsigned short um_tile_id[];
extern char um_tile_flag[];

extern unsigned char usermap_bt_id[];
extern bt_breaktile_min usermap_bt_struct[];

extern unsigned char um_edit_x, um_edit_y, um_edit_z;
extern unsigned char um_edit_id;

extern um_tilerefresh_a um_refresha;

#define UM_MAX_MAP_WIDTH	16
#define UM_MAX_MAP_HEIGHT	16