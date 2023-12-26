#include "package.h"

icac_itemcache_min pkg_itemcache[] =
{
	//SECTION:	0 3
	//plants
	{ 43, 2, },
	{ 45, 3, },
	{ 42, 10, },
	//SECTION:	3 3
	//borders
	{ 33, 2, },
	{ 36, 3, },
	{ 39, 5, },
	//SECTION:	6 4
	//others
	{ 38, 2, },
	{ 44, 3, },
	{ 37, 2, },
	{ 42, 5, },
};

pkg_package pkg_table[] =
{
	//package item id, cache arr, cache length, num user gets from cache arr, elem arr, elem length (user gets at most 1)
	//plants
	{
		49, &pkg_itemcache[0], 3, 2,
		NULL, 0,
	},
	//borders
	{
		50, &pkg_itemcache[3], 3, 2,
		NULL, 0,
	},
	//others
	{
		51, &pkg_itemcache[6], 4, 2,
		NULL, 0,
	},
};

void pkg_writepid(pkg_pid_entry* arr, int len, const char* name)
{
	FILE* fp = fopen(name, "wb");
	fwrite(arr, sizeof(pkg_pid_entry), len, fp);
	fclose(fp);
}

void pkg_readpid(pkg_pid_entry* arr, int len, const char* name)
{
	FILE* fp = fopen(name, "rb");
	fread(arr, sizeof(pkg_pid_entry), len, fp);
	fclose(fp);
}