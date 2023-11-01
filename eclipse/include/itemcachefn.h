#pragma once

#include "itemcache.h"

typedef struct
{
	short id;
	short idx;
} ICID_ICIDFN;

typedef struct
{
	char id;
	char val;
} ID_VAL_CHAR;

void apply_ic(int i);
void apply_all_ic();

char itmcacfn_refil_spr(icac_itemcache* c);
char itmcacfn_refil_lif(icac_itemcache* c);
char itmcacfn_set_cas(icac_itemcache* c);
char itmcacfn_apply_gem(icac_itemcache* c);
char itmcacfn_tport(icac_itemcache* c);
char itmcacfn_apply_espr(icac_itemcache* c);
char itmcacfn_apply_spr(icac_itemcache* c);
char itmcacfn_apply_mat(icac_itemcache* c);
char itmcacfn_apply_pkg(icac_itemcache* c);

char apply_setcac();

extern ICID_ICIDFN icid_icidfn[];
#define ICID_ICIDFN_LENGTH	(sizeof(icid_icidfn) / sizeof(ICID_ICIDFN))

extern ID_VAL_CHAR icid_mat[];

extern char (*itmcacfn[])(icac_itemcache* c);

#define GEM_RNG_UNIT	RAR_FN0_ADJ_DEF