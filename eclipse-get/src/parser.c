#include "parser.h"

#include "../../eclipse/include/save.h"
#include "../../eclipse/include/cachefn.h"

#include <stdio.h>

void eid_write_ex(get_eid_entry* entry, const char* name, const char* mode)
{
	FILE* fp = fopen(name, mode);
	fwrite(&entry->mode, sizeof(entry->mode), 1, fp);
	fwrite(&entry->size, sizeof(entry->size), 1, fp);
	fwrite(entry->data, 1, entry->size, fp);
	fclose(fp);
}

void eid_parse_write(const char* name)
{
	FILE* fp = fopen(name, "rb");

	get_eid_entry entry;

	char str[PATH_DIR_DYNAMIC_MAX_LEN];
	char sbuf[PATH_DIR_DYNAMIC_MAX_LEN];

	for (; EOF != fgetc(fp);)
	{
		fseek(fp, -1, SEEK_CUR);

		fread(&entry.mode, sizeof(entry.mode), 1, fp);
		fread(&entry.size, sizeof(entry.size), 1, fp);

		{
			if (entry.mode == GET_EID_MODE_PACKAGE)
			{
				pkg_pid_entry buf;

				fread(&buf, sizeof(buf), 1, fp);

				//write the pid
				{
					sprintf(sbuf, "%s%s", PATH_D_MAIN_PREFIX, PATH_PID);
					sruname(str, sbuf);

					FILE* fp = fopen(str, "ab");
					fwrite(&buf, sizeof(pkg_pid_entry), 1, fp);
					fclose(fp);
				}

				//overwrite save file
				{
					sprintf(sbuf, "%s%s", PATH_D_MAIN_PREFIX, PATH_SED);
					sruname(str, sbuf);

					readsed(str);

					icac_itemcache cbuf;
					cbuf.active = 0;
					cbuf.id = buf.id;
					cbuf.mul = 1;

					add_cache(cache, &cache_top, ICACHE_MAX_SIZE, &cbuf);

					writesed(str);
				}
			}
		}
	}

	fclose(fp);
	remove(name);
}