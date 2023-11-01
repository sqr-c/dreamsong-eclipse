#pragma once

#include "character.h"
#include "itemcache.h"

#include <stdint.h>

extern e_element park_elem[];
#define PARK_ELEM_LEN	(sizeof(park_elem) / sizeof(e_element))

extern short park_elem_sel;
extern short park_elem_i;
extern char park_elem_l;

extern icac_itemcache park_cac[];

extern short park_cac_i;
extern char park_cac_l;
extern short park_cac_sel[4];
#define PARK_CAC_SEL_LEN	(sizeof(park_cac_sel) / sizeof(short))

extern int32_t park_seek;
extern short park_ix;
extern short park_iy;

void end_park();
void trs_exit_park();

#define PARK_ELEM_LENGTH (sizeof(park_elem)/sizeof(e_element))