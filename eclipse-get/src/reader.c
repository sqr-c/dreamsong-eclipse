#ifdef UNIX
#include <stdio.h>
#include <dirent.h>
#include <string.h>

#include "reader.h"
#include "parser.h"

#define mod(a, b) ( (((a) % (b)) + (b)) % (b) )

char dir_eid_process(const char* dir)
{
	char ret = 1;

	typedef struct dirent dirent;

	DIR* dr = opendir(dir);
	if (dr == NULL) return 1;

	dirent* entry;

	int l = strlen(dir);

	for (; entry = readdir(dr);)
	{
		int ll = strlen(entry->d_name);
		if (0 == strcmp(&entry->d_name[mod(ll - sizeof(PATH_EID_EXT) + 1, ll)], PATH_EID_EXT))
		{
			ret = 0;

			char buf[512];
			strcpy(buf, dir);
			buf[l] = '/';
			strcpy(&buf[l + 1], entry->d_name);

			eid_parse_write(buf);
		}
	}

	return ret;
}

#else
#include <windows.h>

#include "reader.h"
#include "parser.h"

#define mod(a, b) ( (((a) % (b)) + (b)) % (b) )

char dir_eid_process(const char* dir)
{
	char ret = 1;

	wchar_t searchPattern[512];
	{
		char buf_searchPattern[512];
		sprintf(buf_searchPattern, "%s%s", dir, "\\*");

		for (int i = 0; buf_searchPattern[i - 1] != 0; i++)
			searchPattern[i] = buf_searchPattern[i];
	}

	WIN32_FIND_DATA findfiledata;
	HANDLE hfind = FindFirstFile(searchPattern, &findfiledata);

	if (hfind == INVALID_HANDLE_VALUE) return 1;

	int l = strlen(dir);

	do
	{
		if (!(findfiledata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			char buf[512];

			for (int i = 0; findfiledata.cFileName[i - 1] != 0; i++)
				buf[i] = findfiledata.cFileName[i];

			int ll = strlen(buf);
			if (0 == strcmp(&buf[mod(ll - sizeof(PATH_EID_EXT) + 1, ll)], PATH_EID_EXT))
			{
				ret = 0;

				char _buf[512];
				strcpy(_buf, dir);
				_buf[l] = '/';
				strcpy(&_buf[l + 1], buf);

				eid_parse_write(_buf);
			}
		}
	} while (FindNextFile(hfind, &findfiledata) != 0);

	return ret;
}

#endif