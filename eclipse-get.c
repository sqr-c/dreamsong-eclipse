#include <stdio.h>

#include "parser.h"

int main()
{
	get_eid_entry foo;

	foo.mode = GET_EID_MODE_PACKAGE;

	pkg_pid_entry bar;
	bar.id = 49;
	bar.seed = 0x1c0de;

	foo.data = &bar;
	foo.size = sizeof(bar);

	remove(PATH_EID);

	eid_write(&foo, PATH_EID);

	eid_parse_write(PATH_EID);

	return 0;
}