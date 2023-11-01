#include <stdio.h>

#include "parser.h"
#include "reader.h"

int main()
{
	path_init_path();

	char buf = 0;

	{
		get_eid_entry foo;

		foo.mode = GET_EID_MODE_PACKAGE;
		pkg_pid_entry bar;
		bar.id = 50;
		bar.seed = 1333;
		foo.size = sizeof(bar);
		foo.data = malloc(foo.size);
		memcpy(foo.data, &bar, sizeof(bar));

		//here, temp code, here
		eid_write_ex(&foo, "/home/ubuntu/Downloads/foo.eid", "wb");
	}

	const char* sdir[] = { EID_SEARCH_DIRS };
	for (int i = 0; i < sizeof(sdir) / sizeof(*sdir); i++)
	{
		char dirbuf[128];

		sruname(dirbuf, sdir[i]);

		if (0 == dir_eid_process(dirbuf))
		{
			buf = 1;
			printf("Success: received package!\n");
		}
	}
	if (0 == buf)
		printf("Could not find packages!\n");

	return 0;
}
