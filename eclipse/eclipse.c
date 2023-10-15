#include "dglobal.h"

#include "cachefn.h"
#include "tilefn.h"
#include "npcfn.h"
#include "breaktilefn.h"

#include "dmapgen.h"
#include "dynamicmap.h"

#include <SDL.h>
#include <stdio.h>
#include <math.h>

char init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}
	else
	{
		{
			keys = SDL_GetKeyboardState(NULL);
			screenrect = (SDL_Rect){ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, };
			ch = create_character(1, 1, CHAR_SPRD_SIZE * 0, SPR_BLOCK * 0, SPR_BLOCK * 0, 0);
		}

		window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return 1;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			if (renderer == NULL)
			{
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
				return 1;
			}
		}
	}

	init_all();

	return 0;
}

bt_breaktile_h create_breaktileh_auto(m_map* map);

char loadmedia()
{
	SDL_Surface* bmp = SDL_LoadBMP(PATH_BMP);
	tilew = bmp->w;
	tileh = bmp->h;
	SDL_SetColorKey(bmp, SDL_TRUE, SDL_MapRGB(bmp->format, 0xff / 2, 0xff / 2, 0xff / 2));

	tile = SDL_CreateTextureFromSurface(renderer, bmp);
	SDL_FreeSurface(bmp);

	if (tile == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", PATH_BMP, SDL_GetError());
		return 1;
	}

	FILE* fp;

	fp = fopen(PATH_STD, "rb");

	if (fp == NULL) return 1;

	fread(sprdata, sizeof(s_spritedata), NUM_SPRITE, fp);

	fclose(fp);

	init_all();

	return (tile == NULL) | (fp == NULL);
}

void free_all()
{
	free_market();
	free_map(&map);
	free_mapseek(&loadstack);
	free_TScreenObj(&tsoa);
	free_IterFNA(&cyfn);
	free_IntCast(&intcasts);
	free_DMTableFA(&dmtable);
	free_UMRefreshA(&um_refresha);
}

void close()
{
	free_all();

	SDL_DestroyTexture(tile);
	//tile = NULL;
	SDL_DestroyWindow(window);
	//window = NULL;
	SDL_DestroyRenderer(renderer);
	//renderer = NULL;

	SDL_Quit();
}

void init_texts()
{
	t_aidx = 0;
	t_bidx = 0;
	t_width = 16 + 8 + 4 + 2;

	for (int i = 0; i < ITEM_MAX_SIZE; i++)
	{
		itexta[i] = create_SpriteDataArr(strlen(ItemText[i]));
		push_back_sprdataa_str(&(itexta[i]), ItemText[i]);
	}

	for (int i = 0; i < GTEXT_SIZE; i++)
	{
		gtexta[i] = create_SpriteDataArr(strlen(GlobalText[i]));
		push_back_sprdataa_str(&(gtexta[i]), GlobalText[i]);
	}
}

void render_elem_cache()
{
	if (npcai == -1)
		blit_elem_cache(ch.elem, ch.sprb, ch.token);
	else
		blit_elem_cache(
			map.npca.arr[npcai].ch.elem,
			map.npca.arr[npcai].ch.sprb, map.npca.arr[npcai].ch.token);

	if (screen_mode == SCREEN_MODE_DISP_TERM && menu_sel_idx_2 != -1)
	{
		sel_elem_menu_ofs = 1;
		blit_confirm_menu();
	}

	else if (menu_sel_idx_2 != -1)
	{
		if (npcai != -1)
			blit_cast_menu(
				menu_sel_idx_1 == -1 ? map.npca.arr[npcai].ch.defcast :
				map.npca.arr[npcai].ch.elem[menu_sel_idx_1].cast
			);
		else
			blit_cast_menu(
				menu_sel_idx_1 == -1 ? ch.defcast :
				ch.elem[menu_sel_idx_1].cast
			);
	}
}

