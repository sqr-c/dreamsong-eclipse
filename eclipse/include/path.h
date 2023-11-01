#pragma once

#include "build.h"

void sruname(char* buf, const char* str);
void get_username(char* buf);

#ifndef PATH_DYNAMIC_PREFIX
#define PATH_DYNAMIC_PREFIX	"./dynamic/"
#endif

#ifndef PATH_STATIC_PREFIX
#define PATH_STATIC_PREFIX	"./static/"
#endif

#ifdef UNIX
#ifndef PATH_DIR_DYNAMIC_MAX_LEN
#define PATH_DIR_DYNAMIC_MAX_LEN	(64)
#endif

#else

#ifndef PATH_DIR_DYNAMIC_MAX_LEN
#define PATH_DIR_DYNAMIC_MAX_LEN	(64)
#endif

#endif

extern char path_userdir_dynamic[PATH_DIR_DYNAMIC_MAX_LEN];

#define PATH_SMD_EXT	".smd"
#define PATH_DMT_EXT	".dmt"
#define PATH_STD_EXT	".std"
#define PATH_BMP_EXT	".bmp"

//const,	map,
#define PATH_SMD	PATH_STATIC_PREFIX	PATH_SMD_EXT
//const,	dig map table,
#define PATH_DMT	PATH_STATIC_PREFIX	PATH_DMT_EXT
//const,	tile draw,
#define PATH_STD	PATH_STATIC_PREFIX	PATH_STD_EXT
//const,	tile pixel,
#define PATH_BMP	PATH_STATIC_PREFIX	PATH_BMP_EXT

#define PATH_NSM_EXT	".nsm"
#define PATH_CID_EXT	".cid"
#define PATH_PID_EXT	".pid"
#define PATH_BDB_EXT	".bdb"
#define PATH_SED_EXT	".sed"
#define PATH_LAS_EXT	".las"
#define PATH_DMD_EXT	".dmd"

//var,		npc,
extern char path_path_nsm[PATH_DIR_DYNAMIC_MAX_LEN];
//var,		cache,
extern char path_path_cid[PATH_DIR_DYNAMIC_MAX_LEN];
//var,		package,
extern char path_path_pid[PATH_DIR_DYNAMIC_MAX_LEN];
//var,		breaktile buffer,
extern char path_path_bdb[PATH_DIR_DYNAMIC_MAX_LEN];
//var,		save,
extern char path_path_sed[PATH_DIR_DYNAMIC_MAX_LEN];
//var,		load stack,
extern char path_path_las[PATH_DIR_DYNAMIC_MAX_LEN];
//var,		dig map,
extern char path_path_dmd[PATH_DIR_DYNAMIC_MAX_LEN];

//var,		npc,
#define PATH_NSM	path_path_nsm
//var,		cache,
#define PATH_CID	path_path_cid
//var,		package,
#define PATH_PID	path_path_pid
//var,		breaktile buffer,
#define PATH_BDB	path_path_bdb
//var,		save,
#define PATH_SED	path_path_sed
//var,		load stack,
#define PATH_LAS	path_path_las
//var,		dig map,
#define PATH_DMD	path_path_dmd

#define PATH_DMD_NAME_LEN	4

//ptr,		map being utilized,
extern const char* PATH_VAR_MAP;
