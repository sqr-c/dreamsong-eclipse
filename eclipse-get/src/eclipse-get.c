#include <stdio.h>

#include "parser.h"
#include "reader.h"

int main()
{
	path_init_path();

	char buf = 0;

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