void render()
{
	if (tik > ch.sprite.deltatime)
	{
		ch.sprite.deltatime += sprdata[ch.sprite.id].FD;
		ch.sprite.id += sprdata[ch.sprite.id].FJ;
	}

	for (int i = 0; i < map.npca.size; i++)
		if (tik > map.npca.arr[i].ch.sprite.deltatime)
		{
			map.npca.arr[i].ch.sprite.deltatime += sprdata[map.npca.arr[i].ch.sprite.id].FD;
			map.npca.arr[i].ch.sprite.id += sprdata[map.npca.arr[i].ch.sprite.id].FJ;
		}

	point chscr = pt_world_screen(ch.sprite.x, ch.sprite.y, -1 * ch.sprite.z);

	char blitc = 0;

	for (int i = 0; i < map.size; i++)
	{
		if (ch.sprite.x < map.arr[i].ox || ch.sprite.y < map.arr[i].oy)
		{
			blitc = 1;
			blit_ch(&ch);
		}

		for (int ii = 0; ii < map.npca.size; ii++)
			if (map.npca.arr[ii].ch.sprite.x < map.arr[i].ox || map.npca.arr[ii].ch.sprite.y < map.arr[i].oy)
			{
				point npcpt = pt_world_screen(map.npca.arr[ii].ch.sprite.x, map.npca.arr[ii].ch.sprite.y, 0);

				float xx = npcpt.x - chscr.x;
				float yy = npcpt.y - chscr.y;

				if (status_bar_mode && ii == npcai) isoblit(&(sprdata[SPRITE_MARKER_OFFSET]), xx, yy);
				blit_npc(&map.npca.arr[ii], xx, yy, map.npca.arr[ii].ch.sprite.z);
			}

		for (int y = 0; y < map.arr[i].height; y++)
			for (int x = 0; x < map.arr[i].width; x++)
			{
				for (int ii = 0; ii < map.npca.size; ii++)
					if (i == 1
						&& x + map.arr[i].ox == (int)map.npca.arr[ii].ch.sprite.x
						&& y + map.arr[i].oy == (int)map.npca.arr[ii].ch.sprite.y
						)
					{
						point npcpt = pt_world_screen(map.npca.arr[ii].ch.sprite.x, map.npca.arr[ii].ch.sprite.y, 0);

						float xx = npcpt.x - chscr.x;
						float yy = npcpt.y - chscr.y;

						if (status_bar_mode && ii == npcai) isoblit(&(sprdata[SPRITE_MARKER_OFFSET]), xx, yy);
						blit_npc(&map.npca.arr[ii], xx, yy, map.npca.arr[ii].ch.sprite.z);
					}

				if (i == 1
					&& x + map.arr[i].ox == (int)ch.sprite.x
					&& y + map.arr[i].oy == (int)ch.sprite.y
					)
				{
					blitc = 1;
					blit_ch(&ch);
				}

				s_sprite_a* sa = &(map.arr[i].tile.arr[p2idx(x, y, map.arr[i].width)]);

				for (int l = 0; l < sa->size; l++)
				{
					s_sprite* at = &(sa->arr[l]);

					if (at->id < 0) continue;

					if (sprdata[at->id].FJ != 0 && tik > at->deltatime)
					{
						at->id += sprdata[at->id].FJ;
						at->deltatime =
							tik
							+ ((1024 + 512) / 255) * sprdata[at->id].FD;
					}
					point tscr = pt_world_screen(at->x + map.arr[i].ox, at->y + map.arr[i].oy, 0);

					isoblit_z(
						&(sprdata[at->id]),
						tscr.x - chscr.x,
						tscr.y - chscr.y,
						at->z
					);
				}
			}
	}

	if (!blitc)
		blit_ch(&ch);

	if (status_bar_mode)
	{
		char e1m = ch.elemidx == -1 ? -1 : ch.elem[ch.elemidx].id;
		char e2m = npcai == -1
			? -1
			: (
				map.npca.arr[npcai].ch.elemidx == -1
				? -1
				: map.npca.arr[npcai].ch.elem[map.npca.arr[npcai].ch.elemidx].id
				);

		ch_char* npc = npcai == -1
			? NULL
			: &map.npca.arr[npcai].ch;

		stat_bar_dsp buf =
		{
			e1m < 0 ? -1 : (ELEM_OFFSET + 8 * e1m)
			, (npcai < 0 ? -1 : (e2m < 0 ? -1 : ELEM_OFFSET + 8 * e2m))
			, ((unsigned int)ch.sprb + 1) / (unsigned int)SPR_BLOCK
			, (unsigned int)(ch.spra + SPR_BLOCK - 1) / (unsigned int)SPR_BLOCK
			, ((npcai < 0 ? 0 : (unsigned int)npc->sprb) + 1) / (unsigned int)SPR_BLOCK
			, ((npcai < 0 ? 0 : (unsigned int)npc->spra) + SPR_BLOCK - 1) / (unsigned int)SPR_BLOCK
			, (ch.lifea != ch.lifeb) * STATUS_BAR_LF_MAX * (lifec(&ch) / (float)ch.lifeb)
			, npc ? (npc->lifea != npc->lifeb) * STATUS_BAR_LF_MAX * (lifec(npc) / (float)npc->lifeb) : 0
		};

		blit_status_bar(&buf);
	}

	if (screen_mode == SCREEN_MODE_DISP_CACHE)
		blit_ls(cache, cache_top, menu_sel_idx_0);
	else if (screen_mode == SCREEN_MODE_TEXT)
	{
		inc_textb(&gtexta[gtidx], 2);
		blit_textbox_std(&gtexta[gtidx], 0, 0, 16 * 2, 8 - 1);
	}
	else if (screen_mode == SCREEN_MODE_DISP_TERM)
	{
		{
			int w = 14;
			int x0 = (SCREEN_WIDTH / 16) - w - 2 - 2;
			int y0 = 0;
			int x1 = x0 + w + 2 + 1;
			int y1 = y0 + 6 + 2;

			blit_border(BORD_OFFSET + 6, x0, y0, x1, y1);

			for (int ii = 0; ii < TERM_MENU_MAX; ii++)
			{
				s_spritedata_a* idx[] = { &gtexta[68],&gtexta[5],&gtexta[6],&gtexta[7],&gtexta[8],&gtexta[9],&gtexta[10], };
				blit_text(idx[ii], x0, y0, w, ii);
			}

			blit_ls_cursor(&(sprdata[TEXT_ARROW_R_IDX]), x0, y0 + 1 + menu_sel_idx_0);
		}
		if (menu_sel_idx_1 != -1)
			if (menu_sel_idx_0 == 3)
				blit_ls(cstore, cstore_top, menu_sel_idx_1);
			else if (menu_sel_idx_0 == 4)
				blit_ls(cache, cache_top, menu_sel_idx_1);
			else if (menu_sel_idx_0 == 0)
				blit_ctg_menu_crn_ex(79 - 10, 3, 14, menu_sel_idx_1, 0);

		if (menu_sel_idx_2 != -1)
			blit_text_str_cnt(um_name_text, UM_MAX_MAP_NAME_TEXT_SIZE);
	}
	else if (screen_mode == SCREEN_MODE_DISP_MRKT)
	{
#define MARKET_TXT_IDX	(36-10)

		int x0 = (SCREEN_WIDTH / 16) - gtexta[MARKET_TXT_IDX].size - 2 - 2;
		int y0 = 0;
		int x1 = x0 + gtexta[MARKET_TXT_IDX].size + 2 + 1;
		int y1 = y0 + 3 + 2;

		blit_border(BORD_OFFSET + 6, x0, y0, x1, y1);

		for (int ii = 0; ii < 3; ii++)
		{
			int iat = ii == 2 ? 10 : MARKET_TXT_IDX + ii;

			blit_text(&(gtexta[iat]), x0, y0, gtexta[MARKET_TXT_IDX].size, ii);
		}

		blit_ls_cursor(&(sprdata[TEXT_ARROW_R_IDX]), x0, y0 + 1 + menu_sel_idx_0);

		if (menu_sel_idx_1 != -1)
		{
			if (menu_sel_idx_0 == 0)
			{
				blit_ls(
					markit[markitidx].arr,
					markit[markitidx].size,
					menu_sel_idx_1
				);
			}
			else if (menu_sel_idx_0 == 1)
			{
				blit_ls(
					cache, cache_top, menu_sel_idx_1
				);
			}
			if (menu_sel_idx_2 != -1)
			{
				sel_elem_menu_ofs = 40 - 21;
				blit_confirm_menu();
			}
		}
	}
	else if (screen_mode == SCREEN_MODE_DISP_TRADE)
	{
		sel_elem_menu_ofs = 42 - 21;
		blit_confirm_menu();
	}
	else if (screen_mode == SCREEN_MODE_DISP_ELEM)
	{
		int xc = (SCREEN_WIDTH / 2) - (ELEM_AREA / 2);
		int yc = (SCREEN_HEIGHT / 2) - (ELEM_AREA / 2);

		blit_border(6 + BORD_OFFSET,
			(SCREEN_WIDTH / 2) / BORD_SIZE - (ELEM_AREA / BORD_SIZE) / 2,
			(SCREEN_HEIGHT / 2) / BORD_SIZE - (ELEM_AREA / BORD_SIZE) / 2,
			(SCREEN_WIDTH / 2) / BORD_SIZE + (ELEM_AREA / BORD_SIZE) / 2 - 1,
			(SCREEN_HEIGHT / 2) / BORD_SIZE + (ELEM_AREA / BORD_SIZE) / 2 - 1
		);
		blit_elem((ELEM_OFFSET + 8 * disp_elem_idx), xc, yc);
	}
	else if (screen_mode == SCREEN_MODE_DISP_TRS)
	{
		int* trbuf = blit_transition();

		if (*trbuf == -1)
		{
			SDL_RenderClear(renderer);
			SDL_RenderPresent(renderer);
		}
		else if (*trbuf >= SCREEN_WIDTH)
		{
			SDL_RenderPresent(renderer);
			*trbuf = -1;
		}
		else if (*trbuf >= 0)
			*trbuf += SCREEN_WIDTH / (8);
	}
	else if (screen_mode == SCREEN_MODE_MENU_CNFRM)
	{
		sel_elem_menu_ofs = (48 - 10) - 11;
		blit_confirm_menu();
	}
	else if (screen_mode == SCREEN_MODE_UM_EDIT_MENU)
	{
		blit_ctg_menu_crn(73 - 10, 5, 18);
	}
	else if (screen_mode == SCREEN_MODE_UM_EDIT_NEWMAP)
		blit_ctg_menu_txt(UM_NEWMAP_NAME_IDX - (2 - 1), 2, 16);
}

