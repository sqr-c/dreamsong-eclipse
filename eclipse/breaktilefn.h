#pragma once

#include "breaktile.h"
#include "dglobal.h"

extern short cast_break_value;

char btfn0(bt_breaktile* t);
char btfn1(bt_breaktile* t);
char btfn_park_cac(bt_breaktile* t);
char btfn_park_elem(bt_breaktile* t);

#define BREAK_TILE_FN_SIZE	14
extern char (*btfn[BREAK_TILE_FN_SIZE]) (bt_breaktile* t);