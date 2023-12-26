#pragma once

#include "map.h"
#include "mathfunction.h"

#include "path.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

void set_gtext_itemfound(int iid);
char cache_std_ex(cac_cache* c);
char add_cache(icac_itemcache* cac, unsigned char* top, int l, icac_itemcache* ic);
char cache_iswp0_ex(cac_cache* c);
char cache_prm(cac_cache* c);
char cache_std_rm(cac_cache* c);
char cache_dm_std_find(cac_cache* c);
char cache_null_fn(cac_cache* c);

#define CFN_CACHE_STD_RM_ID		4

#define NUM_CACHE_FN	12
extern char (*cfn[NUM_CACHE_FN])(cac_cache* c);