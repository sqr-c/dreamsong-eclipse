#include "dynamicmap.h"

#include "dmapgen.h"
#include "dmtable.h"

long file_size(const char* path)
{
	FILE* file = fopen(path, "rb");

	if (file == NULL) return 0;

	long int size;

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	fclose(file);

	return size;
}

void init_dm()
{
	free_map(&map);

	m_map buf;

	dm_dmapmd dmmd;

	memset(&dmmd, 0, sizeof(dm_dmapmd));

	dmmd.endtick = 1;
	dmmd.pathl = 4;

	dm_generate(&buf, &dmmd, xrseed);

	m_maploader loadbuf;
	for (int i = 0; i < 4; i++)
	{
		short xp[] = { 8, 16, 8, 0 };
		short yp[] = { 16, 8, 0, 8 };

		loadbuf.ix = xp[i];
		loadbuf.iy = yp[i];
		loadbuf.ox = xp[(i + 2) % 4];
		loadbuf.oy = yp[(i + 2) % 4];
		loadbuf.seek = -1;
		push_back_mapload(&buf.loads, &loadbuf);

		for (int ii = -2; ii <= +2; ii++)
		{
			s_sprite* s = &buf.arr[1].tile.arr[p2idx(xp[i] + ii * ((i + 1) % 2), yp[i] + ii * ((i + 0) % 2), DM_LENGTH)].arr[0];

			if (ii && s->id == -1)
				s->flag = SPRITE_FLAG_POPDSTSTACK;
		}
	}
	tile_load_map_spec = &gen_dyn_map_node;

	dm_place_end(&buf);

	ch.sprite.x = DM_LENGTH / 2;
	ch.sprite.y = DM_LENGTH / 2;
	ch.dx = DM_LENGTH / 2;
	ch.dy = DM_LENGTH / 2;

	appendmap(&buf, PATH_VAR_DMD);
	free_map(&buf);

	map = readmapp(PATH_VAR_DMD, 0);
	linkmap();
}

char gen_dyn_map_node(m_map* m, int loadidx)
{
	//overwrite the map with the new load pointer
	if (mapfp != NULL) { fclose(mapfp); mapfp = NULL; }
	long fs = file_size(PATH_VAR_DMD);

	int preox = m->loads.arr[loadidx].ox;
	int preoy = m->loads.arr[loadidx].oy;
	{
		m_map buf = readmapp(PATH_VAR_DMD, mapseek);
		buf.loads.arr[loadidx].seek = fs;
		if (mapfp == NULL) mapfp = fopen(PATH_VAR_DMD, "rb+");
		fseek(mapfp, mapseek, SEEK_SET);
		writemapp(&buf, PATH_VAR_DMD);
		free_map(&map);
		map = buf;
		linkmap();
	}

	m_map buf;
	dm_generate(&buf, dm_dmmd, xrseed);

	//place the loads
	m_maploader loadbuf;
	for (int i = 0; i < 4; i++)
	{
		short xp[] = { 8, 16, 8, 0 };
		short yp[] = { 16, 8, 0, 8 };

		if (dm_dmmd->path.p[i])
		{
			if (!(xp[i] == preox && yp[i] == preoy))
			{
				loadbuf.ix = xp[i];
				loadbuf.iy = yp[i];
				loadbuf.ox = xp[(i + 2) % 4];
				loadbuf.oy = yp[(i + 2) % 4];
				loadbuf.seek = -1;

				push_back_mapload(&buf.loads, &loadbuf);
			}

			for (int ii = -2; ii <= +2; ii++)
			{
				s_sprite* s = &buf.arr[1].tile.arr[p2idx(xp[i] + ii * ((i + 1) % 2), yp[i] + ii * ((i + 0) % 2), DM_LENGTH)].arr[0];

				if (ii && s->id == -1)
					s->flag = SPRITE_FLAG_POPDSTSTACK;
			}
		}
	}

	fclose(mapfp); mapfp = 0;

	appendmap(&buf, PATH_VAR_DMD);

	return 0;
}

void set_npc_end_state_dm(n_npc* buf, int ix, int iy, float m)
{
	buf->ch.sprite.x = ix
		+ ((ch.rot + 1) % 2)
		* -3 * ((ch.rot / 2)
			? -1
			: +1);

	buf->ch.sprite.y = iy
		+ (ch.rot % 2)
		* -3 * ((ch.rot / 2)
			? -1
			: +1);

	set_npc_init_state_dm(buf, ix, iy, m);
}

void ch_transition_update()
{
	static unsigned char rot0 = 0;
	static int tik0 = 0;

	if (ch.lifea != ch.lifeb &&
		(ch.lifea + disps[0].dsplif - disps[1].dspbrk < 0)
		&& screen_mode != SCREEN_MODE_DISP_TRS)
	{
		ch.lifea = 0;
		memset(disps, 0, sizeof(disps));

		trs_end_fn = &trs_new_load;

		ch.dx = ch.sprite.x;
		ch.dy = ch.sprite.y;

		if (tik0 < tik)
		{
			screen_mode = -1;

			ch.rot = rot0;
			ch.state = 0;
			set_state(&ch, ch.rot % 4, 1);

			if (rot0 == 4 + 2)
			{
				rot0 = 0;
				tik0 = 0;
				screen_mode = SCREEN_MODE_DISP_TRS;
			}
			else rot0++;

			tik0 = tik + 50;
		}
	}
}

void trs_new_load()
{
	free_all();
	init_all();
}

void (*trs_end_fn)() = &trs_new_load;
dm_dmapmd* dm_dmmd;
dm_dmapmd* dm_dmmd_def;

inline int dm_rcomp(int i, int l)
{
	return rar_fn[0](xorrand() % RARITY_FN0_MAX, i, l);
}

void comp_dm_all()
{
	dmcachetempi = dm_rcomp(dm_dmmd->cachei, dm_dmmd->cachel);
	dmelemtempi = dm_rcomp(dm_dmmd->elemi, dm_dmmd->eleml);
	dmbreaktempi = dm_rcomp(dm_dmmd->breaktilei, dm_dmmd->breaktilel);
	dmnpctempi = dm_rcomp(dm_dmmd->npci, dm_dmmd->npcl);
}

void comp_dm_cachei()
{
	dmcachetempi = dm_rcomp(dm_dmmd->cachei, dm_dmmd->cachel);
}

void comp_dm_cachee()
{
	dmelemtempi = dm_rcomp(dm_dmmd->elemi, dm_dmmd->eleml);
}