void render_select_elem()
{
	blit_fill_rect(0x00);

	for (int i = 0; i < 8 * (3); i++)
	{
		int x = ((i % 8) * 8);
		int y = 3 + 8 * (i / 8);

		blit_border(BORD_OFFSET + 6, x, y, x + (128 / 16) - 1, y + (128 / 16) - 1);
	}

	char m = (SCREEN_HEIGHT / 128) * (SCREEN_WIDTH / 128);
	char o = m * (menu_sel_idx_1 / m);

	for (int i = o; i < o + m && i < estore_top; i++)
	{
		int x = (i - o) % (SCREEN_WIDTH / 128);
		int y = (i - o) / (SCREEN_WIDTH / 128);

		if (i != menu_sel_idx_1 || seltik)
			blit_elem(estore[i].id * 8 + ELEM_OFFSET, x * 128, y * 128 - ((128 * (SCREEN_HEIGHT / 128)) / 2) + SCREEN_HEIGHT / 2);
	}

	UPDATE_TICK();

	if (menu_sel_idx_2 != -1)
	{
		blit_confirm_menu();
	}
}

void zero_menu_select()
{
	menu_sel_idx_0 = 0;
	menu_sel_idx_1 = -1;
	menu_sel_idx_2 = -1;
}

char menu_up_down_curs_set(SDL_Keycode sym, int max)
{
	if (menu_sel_idx_1 == -1 && sym == KEY_MENU_UP && menu_sel_idx_0 - 1 >= 0) { menu_sel_idx_0--; return 1; }
	else if (menu_sel_idx_1 == -1 && sym == KEY_MENU_DOWN && menu_sel_idx_0 + 1 < max) { menu_sel_idx_0++; return 1; }

	return 0;
}

char menu_1_up_down_curs_set(SDL_Keycode sym, int max)
{
	if (sym == KEY_MENU_DOWN && menu_sel_idx_1 + 1 < max) { menu_sel_idx_1++; return 1; }
	else if (sym == KEY_MENU_UP && menu_sel_idx_1 - 1 >= 0) { menu_sel_idx_1--; return 1; }
	return 0;
}

char menu_2_up_down_curs_set(SDL_Keycode sym, int max)
{
	if (sym == KEY_MENU_DOWN && menu_sel_idx_2 + 1 < max)
	{
		menu_sel_idx_2++;
		return 1;
	}
	else if (sym == KEY_MENU_UP && menu_sel_idx_2 - 1 >= 0)
	{
		menu_sel_idx_2--;
		return 1;
	}
	return 0;
}

void exit_menu_set_tidx(int tidx)
{
	menu_sel_idx_0 = 0;
	menu_sel_idx_1 = -1;
	menu_sel_idx_2 = -1;
	t_aidx = 0;
	t_bidx = 0;
	screen_mode = SCREEN_MODE_TEXT;
	gtidx = tidx;
}

void toggle_elem_screen()
{
	keys = SDL_GetKeyboardState(NULL);

	blit_fill_rect(0xFF);
	SDL_RenderPresent(renderer);

	SDL_Delay(250);

	screen_mode = (screen_mode == SCREEN_MODE_REND_CACHE || screen_mode == SCREEN_MODE_APPLY_CAS) ? SCREEN_MODE_NORMAL : SCREEN_MODE_REND_CACHE;

	menu_sel_idx_1 = -1;

	if (screen_mode == SCREEN_MODE_REND_CACHE) renderfn = &render_elem_cache;
	else renderfn = &render;
}

//debug code v
void rmdata()
{
	xorrand();

	remove(PATH_CID);
	remove(PATH_NSM);
	remove(PATH_SED);
}

void tcdata()
{
	touch(PATH_NSM);
	touch(PATH_CID);
	touch(PATH_PID);
	touch(PATH_BDB);
	touch(PATH_SED);
	touch(PATH_LAS);
}

void rmtcdata()
{
	rmdata();
	tcdata();
}


void init_all()
{
	cyfn = create_IterFNA(1);
	um_refresha = create_UMRefreshA(1);

	push_back_IterFNA(&cyfn, &itfn_auto_ref);
	push_back_IterFNA(&cyfn, &itfn_apply_icas);

	{
		readsed(PATH_SED);
		readlas(PATH_LAS);
		map = readmapp(PATH_VAR_MAP, mapseek);
		linkmap();

		push_back_mapload(&map.loads, &loadstack.arr[loadstack.size - 1].load);
	}

	touch(PATH_CID);
	touch(PATH_SED);
	touch(PATH_NSM);
	touch(PATH_DMT);

	init_all_market();
	readlas(PATH_LAS);
	bthmupdate = create_bthmupdatea(1);
	tsoa = create_TScreenObjA(1);
	intcasts = create_IntCastA(1);
	init_texts();
	dmtable = readdmt(PATH_DMT);
	readsed(PATH_SED);
}

