#include "path.h"

void dbg_rmdata()
{
	remove(PATH_CID);
	remove(PATH_NSM);
	remove(PATH_SED);
}

void dbg_tcdata()
{
	touch(PATH_NSM);
	touch(PATH_CID);
	touch(PATH_PID);
	touch(PATH_BDB);
	touch(PATH_SED);
	touch(PATH_LAS);
}

void dbg_rmtcdata()
{
	dbg_rmdata();
	dbg_tcdata();
}
