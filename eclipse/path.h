#pragma once

#include "build.h"

//const,	map,
#define PATH_SMD	"./static/.smd"
//var,		npc,
#define PATH_NSM	".nsm"
//var,		cache,
#define PATH_CID	".cid"
//const,	dig map table,
#define PATH_DMT	"./static/.dmt"
//var,		package,
#define PATH_PID	".pid"

//const,	tile draw,
#define PATH_STD	"./static/.std"
//const,	tile pixel,
#define PATH_BMP	"./static/.bmp"

//var,		breaktile buffer,
#define PATH_BDB	".bdb"

//var,		save,
#define PATH_SED	".sed"
//var,		load stack,
#define PATH_LAS	".las"

//var,		dig map,
extern char PATH_VAR_DMD[];
//ptr,		map being utilized,
extern const char* PATH_VAR_MAP;