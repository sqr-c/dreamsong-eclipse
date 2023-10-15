#include "parser.h"

#include "../eclipse/save.h"
#include "../eclipse/cachefn.h"

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
					FILE* fp = fopen(PATH_RELATIE_MAIN PATH_PID, "ab");
					fwrite(&buf, sizeof(pkg_pid_entry), 1, fp);
					fclose(fp);
				}

				//overwrite save file
				{
					readsed(PATH_RELATIE_MAIN PATH_SED);

					icac_itemcache cbuf;
					cbuf.active = 0;
					cbuf.id = buf.id;
					cbuf.mul = 1;

					add_cache(cache, &cache_top, ICACHE_MAX_SIZE, &cbuf);

					writesed(PATH_RELATIE_MAIN PATH_SED);
				}
			}
		}
	}

	fclose(fp);
	remove(name);
}