#include "path.h"

#include <stdio.h>

//var,
char path_userdir_dynamic[PATH_DIR_DYNAMIC_MAX_LEN];

//var,		dig map,
char PATH_DMD[PATH_DIR_DYNAMIC_MAX_LEN];
//ptr,		map being utilized,
const char* PATH_VAR_MAP = PATH_SMD;

//var,		npc,
char path_path_nsm[PATH_DIR_DYNAMIC_MAX_LEN];
//var,		cache,
char path_path_cid[PATH_DIR_DYNAMIC_MAX_LEN];
//var,		package,
char path_path_pid[PATH_DIR_DYNAMIC_MAX_LEN];
//var,		breaktile buffer,
char path_path_bdb[PATH_DIR_DYNAMIC_MAX_LEN];
//var,		save,
char path_path_sed[PATH_DIR_DYNAMIC_MAX_LEN];
//var,		load stack,
char path_path_las[PATH_DIR_DYNAMIC_MAX_LEN];
//var,		dig map,
char path_path_dmd[PATH_DIR_DYNAMIC_MAX_LEN];

#ifdef UNIX

#include <unistd.h>
#include <string.h>

void get_username(char* buf)
{
	strcpy(buf, getlogin());
}

#else

#include <windows.h>

void get_username(char* buf)
{
	wchar_t wbuf[20 + 1];
	int len;
	GetUserName(wbuf, &len);

	int i;
	for (i = 0; wbuf[i]; i++) buf[i] = wbuf[i];
	buf[i] = 0;
}

#endif

void sruname(char* buf, const char* str)
{
	int o = 0;
	int i;

	for (i = 0; str[i]; i++)
		if (str[i] == '~')
		{
			char ubuf[20 + 1];
			get_username(ubuf);
			o = strlen(ubuf) - 1;
			memcpy(&buf[i], ubuf, strlen(ubuf));
		}
		else
			buf[i + o] = str[i];
	buf[i + o] = 0;
}

void path_init_path()
{
	sruname(path_userdir_dynamic, PATH_DYNAMIC_PREFIX);

	sprintf(path_path_nsm, "%s%s", path_userdir_dynamic, PATH_NSM_EXT);
	sprintf(path_path_cid, "%s%s", path_userdir_dynamic, PATH_CID_EXT);
	sprintf(path_path_pid, "%s%s", path_userdir_dynamic, PATH_PID_EXT);
	sprintf(path_path_bdb, "%s%s", path_userdir_dynamic, PATH_BDB_EXT);
	sprintf(path_path_sed, "%s%s", path_userdir_dynamic, PATH_SED_EXT);
	sprintf(path_path_las, "%s%s", path_userdir_dynamic, PATH_LAS_EXT);

	{
		int i;
		char buf[PATH_DMD_NAME_LEN + 1];
		for (i = 0; i < sizeof(buf) / sizeof(*buf) - 1; i++)
			buf[i] = 'x';
		buf[i] = 0;
		sprintf(path_path_dmd, "%s%s%s", path_userdir_dynamic, buf, PATH_DMD_EXT);
	}
}
