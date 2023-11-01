#include "dglobal.h"

#include "cachefn.h"
#include "tilefn.h"
#include "npcfn.h"
#include "breaktilefn.h"

#include "dmapgen.h"
#include "dynamicmap.h"

#include "debug.h"
#include "displayctrl.h"

#include <stdio.h>
#include <math.h>

int main(int argc, char* args[])
{
	if (display_init() == 0)
	{
		if (display_loadmedia() == 0) main_loop();
		else return 1;
	}
	else return 1;

	display_close();

	if (mapfp != NULL) fclose(mapfp);

	return 0;
}