int main(int argc, char* args[])
{
	if (init() == 0)
	{
		if (loadmedia() == 0)
		{
			surfacerect = (SDL_Rect){ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			static SDL_Event e;
			for (char run = 1; run;)
			{
				tik = SDL_GetTicks();

				gcdflag = ch.gcool > tik;

				{
					for (int i = 0; i < map.npca.size; i++)
					{
						update_npc(&map.npca.arr[i], &(map.arr[1]));
						move(&(map.npca.arr[i].ch), map.npca.arr[i].ch.dx, map.npca.arr[i].ch.dy);
					}

					update_bthm(&bthmupdate, &(map.arr[1]), 0);

					update_acttick(&ch);

					if (ch.state == 2 && ch.acttick <= tik)
						set_state(&ch, ch.rot, 0);

					if (ch.gcool != -1 && !gcdflag)
						ch.gcool = -1;
					else if (!gcdflag)
						move(&ch, ch.dx, ch.dy);

					tile_load_map(chdx, chdy);

					if ((int)ch.sprite.x == ch.sprite.x && (int)ch.sprite.y == ch.sprite.y)
					{
						chdx = ch.sprite.x;
						chdy = ch.sprite.y;
					}

					if ((keys[KEY_CH_WALK_XL]
						|| keys[KEY_CH_WALK_XR]
						|| keys[KEY_CH_WALK_YL]
						|| keys[KEY_CH_WALK_YR]) && !keys[KEY_CH_ACTION])
						goto keydown;
				}

				while (SDL_PollEvent(&e))
				{
					if (e.type == SDL_QUIT)
						run = 0;
					if (e.type == SDL_KEYDOWN)
					{
					keydown:
						keys = SDL_GetKeyboardState(NULL);

						deltakey = 1;

						if (keys[KEY_SELECT_2])
						{
							if (screen_mode == SCREEN_MODE_NORMAL)
							{
								toggle_elem_screen();
							}
							else if (screen_mode == SCREEN_MODE_REND_CACHE)
							{
								static char idx = -1;

								if (idx == -1)
								{
									idx = menu_sel_idx_1;
								}
								else
								{
									e_element telem = ch.elem[idx];
									ch.elem[idx] = ch.elem[menu_sel_idx_1];
									ch.elem[menu_sel_idx_1] = telem;

									idx = -1;
								}
							}
						}

						else if (keys[KEY_STATUSBAR_TOGGLE]) status_bar_mode ^= 1;

						ddx = ch.dx;
						ddy = ch.dy;

						if (screen_mode == SCREEN_MODE_NORMAL)
						{
							if (e.key.keysym.sym == KEY_TEST_0)
							{
								system("echo \a");

								ch.elemidx = 0;
								static int foo = 0;
								ch.elem[0].id = 50 - foo++;

								ch.defcast[0]
									= 22;

								ch.spra = ch.sprb = SPR_BLOCK * 3;

								{
									short buf[] = { 1,1,1,1, };

									memcpy(ch.elem[0].cast, buf, sizeof(buf));

									static char r = 0;
									if (!r++)
										ch.elem[0].spra =
										ch.elem[0].sprb = 0;
								}

								{
									for (int i = 0; i < 8; i++)
									{
										cache[i].id = 49 + i;
										cache[i].mul = 111;
										cache[i].active = 0;
										cache_top = i;
									}

									continue;
								}
							}

							if (e.key.keysym.sym == KEY_TEST_1)
							{
								system("echo \a");

								debug_ch_collison ^= 1;

								continue;

								cache_top = 3;

								cache[0].id = 1;
								cache[0].mul = 21;

								cache[1].id = 1;
								cache[1].mul = 2;

								cache[2].id = 1 + 4;
								cache[2].mul = 2;

								ch.lifea = ch.lifeb = SPR_MAX;
								ch.spra = ch.sprb = SPR_BLOCK * 15;

								ch.spra -= SPR_BLOCK * 8;
								ch.lifea -= SPR_BLOCK * 4;

								dm_dmmd->msrchm = 0;

								continue;

								debug_ch_collison ^= 1;
							}

							if (keys[KEY_SWAP_LEFT])
							{
								if (ch.elemidx - 1 >= -1) ch.elemidx--;
								SDL_Delay(128);
							}
							if (keys[KEY_SWAP_RIGHT])
							{
								if (ch.elemidx < 8 && ch.elem[ch.elemidx + 1].id != -1) ch.elemidx++;
								SDL_Delay(128);
							}

							if (keys[KEY_SELECT_1])
							{
								if (0 != rayselect(&(map.npca), &ch)
									&& select_none_fn != NULL)
									select_none_fn();
							}

							if (keys[KEY_APPLY_ICACHE])
							{
								apply_all_ic();
							}

							if (keys[KEY_ELEM_SWAP])
							{
								static char state = -1;
								xswap(state, ch.elemidx);

								((unsigned char*)keys)[KEY_ELEM_SWAP] = 0;

								SDL_Delay(100);
							}

							if (action_down() || keys[KEY_SELECT_1])
							{
								if (!gcdflag)
								{
									int dx = 0;
									int dy = 0;

									if (ch.rot == 0) dx = +1;
									else if (ch.rot == 1) dy = +1;
									else if (ch.rot == 2) dx = -1;
									else if (ch.rot == 3) dy = -1;

									int ix0 = dx + ch.sprite.x;
									int iy0 = dy + ch.sprite.y;

									int ix = ix0;
									int iy = iy0;

									if (keys[KEY_SELECT_1])
									{
										char flag = map.arr[1].tile.arr
											[pm(p2idx(ix, iy, map.arr[1].width), map.arr[1].tile.size)]
											.arr[0].flag;

										if (flag == SPRITE_FLAG_TERM)
										{
											SDL_Delay(100);
											((Uint8*)keys)[KEY_SELECT_1] = 0;

											zero_menu_select();
											keys = SDL_GetKeyboardState(NULL);

											screen_mode = SCREEN_MODE_DISP_TERM;
											goto breakactsel;
										}
										else if (flag == SPRITE_FLAG_TEXT)
										{
											for (int i = 0; i < map.texts.size; i++)
											{
												if (
													map.texts.arr[i].x == ix0
													&&
													map.texts.arr[i].y == iy0
													)
												{
													t_aidx = 0;
													t_bidx = 0;
													screen_mode = SCREEN_MODE_TEXT;
													gtidx = map.texts.arr[i].tidx;

													break;
												}
											}
										}
										else
										{
											for (int i = 0; i < map.npca.size; i++)
											{
												if (abs((int)map.npca.arr[i].ch.sprite.x - ix0) <= map.npca.arr[i].cix
													&& abs((int)map.npca.arr[i].ch.sprite.y - iy0) <= map.npca.arr[i].ciy)
												{
													set_state(&(map.npca.arr[i].ch), (ch.rot + 2) % 4, map.npca.arr[i].ch.state);
													if (map.npca.arr[i].textidx >= 0)
													{
														gtidx = map.npca.arr[i].textidx;
														screen_mode = SCREEN_MODE_TEXT;

														t_aidx = 0;
														t_bidx = 0;

														npcidx = i;
													}

													goto breakactsel;
												}
											}
										}
										goto breakactsel;
									}
									else
									{
										c_cast v = { &ch,
											(npcai == -1 ? NULL : &map.npca.arr[npcai].ch)
											, ix, iy };

										short* cast =
											ch.elemidx == -1
											? ch.defcast
											: ch.elem[ch.elemidx].cast
											;

										apply_cast(cast, actkeyidx, &v, &ch);
										if (ch.spra < 0) ch.spra = 0;
										else if (ch.spra == 0)
										{
											short cast = 0;
											apply_cast(&cast, 0, &v, &ch);
										}
									}
								}
							}

						breakactsel:
							if (gcdflag) continue;
							{
								if (keys[KEY_CH_WALK_XL])
								{
									ch.dx = ceil(ch.sprite.x) - 1;
									dr = 2;

									if (statusr) adjust_map();
								}
								else if (keys[KEY_CH_WALK_XR])
								{
									ch.dx = floor(ch.sprite.x) + 1;
									dr = 0;

									if (statusr) adjust_map();
								}
							}

							{
								if (keys[KEY_CH_WALK_YL])
								{
									ch.dy = ceil(ch.sprite.y) - 1;
									dr = 3;

									if (statusr) adjust_map();
								}

								else if (keys[KEY_CH_WALK_YR])
								{
									ch.dy = floor(ch.sprite.y) + 1;
									dr = 1;

									if (statusr) adjust_map();
								}
							}
						}
						else if (screen_mode == SCREEN_MODE_UM_EDIT_ADD)
						{
							if (keys[KEY_CH_WALK_XR] && um_edit_x + 1 < map.arr[1].width)
								um_edit_add_position(+1, 0, 0);
							else if (keys[KEY_CH_WALK_XL] && um_edit_x - 1 >= 0)
								um_edit_add_position(-1, 0, 0);
							else if (keys[KEY_CH_WALK_YR] && um_edit_y + 1 < map.arr[1].height)
								um_edit_add_position(0, +1, 0);
							else if (keys[KEY_CH_WALK_YL] && um_edit_y - 1 >= 0)
								um_edit_add_position(0, -1, 0);

							else if (keys[KEY_SWAP_LEFT] && um_edit_z - 1 >= 0)
								um_edit_add_position(0, 0, -1);
							else if (keys[KEY_SWAP_RIGHT] && um_edit_z + 1 < UM_Z_MAX)
								um_edit_add_position(0, 0, +1);

							else if (keys[KEY_SELECT_1] || keys[KEY_CH_ACTION])
							{
								clear_TScreenObjA(&tsoa);
								rmf_IterFNA(&cyfn, &um_blink_tile);
								screen_mode = SCREEN_MODE_NORMAL;

								if (keys[KEY_SELECT_1])
									um_add_mat();
							}
						}
						else if (screen_mode == SCREEN_MODE_UM_EDIT_RM)
						{
							if (2 == menu_sel_idx_0)
							{
								um_rm_all_mat();

								clear_TScreenObjA(&tsoa);
								rmf_IterFNA(&cyfn, &um_blink_tile);
								screen_mode = SCREEN_MODE_NORMAL;
							}

							else if (keys[KEY_CH_WALK_XR])
								um_edit_rm_mat(+1, 0, -um_edit_z);
							else if (keys[KEY_CH_WALK_XL])
								um_edit_rm_mat(-1, 0, -um_edit_z);
							else if (keys[KEY_CH_WALK_YR])
								um_edit_rm_mat(0, +1, -um_edit_z);
							else if (keys[KEY_CH_WALK_YL])
								um_edit_rm_mat(0, -1, -um_edit_z);

							else if (keys[KEY_SWAP_LEFT] && um_edit_z - 1 >= 0)
								um_edit_rm_mat(0, 0, -1);
							else if (keys[KEY_SWAP_RIGHT] && um_edit_z + 1 < UM_Z_MAX)
								um_edit_rm_mat(0, 0, +1);

							else if (keys[KEY_SELECT_1] || keys[KEY_CH_ACTION])
							{
								char valid = tsoa.size != 0;

								if (valid)
								{
									int i0 = p2idx(um_edit_x, um_edit_y, map.arr[1].width);
									s_sprite_a* sa = &map.arr[1].tile.arr[i0];
									sa->arr[um_edit_z]
										.id = abs(tsoa.arr[0].v.id);
								}

								clear_TScreenObjA(&tsoa);
								rmf_IterFNA(&cyfn, &um_blink_tile);
								screen_mode = SCREEN_MODE_NORMAL;

								if (keys[KEY_SELECT_1] && valid)
								{
									if (0 == menu_sel_idx_0)
										um_rm_mat();
									else if (1 == menu_sel_idx_0)
										um_rm_stack_mat();
									else if (2 == menu_sel_idx_0)
										um_rm_all_mat();
								}
							}
						}
						else if (screen_mode == SCREEN_MODE_DISP_CACHE)
						{
							menu_up_down_curs_set(e.key.keysym.sym, cache_top);

							if (keys[KEY_SELECT_1])
							{
								char top = cache_top;

								apply_ic(menu_sel_idx_0);

								if (top != cache_top)
									menu_sel_idx_0--;
							}
						}
						else if (screen_mode == SCREEN_MODE_REND_CACHE || screen_mode == SCREEN_MODE_APPLY_CAS)
						{
							blit_tick_elem_cache = 1;

							if (keys[KEY_SELECT_1])
							{
								if (menu_sel_idx_2 == -1)
									menu_sel_idx_2 = 0;
								else if (npcai == -1)
								{
									if (screen_mode == SCREEN_MODE_REND_CACHE)
									{
										static char elem_0 = -1;

										short* castfield;

										if (menu_sel_idx_1 == -1)
											castfield = ch.defcast;
										else
											castfield = ch.elem[menu_sel_idx_1].cast;

										if (menu_sel_idx_2 != elem_0)
										{
											if (elem_0 == -1)
												elem_0 = menu_sel_idx_2;
											else
											{
												xswap(castfield[menu_sel_idx_2]
													, castfield[elem_0]);
												elem_0 = -1;
											}
										}
									}
									else
									{
										apply_setcac();
									}
								}
							}

							if (menu_sel_idx_2 == -1)
							{
								ch_char* tar =
									npcai == -1
									? &ch
									: &map.npca.arr[npcai].ch;

								if (e.key.keysym.sym == KEY_MENU_LEFT && menu_sel_idx_1 - 1 >= 0)
									menu_sel_idx_1--;
								else if (e.key.keysym.sym == KEY_MENU_RIGHT && tar->elem[menu_sel_idx_1 + 1].id != -1)
									menu_sel_idx_1++;

								seltik = 0;
							}
							else
							{
								if (e.key.keysym.sym == KEY_MENU_UP && menu_sel_idx_2 - 1 >= 0)
									menu_sel_idx_2--;
								else if (e.key.keysym.sym == KEY_MENU_DOWN && menu_sel_idx_2 + 1 < 4)
									menu_sel_idx_2++;
							}

							if (keys[KEY_CH_ACTION])
							{
								if (menu_sel_idx_2 == -1)
									toggle_elem_screen();
								else if (menu_sel_idx_2 != -1)
									menu_sel_idx_2 = -1;
							}
						}
						else if (screen_mode == SCREEN_MODE_DISP_MRKT)
						{
							if (menu_up_down_curs_set(e.key.keysym.sym, 3))
								;
							else if (menu_sel_idx_2 != -1)
								menu_2_up_down_curs_set(e.key.keysym.sym, 2);
							else if (menu_sel_idx_1 != -1)
								menu_1_up_down_curs_set(e.key.keysym.sym, markit[markitidx].size);

							static int token_trade = 0;

							if (keys[KEY_SELECT_1])
							{
								if (menu_sel_idx_1 == -1)
								{
									if (menu_sel_idx_0 == 0 || menu_sel_idx_0 == 1)
										menu_sel_idx_1 = 0;
									else if (menu_sel_idx_0 == 2)
										exit_menu_set_tidx(29);
								}
								else if (menu_sel_idx_2 == -1)
								{
									if (menu_sel_idx_0 == 0)
									{
#define NUM_TOKEN_MENU_TXT_IDX	30
										for (int i = 0; i < 4; i++)
											gtexta[NUM_TOKEN_MENU_TXT_IDX]
											.arr[i + 6] =
											&(sprdata[
												pm(((markit[markitidx]
													.cost
													[menu_sel_idx_1]
													/ (int)pow(10, 3 - i)) % 10) - 1, (TEXT_0 - TEXT_1 + 1)) + TEXT_1
											]);

										menu_sel_idx_2 = 0;
									}
									else if (menu_sel_idx_0 == 1)
									{
										int idx;
										for (idx = 0
											; idx < markit[markitidx].size && markit[markitidx].arr[idx].id != cache[menu_sel_idx_1].id
											; idx++)
											;

										if (idx >= markit[markitidx].size)
										{
											exit_menu_set_tidx(31);
											continue;
										}

										menu_sel_idx_2 = 0;

										token_trade = (int)(cache[menu_sel_idx_1].mul *
											(markit[markitidx].cost[idx]
												/ (float)(markit[markitidx].arr[idx].mul)));

										for (int i = 0; i < 4; i++)
											gtexta[NUM_TOKEN_MENU_TXT_IDX]
											.arr[i + 6] =
											&(sprdata[
												pm(((token_trade / (int)pow(10, 3 - i)) % 10) - 1, (TEXT_0 - TEXT_1 + 1)) + TEXT_1
											]);
									}
								}
								else
								{
									if (menu_sel_idx_0 == 0)
									{
										if (menu_sel_idx_2 == 0)
										{
											if (ch.token - markit[markitidx]
												.cost
												[menu_sel_idx_1] < 0)
											{
												exit_menu_set_tidx(28);
												continue;
											}

											menu_sel_idx_2 = -1;

											ch.token -= markit[markitidx]
												.cost
												[menu_sel_idx_1];

											add_cache(
												cache,
												&cache_top,
												ICACHE_MAX_SIZE,
												&markit[markitidx].arr[menu_sel_idx_1]);
										}
										else if (menu_sel_idx_2 == 1)
											menu_sel_idx_2 = -1;
									}
									else if (menu_sel_idx_0 == 1)
									{
										if (menu_sel_idx_2 == 0)
										{
											rm_cache(cache, &cache_top, menu_sel_idx_1);

											ch.token += token_trade;

											menu_sel_idx_2 = -1;
										}
										else if (menu_sel_idx_2 == 1)
											menu_sel_idx_2 = -1;
									}
								}
							}
							else if (keys[KEY_CH_ACTION])
							{
								if (menu_sel_idx_2 != -1)
									menu_sel_idx_2 = -1;
								else if (menu_sel_idx_1 != -1)
									menu_sel_idx_1 = -1;
								else if (menu_sel_idx_0 != -1)
									exit_menu_set_tidx(29);
							}
						}
						else if (screen_mode == SCREEN_MODE_DISP_TERM)
						{
							if (menu_sel_idx_1 != -1 && menu_sel_idx_2 == -1)
								seltik = 0;

							if (menu_up_down_curs_set(e.key.keysym.sym, TERM_MENU_MAX));
							else if (menu_sel_idx_1 != -1 && menu_sel_idx_2 == -1 && e.key.keysym.sym == KEY_MENU_LEFT && (menu_sel_idx_1 - 1) >= 0) menu_sel_idx_1--;
							else if (menu_sel_idx_1 != -1 && menu_sel_idx_2 == -1 && e.key.keysym.sym == KEY_MENU_RIGHT &&
								((menu_sel_idx_1 + 1 < estore_top && menu_sel_idx_0 == 2)
									|| (ch.elem[menu_sel_idx_1 + 1].id != -1 && menu_sel_idx_0 == 2)))
								menu_sel_idx_1++;
							else if (menu_sel_idx_2 != -1)
							{
								if (menu_sel_idx_0 == 0)
								{
									if (e.key.keysym.sym == KEY_MENU_DOWN
										&& um_name_text[UM_MAX_MAP_NAME_TEXT_SIZE * (menu_sel_idx_2 + 1)]
										&& menu_sel_idx_2 + 1 < UM_MAX_MAP_NAME_TEXT_SIZE)
										menu_sel_idx_2++;
									else if (e.key.keysym.sym == KEY_MENU_UP && menu_sel_idx_2 - 1 >= 0)
										menu_sel_idx_2--;
								}
								else
									menu_2_up_down_curs_set(e.key.keysym.sym, 2);
							}
							else if (menu_sel_idx_0 == 3)
								menu_1_up_down_curs_set(e.key.keysym.sym, cstore_top);
							else if (menu_sel_idx_0 == 4)
							{
								if (e.key.keysym.sym == KEY_MENU_DOWN && menu_sel_idx_1 + 1 < cache_top)
									menu_sel_idx_1++;
								else if (e.key.keysym.sym == KEY_MENU_UP && menu_sel_idx_1 - 1 >= 0)
									menu_sel_idx_1--;
							}
							else if (menu_sel_idx_0 == 0 && menu_sel_idx_1 != -1)
							{
								if (e.key.keysym.sym == KEY_MENU_DOWN && menu_sel_idx_1 + 1 < 3)
									menu_sel_idx_1++;
								if (e.key.keysym.sym == KEY_MENU_UP && menu_sel_idx_1 - 1 >= 0)
									menu_sel_idx_1--;
							}

							if (keys[KEY_SELECT_1])
							{
								if (menu_sel_idx_0 == 0)
								{
									if (menu_sel_idx_1 == -1)
										menu_sel_idx_1 = 0;
									else if (menu_sel_idx_2 == -1)
									{
										if (menu_sel_idx_1 == 0)
										{
											if (um_name_text[0])
												menu_sel_idx_2 = 0;
											else
											{
												um_newmap_init_texts();
												menu_sel_idx_0 = 0;
												screen_mode = SCREEN_MODE_UM_EDIT_NEWMAP;
											}
										}
										else if (menu_sel_idx_1 == 1)
										{
											if (um_newmap_init_texts())
											{
												screen_mode = SCREEN_MODE_TEXT;
												gtidx = 84 - 10;
												t_aidx = 0;
												t_bidx = 0;
											}
											else
												screen_mode = SCREEN_MODE_UM_EDIT_NEWMAP;
										}
										else if (menu_sel_idx_1 == 2)
										{
											if (um_name_text[0])
												menu_sel_idx_2 = 0;
											else
											{
												screen_mode = SCREEN_MODE_TEXT;
												gtidx = 87 - 10;
												t_aidx = 0;
												t_bidx = 0;
											}
										}
									}
									else
									{
										um_map_index = menu_sel_idx_2;

										if (menu_sel_idx_1 == 0)
										{
											um_map_in_init();
											um_map_load();

											blit_delay();

											EXIT_MENU_ID(-1);
										}
										else if (menu_sel_idx_1 == 2)
										{
											um_map_remove();

											screen_mode = SCREEN_MODE_TEXT;
											gtidx = 88 - 10;
											t_aidx = 0;
											t_bidx = 0;
										}

									}
								}
								else if (menu_sel_idx_0 == 6)
								{
									EXIT_MENU();
								}
								else if (menu_sel_idx_0 == 5)
								{
#define SAVE_TXT_MSG_IDX	(55 - 38)
									writelas(PATH_LAS, &loadstack);
									writesed(PATH_SED);
									gtidx = SAVE_TXT_MSG_IDX;
									t_aidx = 0;
									t_bidx = 0;

									screen_mode = SCREEN_MODE_TEXT;
									menu_sel_idx_0 = 0;
								}
								else if (menu_sel_idx_1 == -1)
								{
									if (menu_sel_idx_0 == 1)
									{
										menu_sel_idx_2 = -1;
										renderfn = &render_select_elem;
									}
									else if (menu_sel_idx_0 == 2)
									{
										menu_sel_idx_2 = -1;
										renderfn = &render_elem_cache;
									}
									menu_sel_idx_1 = 0;
								}
								else if (menu_sel_idx_2 == -1 && (menu_sel_idx_0 == 1 || menu_sel_idx_0 == 2))
								{
									menu_sel_idx_2 = 0;
									sel_elem_menu_ofs = 0;
								}
								else if (menu_sel_idx_0 == 1 && menu_sel_idx_2 == 0)
								{
									if (sel_elem_menu_ofs == 0)
									{
										if (estore_top <= 0)
										{
											transfer_evt(0);
											break;
										}
										int l;
										for (l = 0; l < ELEM_CACHE_MAX && ch.elem[l].id != -1; l++);

										if (l >= ELEM_CACHE_MAX)
											break;

										e_element tmp;

										for (int i = menu_sel_idx_1; i < estore_top - 1; i++)
										{
											tmp = estore[i + 0];
											estore[i + 0] = estore[i + 1];
											estore[i + 1] = tmp;
										}

										estore_top--;

										tmp = ch.elem[l];
										ch.elem[l] = estore[estore_top];
										estore[estore_top] = tmp;

										ch.elem[l + 1].id = -1;
									}
									else
									{
										for (int i = menu_sel_idx_1; i + 1 < estore_top; i++)
										{
											e_element telem = estore[i];
											estore[i] = estore[i + 1];
											estore[i + 1] = telem;
										}

										estore_top--;

										menu_sel_idx_2 = -1;
									}
								}
								else if (menu_sel_idx_0 == 2 && menu_sel_idx_2 == 0)
								{
									if (ch.elem[1].id == -1 || ch.elem[menu_sel_idx_1].id == -1 || estore_top >= ELEM_STORE_MAX)
									{
										transfer_evt(0);
										break;
									}

									int i;
									for (i = menu_sel_idx_1; ch.elem[i + 1].id != -1 && i + 1 < ELEM_CACHE_MAX; i++)
										swap_elem(
											&(ch.elem[i + 0]),
											&(ch.elem[i + 1])
										);

									estore[estore_top++] = ch.elem[i];
									ch.elem[i].id = -1;

									if (menu_sel_idx_1 == i) menu_sel_idx_1--;
								}
								else if (menu_sel_idx_2 == 1)
								{
									menu_sel_idx_2 = -1;
								}

								else if (menu_sel_idx_0 == 3)
								{
									if (cache_top + 1 > ICACHE_MAX_SIZE)
									{
										transfer_evt(0);
										break;
									}

									char cc = cstore_top;
									for (int i = 0; i < cstore_top; i++)
										if (cstore[i].active)
										{
											for (int ii = i; ii < cstore_top - 1; ii++)
												swap_cache(&(cstore[ii]), &(cstore[ii + 1]));
											i--;

											if (!add_cache(cache, &cache_top, ICACHE_MAX_SIZE, &(cstore[cstore_top - 1])))
											{
												transfer_evt(0);
												break;
											}

											cstore_top--;
										}

									if (cstore_top != cc)
										transfer_evt(1);
								}
								else if (menu_sel_idx_0 == 4)
								{
									char cc = cache_top;
									for (int i = 0; i < cache_top; i++)
										if (cache[i].active)
										{
											for (int ii = i; ii < cache_top - 1; ii++)
												swap_cache(&(cache[ii]), &(cache[ii + 1]));
											i--;

											if (!add_cache(cstore, &cstore_top, ITEM_STORE_MAX, &cache[cache_top - 1]))
												transfer_evt(0);

											cache_top--;
										}

									if (cache_top != cc)
										transfer_evt(1);
								}
							}
							else if (keys[KEY_CH_ACTION])
							{
								if (menu_sel_idx_1 != -1 && menu_sel_idx_2 == -1)
								{
									renderfn = &render;
									menu_sel_idx_1 = -1;
								}
								else if (menu_sel_idx_2 != -1)
									menu_sel_idx_2 = -1;
								else if (menu_sel_idx_1 == -1 && menu_sel_idx_2 == -1)
									screen_mode = SCREEN_MODE_NORMAL;
							}
							else if (keys[KEY_SELECT_2])
							{
								if (menu_sel_idx_1 >= 0)
								{
									if (menu_sel_idx_0 == 3)
										cstore[menu_sel_idx_1].active ^= 1;
									else if (menu_sel_idx_0 == 4)
										cache[menu_sel_idx_1].active ^= 1;
									else if (menu_sel_idx_0 == 1 && menu_sel_idx_2 == -1)
									{
										menu_sel_idx_2 = 0;
										sel_elem_menu_ofs = 45;
									}
								}

							}
						}
						else if (screen_mode == SCREEN_MODE_DISP_TRADE)
						{
							menu_2_up_down_curs_set(e.key.keysym.sym, 2);

							if (keys[KEY_SELECT_1])
							{
								if (menu_sel_idx_2 == 0)
									tradefn();
								else if (menu_sel_idx_2 == 1)
									goto exit;
							}
							else if (keys[KEY_CH_ACTION])
							{
							exit:
								EXIT_MENU();
								menu_sel_idx_2 = -1;
							}
						}
						else if (screen_mode == SCREEN_MODE_DISP_ELEM)
						{
							if (keys[KEY_CH_ACTION] || keys[KEY_SELECT_1])
							{
								screen_mode = SCREEN_MODE_NORMAL;
								if (disp_elem_close != NULL)
									disp_elem_close();
							}
						}
						else if (screen_mode == SCREEN_MODE_DISP_TRS)
						{
							if (*blit_transition() == 0)
								screen_mode = SCREEN_MODE_DISP_TRS;
							else if (*blit_transition() < 0)
							{
								static SDL_Rect rbuf;
								rbuf.x = 0;
								rbuf.y = 0;
								rbuf.w = SCREEN_WIDTH;
								rbuf.h = SCREEN_HEIGHT;

								blit_fill_rect(0xFF);
								SDL_RenderPresent(renderer);

								SDL_Delay(2048);

								*blit_transition() =
									screen_mode = SCREEN_MODE_NORMAL;

								npcai = -1;

								trs_end_fn();
							}
						}
						else if (screen_mode == SCREEN_MODE_MENU_CNFRM)
						{
							menu_2_up_down_curs_set(e.key.keysym.sym, 2);

							if (keys[KEY_SELECT_1])
							{
								if (menu_sel_idx_2 == 0)
								{
									zero_menu_select();
									screen_mode =
										SCREEN_MODE_DISP_TRS;
								}
								else if (menu_sel_idx_2 == 1)
								{
									EXIT_MENU_ID(-1);
								}
							}
							else if (keys[KEY_CH_ACTION])
							{
								EXIT_MENU_ID(-1);
							}
						}
						else if (screen_mode == SCREEN_MODE_UM_EDIT_MENU)
						{
							if (e.key.keysym.sym == KEY_MENU_UP)
								menu_sel_idx_0 = pm(menu_sel_idx_0 - 1, 5);
							else if (e.key.keysym.sym == KEY_MENU_DOWN)
								menu_sel_idx_0 = pm(menu_sel_idx_0 + 1, 5);

							else if (keys[KEY_SELECT_1])
							{
								if (menu_sel_idx_0 == 0 || menu_sel_idx_0 == 1 || menu_sel_idx_0 == 2)
									screen_mode = SCREEN_MODE_UM_EDIT_RM;
								else if (menu_sel_idx_0 == 3)
								{
									um_map_save();
									screen_mode = SCREEN_MODE_TEXT;

									gtidx = 85 - 10;
									t_aidx = 0;
									t_bidx = 0;
								}
								else if (menu_sel_idx_0 == 4)
								{
									um_map_save();

									um_map_out_init();
									um_map_exit();

									blit_delay();
								}
							}
							else if (keys[KEY_CH_ACTION])
							{
								menu_sel_idx_0 = 0;
								screen_mode = SCREEN_MODE_NORMAL;
							}
						}
						else if (screen_mode == SCREEN_MODE_UM_EDIT_NEWMAP)
						{
							if (e.key.keysym.sym == 8 && gtexta[UM_NEWMAP_NAME_IDX].size - 1 >= 0)
							{
								char* s = &um_name_text[um_map_index * UM_MAX_MAP_NAME_TEXT_SIZE];

								s[strlen(s) - 1] = 0;
								gtexta[UM_NEWMAP_NAME_IDX].size--;
							}
							else if (keys[KEY_CH_ACTION])
							{
								memset(&um_name_text[UM_MAX_MAP_NAME_TEXT_SIZE * um_map_index], 0, UM_MAX_MAP_NAME_TEXT_SIZE);
								screen_mode = SCREEN_MODE_DISP_TERM;
							}
							else if (keys[KEY_SELECT_0])
							{
								if (0 == um_name_text[um_map_index * UM_MAX_MAP_NAME_TEXT_SIZE])
								{
									char buf[] = "NEW MAP X";
									buf[strlen(buf) - 1] = '0' + um_map_index;
									strcpy(um_name_text, buf);
								}
								else
								{
									int i;

									for (i = 0; i < UM_MAX_MAP; i++)
										if (i == um_map_index)
											continue;
										else if (
											0 ==
											strcmp(
												&um_name_text[um_map_index * UM_MAX_MAP_NAME_TEXT_SIZE]
												, &um_name_text[i * UM_MAX_MAP_NAME_TEXT_SIZE]
											))
											break;

									if (i < UM_MAX_MAP)
									{
										screen_mode = SCREEN_MODE_TEXT;

										gtidx = 84 - 10;
										t_aidx = 0;
										t_bidx = 0;

										memset(&um_name_text[um_map_index * UM_MAX_MAP_NAME_TEXT_SIZE], 0, UM_MAX_MAP_NAME_TEXT_SIZE);
										um_map_index = 0;

										break;
									}
								}

								int w = 9, h = 9;

								SDL_ResetKeyboard();

								for (;;)
								{
									SDL_PumpEvents();
									keys = SDL_GetKeyboardState(NULL);

									SDL_RenderClear(renderer);

									point pt;

									if (keys[KEY_CH_WALK_XR]) { SDL_Delay(256); w++; }
									else if (keys[KEY_CH_WALK_XL]) { SDL_Delay(256); w--; }
									else if (keys[KEY_CH_WALK_YR]) { SDL_Delay(256); h++; }
									else if (keys[KEY_CH_WALK_YL]) { SDL_Delay(256); h--; }

									else if (keys[KEY_SELECT_1] || keys[KEY_SELECT_0])
										break;

									if (w < 0) w = 0;
									else if (w > UM_MAX_MAP_WIDTH) w = UM_MAX_MAP_WIDTH;
									if (h < 0) h = 0;
									else if (h > UM_MAX_MAP_HEIGHT) h = UM_MAX_MAP_HEIGHT;

									for (int y = 0; y < h; y++)
										for (int x = 0; x < w; x++)
										{
											pt = pt_world_screen(x, y, 0);
											isoblit_z(&sprdata[MAT_FLOOR_BLANK_ID], pt.x, pt.y, calibrator);
										}

									SDL_RenderPresent(renderer);
									SDL_Delay(RENDER_CYCLE_DELAY);
								}

								{
									um_map_in_init();
									um_map_new(w, h);

									blit_delay();
								}

								EXIT_MENU_ID(-1);
							}
							else if (gtexta[UM_NEWMAP_NAME_IDX].size <= 16)
							{
								char buf[] = { e.key.keysym.sym,0 };

								if (buf[0] != e.key.keysym.sym)
									break;

								if (
									(SDL_GetModState() & KMOD_CAPS)
									|| keys[SDL_SCANCODE_RSHIFT]
									|| keys[SDL_SCANCODE_LSHIFT])
									if (buf[0] >= 'a' && buf[0] <= 'z')
										buf[0] += 'A' - 'a';
									else if (buf[0] >= '0' && buf[0] <= '9')
									{
										char map[] = ")!@#$%^&*(";
										buf[0] = map[buf[0] - '0'];
									}
									else if (buf[0] >= '0' && buf[0] <= '9')
									{
										char map[] = ")!@#$%^&*(";
										buf[0] = map[buf[0] - '0'];
									}
									else
									{
										char map0[] = "`-=[]\\;,./";
										char map1[] = "~_+{}|:<>?";

										for (int i = 0; i < sizeof(map0); i++)
											if (map0[i] == buf[0])
											{
												buf[0] = map1[i];
												break;
											}
									}

								for (int i = 0; i < strlen(CHARSET); i++)
									if (CHARSET[i] == buf[0])
									{
										char* s = &um_name_text[um_map_index * UM_MAX_MAP_NAME_TEXT_SIZE];
										s[strlen(s)] = buf[0];
										push_back_sprdataa_str(&(gtexta[UM_NEWMAP_NAME_IDX]), buf);
										break;
									}
							}
						}
						if (keys[KEY_SELECT_0])
						{
							if (cache_top > 0 && screen_mode == SCREEN_MODE_NORMAL)
							{
								menu_sel_idx_0 = 0;
								screen_mode = SCREEN_MODE_DISP_CACHE;
							}
							else if (screen_mode == SCREEN_MODE_DISP_CACHE)
								sm_exit_disp_cache();
						}
						else if (keys[KEY_SELECT_2])
						{
							if (screen_mode == SCREEN_MODE_DISP_CACHE) cache[menu_sel_idx_0].active ^= 1;
						}
						else if (keys[KEY_SELECT_CLEAR])
						{
							npcai = -1;
							npcidx = -1;
						}

						if (keys[KEY_CH_ACTION] || keys[KEY_SELECT_1])
						{
							if (keys[KEY_CH_ACTION])
								if (screen_mode == SCREEN_MODE_DISP_CACHE)
									sm_exit_disp_cache();

							if (screen_mode == SCREEN_MODE_TEXT && t_bidx > t_bptr_limit)
							{
								t_bptr_limit = T_BPTR_LIMIT_DEFAULT;

								if (npcidx != -1)
								{
									if (map.npca.arr[npcidx].fnidx != -1)
										npcfn[map.npca.arr[npcidx].fnidx](&map.npca.arr[npcidx]);
									else screen_mode = SCREEN_MODE_NORMAL;
								}
								else
								{
									screen_mode = SCREEN_MODE_NORMAL;
									SDL_Delay(125);
								}
							}
						}

						collision_full();
					}
					else if (e.type == SDL_KEYUP)
					{
						deltakey = 0;
						keys = SDL_GetKeyboardState(NULL);
					}
				}

				SDL_RenderClear(renderer);
				renderfn();

				for (int i = 0; i < cyfn.size; i++) cyfn.arr[i]();
				update_TScreenObjA(&tsoa);

				if (screen_mode == SCREEN_MODE_DISP_TRS)
				{
					if (trs_end_fn != &trs_new_load)
					{
						int* twipe = blit_transition();

						if (*twipe < 0)
						{
							*twipe = 0;
							screen_mode = SCREEN_MODE_NORMAL;
							trs_end_fn();

							SDL_Delay(750);
						}
					}
				}

				SDL_RenderPresent(renderer);
				SDL_Delay(RENDER_CYCLE_DELAY);
			}
		}
		else return 1;
	}
	else return 1;

	close();

	if (mapfp != NULL) fclose(mapfp);

	return 0;
}

void joinmv()
{
	map = readmapp(PATH_VAR_MAP, mapseek);
	linkmap();
	readallcid(PATH_CID, &(map.cache));
	init_cache();
	free_bthmupdatea(&bthmupdate);
	bthmupdate = create_bthmupdatea(1);
}

void joinmv_f()
{
	free_map(&map);
	map = readmapp(PATH_VAR_MAP, mapseek);
	linkmap();
	init_cache();
	free_bthmupdatea(&bthmupdate);
	bthmupdate = create_bthmupdatea(1);
}