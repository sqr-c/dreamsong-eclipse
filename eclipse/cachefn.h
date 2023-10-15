#pragma once

#include "map.h"
#include "mathfunction.h"

#include "path.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

void set_gtext_itemfound(int iid);
char cache_std_ex(cac_cache* c);
char add_cache(icac_itemcache* cac, int* top, int l, icac_itemcache* ic);
char cache_iswp0_ex(cac_cache* c);
char cache_prm(cac_cache* c);
char cache_std_rm(cac_cache* c);
char cache_dm_std_find(cac_cache* c);
char cache_null_fn(cac_cache* c);

#define NUM_CACHE_FN	11
extern char (*cfn[NUM_CACHE_FN])(cac_cache* c);