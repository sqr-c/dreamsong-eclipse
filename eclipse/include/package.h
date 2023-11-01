#pragma once

#include "itemcache.h"
#include "character.h"

#include "path.h"

typedef struct 
{
	unsigned short id;
	unsigned int seed;
} pkg_pid_entry;

typedef struct 
{
	unsigned short id;

	icac_itemcache_min* icache_arr;
	char icache_len;
	char icache_sel;

	e_element_min* elem_arr;
	char elem_len;
} pkg_package;

void pkg_writepid(pkg_pid_entry* arr, int len, const char* name);
void pkg_readpid(pkg_pid_entry* arr, int len, const char* name);

extern icac_itemcache_min pkg_itemcache[];
extern pkg_package pkg_table[];