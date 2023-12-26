#include "display.h"
#include "dglobal.h"

#include "build.h"
#include SDL_SDL_INCLUDE

void isoblit_z(s_spritedata* sd, float x, float y, float z)
{
	surfacerect = (SDL_Rect){ (int)(SCREEN_CX - sd->CX + ((16 * 2) * x)), (int)(SCREEN_CY - sd->CY + (16 * y) - (z * 32)), sd->OW, sd->OH };
	tilerect = (SDL_Rect){ sd->OX, sd->OY + tileo, sd->OW, sd->OH };
	SDL_RenderCopy(renderer, tile, &tilerect, &surfacerect);
}

void isoblit_z_flip(s_spritedata* sd, float x, float y, float z)
{
	surfacerect = (SDL_Rect){ (int)(SCREEN_CX - sd->CX + ((16 * 2) * x)), (int)(SCREEN_CY - sd->CY + (16 * y) - (z * 32)), 0, 0 };
	tilerect = (SDL_Rect){ sd->OX, sd->OY + tileh / 2, sd->OW, sd->OH };
	SDL_RenderCopyEx(renderer, tile, &tilerect, &surfacerect, 0, NULL, SDL_FLIP_HORIZONTAL);
}

void isoblit(s_spritedata* sd, float x, float y)
{
	surfacerect = (SDL_Rect){ (int)(SCREEN_CX - sd->CX + ((16 * 2) * x)), (int)(SCREEN_CY - sd->CY + (16 * y)), sd->OW, sd->OH };
	tilerect = (SDL_Rect){ sd->OX, sd->OY, sd->OW, sd->OH };
	SDL_RenderCopy(renderer, tile, &tilerect, &surfacerect);
}

void quadblit(s_spritedata* sd, float x, float y)
{
	surfacerect = (SDL_Rect){ (int)x, (int)y - sd->CY, sd->OW, sd->OH };
	tilerect = (SDL_Rect){ sd->OX, sd->OY, sd->OW, sd->OH };
	SDL_RenderCopy(renderer, tile, &tilerect, &surfacerect);
}

void isoblit_flip(s_spritedata* sd, float x, float y)
{
	surfacerect = (SDL_Rect){ (int)
		(SCREEN_CX - (+sd->OW - sd->CX) + ((16 * 2) * x))
		, (int)(SCREEN_CY - sd->CY + (16 * y)), sd->OW, sd->OH };
	tilerect = (SDL_Rect){ sd->OX, sd->OY, sd->OW, sd->OH };
	SDL_RenderCopyEx(renderer, tile, &tilerect, &surfacerect, 0, NULL, SDL_FLIP_HORIZONTAL);
}

void blit_npc(n_npc* npc, float x, float y, float z)
{
	if (npc->ch.flip)
		isoblit_flip(&(sprdata[npc->ch.sprite.id]), x, y);
	else
		isoblit(&(sprdata[npc->ch.sprite.id]), x, y);
}

void blit_ch(ch_char* ch)
{
	if (ch->flip)
		isoblit_flip(&(sprdata[ch->sprite.id]), 0, 0);
	else
		isoblit(&(sprdata[ch->sprite.id]), 0, 0);
}

void blit_elem_cache(e_element* cache, int sprb, int token)
{
	blit_fill_rect(0xFF);

	char l = 0;
	for (int i = 0; (i < ELEM_CACHE_MAX); i++)
	{
		l |= (cache[i].id == -1);

#define SECTION_AREA	ELEM_AREA

#define SECTION_OFFSET	(16 * 1)

#define ELEM_SCREEN_WIDTH	4
		int ex = SECTION_AREA * (i % ELEM_SCREEN_WIDTH) - (ELEM_SCREEN_WIDTH * (SECTION_AREA / 2)) + (SCREEN_WIDTH / 2);
		int ey = SECTION_AREA * (i / ELEM_SCREEN_WIDTH) - ((ELEM_CACHE_MAX / ELEM_SCREEN_WIDTH) * (SECTION_AREA / 2)) + (SCREEN_HEIGHT / 2)
			+ SECTION_OFFSET * (i / ELEM_SCREEN_WIDTH);

		int baa = (SECTION_AREA / (16 + 2) + (SECTION_AREA > ELEM_AREA));
		int ba = ELEM_AREA / (16 + 2);

		int bx = ex / 16;
		int by = ey / 16;

		blit_border(SPRITE_BORD_OFFSET, bx, by, ba + bx, by + ba + 1);

		if (blit_tick_elem_cache && menu_sel_idx_1 == i && seltik) continue;
		else if (!l)
		{
			blit_std(SPRITE_ELEM_OFFSET + cache[i].id,
				ex - (ELEM_AREA) / 2 + (SECTION_AREA) / 2,
				ey - (ELEM_AREA) / 2 + (SECTION_AREA) / 2);

			for (int ii = 0; ii < 4; ii++)
			{
				int o = !((int)(((1 + cache[i].spra) / ELEM_SPR_BLOCK)) > (ii));
				quadblit(
					&(sprdata[SPRITE_FLAT_OFFSET + o])
					, ((ii + ex) + 5) + ((ii) * (20 - 1)) + (36 / 2)
					, (ey + 124)
				);
			}
		}
	}

	UPDATE_TICK();


#define BORD_TOKEN_MARK (SPRITE_BORD_OFFSET + 12)

	{
		SDL_Rect rr = (SDL_Rect){ 4 * 16, 25 * 16, 32 * 16, 2 * 16 };

		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(renderer, &rr);

		SDL_SetRenderDrawColor(renderer, r, g, b, a);
	}

	for (int i = 0; i < 16; i++)
	{
		int offs = SPRITE_BORD_OFFSET + 13 + ((i) >= (sprb / SPR_BLOCK));

		tilerect = (SDL_Rect){ sprdata[offs].OX, sprdata[offs].OY, sprdata[offs].OW, sprdata[offs].OH };
		surfacerect = (SDL_Rect)
		{
			(16 * 4) + (i * sprdata[offs].OW) + (16 * 32) / 2 - (sprdata[offs].OW * 16) / 2,
			16 * 26 - 4,
			sprdata[offs].OW, sprdata[offs].OH,
		};
		SDL_RenderCopy(renderer, tile, &tilerect, &surfacerect);
	}

	{
		tilerect = (SDL_Rect){ sprdata[BORD_TOKEN_MARK].OX, sprdata[BORD_TOKEN_MARK].OY, sprdata[BORD_TOKEN_MARK].OW, sprdata[BORD_TOKEN_MARK].OH };
		surfacerect = (SDL_Rect){ 16 * (+26 + 5 - 1) - sprdata[BORD_TOKEN_MARK].CX * 2, 16 * (6) - sprdata[BORD_TOKEN_MARK].CY, sprdata[BORD_TOKEN_MARK].OW, sprdata[BORD_TOKEN_MARK].OH };
		SDL_RenderCopy(renderer, tile, &tilerect, &surfacerect);
	}

	for (int i = 0; i < 6; i++)
	{
		s_spritedata* at = &(sprdata[((token / (int)pow(10, (6 - 1) - i)) % 10) + TEXT_0]);

		tilerect = (SDL_Rect){ at->OX, at->OY, at->OW, at->OH };
		surfacerect = (SDL_Rect){ 16 * (-1 + 6 + i + 26 + 0) - at->CX - sprdata[BORD_TOKEN_MARK].CX, 16 * (6) - at->CY, at->OW, at->OH };
		SDL_RenderCopy(renderer, tile, &tilerect, &surfacerect);
	}
}

int* blit_transition()
{
	static SDL_Rect wiper = { 0, 0, 0, (SCREEN_HEIGHT / 16) };

	static int wipex = 0;

	char black = 0x00;
	char white = 0xFF;
	white = black;

	wiper.w = wipex % (SCREEN_WIDTH + (SCREEN_WIDTH / 8));

	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	for (int ii = 0; ii < 16 / 2; ii++)
	{
		wiper.x = SCREEN_WIDTH - wiper.w - wiper.x;
		wiper.y = (2 * ii) * wiper.h;
		SDL_RenderFillRect(renderer, &wiper);

		wiper.x = SCREEN_WIDTH - wiper.w - wiper.x;
		wiper.y = (2 * ii + 1) * wiper.h;
		SDL_RenderFillRect(renderer, &wiper);
	}
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	return &wipex;
}

inline void blit_elem_c(int id)
{
	int xo = -ELEM_AREA / 2 + SCREEN_WIDTH / 2;
	int yo = -ELEM_AREA / 2 + SCREEN_HEIGHT / 2;
	blit_border(SPRITE_BORD_OFFSET, xo / 16, (yo / 16), (xo / 16) + (128 / 16 - 1), (yo / 16) + 128 / 16 - 1);
	blit_std(id, xo, yo);
}

void blit_status_bar(stat_bar_dsp* p)
{
#define BORD_COLOR	6

	int o = 7;
	int yy = (SCREEN_HEIGHT - 128);
	blit_border(SPRITE_BORD_OFFSET + BORD_COLOR, 0, yy / 16, o + 32, o + yy / 16);

	blit_column(SPRITE_BORD_OFFSET + BORD_COLOR, o, yy / 16 + 1, -1 + o + yy / 16, tileh / 2);
	blit_column(SPRITE_BORD_OFFSET + BORD_COLOR, (SCREEN_WIDTH / 2) / 16, yy / 16 + 1, -1 + o + yy / 16, 0);
	blit_column(SPRITE_BORD_OFFSET + BORD_COLOR, o + (SCREEN_WIDTH / 2) / 16, yy / 16 + 1, -1 + o + yy / 16, SCREEN_HEIGHT / 2);

	if (p->e0 != -1) blit_std(p->e0, 0, yy);
	for (int i = 0; i < p->s0; i++)
		quadblit(&(sprdata[SPRITE_FLAT_OFFSET + (i >= p->sp0)])
			, (ELEM_AREA - 1) + ((i / 2) * (1.5f * 16))
			, (yy + 44) + ((i % 2) * (1.5f * 16))
		);

	float oo
		= SCREEN_WIDTH / 2
		- ((p->s1 >= 16 - 1) * 5);

	if (p->e1 != -1) blit_std(p->e1, oo, yy);
	for (int i = 0; i < p->s1; i++)
		quadblit(&(sprdata[SPRITE_FLAT_OFFSET + (i >= p->sp1)])
			, (oo + ELEM_AREA - 1) + ((i / 2) * (1.5f * 16))
			, (yy + 44) + ((i % 2) * (1.5f * 16))
		);

	{
		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
#if BORD_COLOR == 0
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
#else
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
#endif
		for (int ii = 0; ii < 2; ii++)
			for (int i = 0; i < (&p->l0)[ii]; i++)
			{
				static SDL_Rect rbuf = { 0, SCREEN_HEIGHT - 16 - 16, 4, 16 };

				rbuf.x = (16) * (8 + i) - 8 * i - 8
					+ ii * 16 * 20
					+ (8 + 2)
					;

				SDL_RenderFillRect(renderer, &rbuf);
			}
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
	}
}

void blit_border(int id, int x0, int y0, int x1, int y1)
{
	int i = 0;

	for (int x = x0; x <= x1; x++)
		for (int y = y0; y <= y1; y++)
		{

			if (x == x0 && y == y0) i = 0;
			else if ((x > x0 && x < x1) && y == y0) i = 1;
			else if (x == x1 && y == y0) i = 0;
			else if (x == x0 && (y > y0 && y < y1)) i = 2;
			else if ((x > x0 && x < x1) && (y > y0 && y < y1)) i = 3;
			else if (x == x1 && (y > y0 && y < y1)) i = 2;
			else if (x == x0 && y == y1) i = 4;
			else if ((x > x0 && x < x1) && y == y1) i = 5;
			else if (x == x1 && y == y1) i = 4;

			int ida = id + i;
			{
				int w = sprdata[ida].OW, h = sprdata[id + i].OH;

				surfacerect = (SDL_Rect){ x * sprdata[ida].OW, y * sprdata[ida].OH, w, h };
				tilerect = (SDL_Rect){ sprdata[ida].OX, sprdata[ida].OY, w, h };
				SDL_RenderCopyEx(renderer, tile, &tilerect, &surfacerect
					, 0, NULL,
					(SDL_RendererFlip)(x > ((x1 - x0) / 2 + x0))
				);
			}
		}
}

void blit_column(int id, int x, int y0, int y1, char flip)
{
	int i = 2;

	for (int y = y0; y <= y1; y++)
	{
		int w = sprdata[id + i].OW, h = sprdata[id + i].OH;
		surfacerect = (SDL_Rect){ x * sprdata[id + i].OW, y * sprdata[id + i].OH, w, h };
		tilerect = (SDL_Rect){ sprdata[id + i].OX, sprdata[id + i].OY, w, h };

		SDL_RenderCopyEx(renderer, tile, &tilerect, &surfacerect,
			0,
			NULL,
			(SDL_RendererFlip)(flip)
		);
	}
}

void blit_textbox_std(s_spritedata_a* t, int x, int y, int w, int h)
{
	int o = 1 * 2;
	int ho = 2;

	int tox = +(SCREEN_WIDTH / 2) - ((TEXT_WIDTH * (w - o)) / 2);
	int toy = 2 * (+(SCREEN_HEIGHT / 2) - ((TEXT_HEIGHT * (h + ho)) / 2));

	blit_border(SPRITE_BORD_OFFSET + 6,
		3,
		18,
		36,
		27
	);

	t_aidx %= t->size;
	for (int i = t_aidx % t->size; i <= t_bidx % t->size; i++)
	{
		if (((i - t_aidx) / t_width) >= h)
			(t_aidx += t_width);

		{
			int w = t->arr[i]->OW, h = t->arr[i]->OH;

			surfacerect = (SDL_Rect){ tox + x + TEXT_WIDTH * ((i) % t_width), toy + y + TEXT_HEIGHT * (-1 + ((i - t_aidx) / t_width)) - t->arr[i]->CY, w, h };
			tilerect = (SDL_Rect){ t->arr[i]->OX, t->arr[i]->OY, w, h };
			SDL_RenderCopy(renderer, tile, &tilerect, &surfacerect);
		}
	}
}

void blit_ls(icac_itemcache* cache, int cache_top, int cursor)
{
	int w = (SCREEN_WIDTH / TEXT_WIDTH) - 1,
		h = (SCREEN_HEIGHT / TEXT_WIDTH) - (128 / 16) - 1;

	int wo = 4 * 0;
	int ho = 8 * 0;

	int x0 = -w + -1 + (SCREEN_WIDTH / TEXT_WIDTH) + wo;
	int y0 = 0 + ho;
	int x1 = x0 + w - wo * 2;
	int y1 = y0 + h - ho * 2;

	x0 = 0;
	x1 = w;

	blit_border(SPRITE_BORD_OFFSET + 6, x0, y0, x1, y1);

	s_spritedata* at;

#define SCREEN_CACHE_MAX	18

	if (cursor > SCREEN_CACHE_MAX + ls_ptr)
	{
		if (SCREEN_CACHE_MAX + ls_ptr < ICACHE_MAX_SIZE) ls_ptr++;
		cursor = SCREEN_CACHE_MAX + ls_ptr;
	}
	else if (cursor < ls_ptr)
	{
		if (ls_ptr > 0) ls_ptr--;
		cursor = ls_ptr;
	}

	int ls_max = min(SCREEN_CACHE_MAX + ls_ptr, cache_top - 1);
	if (cursor > ls_max) cursor = ls_max;

#define TEXT_MARK (SPRITE_TEXT_OFFSET + 110 + 0*124 + 0*97)

	for (int i = ls_ptr; i <= ls_max; i++)
	{
		if (cache[i].active)
		{
			at = &(sprdata[TEXT_MARK]);
			surfacerect = (SDL_Rect){ (x0 + 2) * TEXT_WIDTH - at->CX, (i + y0 + 1 - ls_ptr) * TEXT_HEIGHT - at->CY, at->OW, at->OH };
			tilerect = (SDL_Rect){ at->OX, at->OY, at->OW, at->OH };
			SDL_RenderCopy(renderer, tile, &tilerect, &surfacerect);
		}

		int tid = cache[i].id;
		char tmax = itexta[tid].size > (w - 6) ? (w - 6) : itexta[tid].size;
		for (int ii = 0; ii < tmax; ii++)
		{
			at = itexta[tid].arr[ii];

			if (at - sprdata != SPRITE_TEXT_OFFSET)
			{
				surfacerect = (SDL_Rect){ (ii + x0 + 1 + 1 + 4 + 1) * TEXT_WIDTH - at->CX, (i + y0 + 1 - ls_ptr) * TEXT_HEIGHT - at->CY, at->OW, at->OH };
				tilerect = (SDL_Rect){ at->OX, at->OY, at->OW, at->OH };
				SDL_RenderCopy(renderer, tile, &tilerect, &surfacerect);
			}
		}

		if (cache[i].mul > 1)
		{
			at = &(sprdata[TEXT_X + SPRITE_TEXT_OFFSET]);

			surfacerect = (SDL_Rect){ (x0 + 1 + 1 + 1) * TEXT_WIDTH - at->CX, (y0 + 1 + i - ls_ptr) * TEXT_HEIGHT - at->CY, at->OW, at->OH };
			tilerect = (SDL_Rect){ at->OX, at->OY, at->OW, at->OH };
			SDL_RenderCopy(renderer, tile, &tilerect, &surfacerect);

			for (int iii = 0; iii < 2; iii++)
			{
				at = &(sprdata[(cache[i].mul / (int)pow(10, iii)) % 10 + TEXT_0]);

				surfacerect = (SDL_Rect){ (x0 + 1 + 1 + 2 + 1 - +iii) * TEXT_WIDTH - at->CX, (y0 + 1 + i - ls_ptr) * TEXT_HEIGHT - at->CY, at->OW, at->OH };
				tilerect = (SDL_Rect){ at->OX, at->OY, at->OW, at->OH };
				SDL_RenderCopy(renderer, tile, &tilerect, &surfacerect);
			}
		}
	}

	at = &(sprdata[TEXT_ARROW_R_IDX]);
	blit_ls_cursor(at, x0, y0 + cursor);
}

void blit_ls_cursor(s_spritedata* at, int x0, int y0)
{
	int im = (tik / (at->FD * 32)) % 2;
	int io = at->FJ;
	at = &(at[im * io]);

	surfacerect = (SDL_Rect){ (x0 + 1) * TEXT_WIDTH - at->CX, (y0 + 1 - ls_ptr) * TEXT_HEIGHT - at->CY, at->OW, at->OH };
	tilerect = (SDL_Rect){ at->OX, at->OY, at->OW, at->OH };
	SDL_RenderCopy(renderer, tile, &tilerect, &surfacerect);
}

void blit_text_cursor(int x, int y)
{
	char c = 255 * ((tik / 512) % 2);

	surfacerect = (SDL_Rect){ -8 + (x + 1) * TEXT_WIDTH, 6 + (y + 1) * TEXT_HEIGHT, 16, 16 / 8 };

	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

	SDL_SetRenderDrawColor(renderer, c, c, c, 255);
	SDL_RenderFillRect(renderer, &surfacerect);

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void blit_ctg_menu_txt(int idx, int h, int w)
{
	s_spritedata_a* at;

	int x0 = +((SCREEN_WIDTH / 16) / 2)
		- ((2 + w) / 2);
	int y0 = +((SCREEN_HEIGHT / 16) / 2)
		- floor(h / 2.0f) + 0 + 1
		- 2
		;
	int x1 = +((SCREEN_WIDTH / 16) / 2)
		+ ((2 + w) / 2);
	int y1 = +((SCREEN_HEIGHT / 16) / 2)
		+ ceil(h / 2.0f) + 1 - 4
		+ 2
		+ 1
		;

	blit_border(SPRITE_BORD_OFFSET + 6
		, x0
		, y0
		, x1
		, y1);

	for (int i = 0; i < h; i++)
	{
		at = &gtexta[idx + i];
		blit_text(at, 0,
			+((SCREEN_HEIGHT / 16) / 2)
			- h / 2
			- 2
			+ i,
			+(
				+((SCREEN_WIDTH / 16) / 2)
				- (w / 2)
				)
			+ (at->size - 2)

			+ (i == 0) *
			(+(w / 2) - (at->size / 2))
			, i != 0);
	}

	blit_text_cursor(x0 + gtexta[idx + h - 1].size + 1, y1 - 1);
}

void blit_ctg_menu_cnt(int idx, int h, int w)
{
	s_spritedata_a* at;

	int x0 = +((SCREEN_WIDTH / 16) / 2)
		- ((2 + w) / 2);
	int y0 = +((SCREEN_HEIGHT / 16) / 2)
		- floor(h / 2.0f) + 0 + 1
		- 2
		;
	int x1 = +((SCREEN_WIDTH / 16) / 2)
		+ ((2 + w) / 2);
	int y1 = +((SCREEN_HEIGHT / 16) / 2)
		+ ceil(h / 2.0f) + 1 - 4
		+ 2
		;

	blit_border(SPRITE_BORD_OFFSET + 6
		, x0
		, y0
		, x1
		, y1);

	for (int i = 0; i < h; i++)
	{
		at = &gtexta[idx + i];
		blit_text(at, 0,
			+((SCREEN_HEIGHT / 16) / 2)
			- h / 2
			- 2
			+ i,
			+(
				+((SCREEN_WIDTH / 16) / 2)
				- (w / 2)
				)
			+ (at->size - 2),
			0);
	}

	blit_ls_cursor(
		&(sprdata[TEXT_ARROW_R_IDX]),
		x0,
		menu_sel_idx_0 + y0
	);
}

void blit_ctg_menu_crn_ex(int idx, int h, int w, int curs, int offs)
{
	s_spritedata_a* at;

	int x0 = +((SCREEN_WIDTH / 16) / 1) - w;
	int y0 = 0;
	int x1 = +((SCREEN_WIDTH / 16) / 1) - 1;
	int y1 = h;

	blit_border(SPRITE_BORD_OFFSET + 6
		, x0
		, y0
		, x1
		, y1);

	for (int i = 0; i < h; i++)
	{
		at = &gtexta[idx + i];
		blit_text(at, 0
			, +y0
			+ i
			- 1
			, +x0
			+ (w - 3)
			- offs + (offs != 0) * (at->size)
			- 1
			, 0);
	}

	blit_ls_cursor(
		&(sprdata[TEXT_ARROW_R_IDX]),
		x0,
		curs + y0);
}

void blit_confirm_menu()
{
#define YES_NO_MENU_IDX	11
#define YES_NO_MENU_W	(gtexta[YES_NO_MENU_IDX + sel_elem_menu_ofs].size+2+2)

	int x0 = (SCREEN_WIDTH / 16) - YES_NO_MENU_W;
	int y0 = 0;
	int x1 = x0 + YES_NO_MENU_W - 1;
	int y1 = y0 + 3 + 3;

	blit_border(SPRITE_BORD_OFFSET + 6, x0, y0, x1, y1);

	blit_text(&(gtexta[YES_NO_MENU_IDX + sel_elem_menu_ofs]), x0, y0, gtexta[YES_NO_MENU_IDX + sel_elem_menu_ofs].size, 0);
	blit_text(&(gtexta[YES_NO_MENU_IDX + 2]), x0, y0, gtexta[YES_NO_MENU_IDX + sel_elem_menu_ofs].size, 2);
	blit_text(&(gtexta[YES_NO_MENU_IDX + 3]), x0, y0, gtexta[YES_NO_MENU_IDX + sel_elem_menu_ofs].size, 3);

	blit_ls_cursor(&(sprdata[TEXT_ARROW_R_IDX]), x0, 3 + menu_sel_idx_2);
}

void blit_cast_menu(short* casts)
{
	int max = 0;
	s_spritedata_a* at;

	for (int i = 0; i < 4; i++)
	{
		if (casts[i] == -1) continue;
		at = &gtexta[casts[i] + TEXT_CAST_OFFSET];
		if (at->size > max) max = at->size;
	}

	int xo = +((SCREEN_WIDTH / 16) / 2)
		- ((2 + max) / 2);
	int yo = +((SCREEN_HEIGHT / 16) / 2)
		- 4
		+ 0 + 1;

	blit_border(SPRITE_BORD_OFFSET + 6,
		xo,
		yo,
		+((SCREEN_WIDTH / 16) / 2)
		+ ((2 + max) / 2),
		+((SCREEN_HEIGHT / 16) / 2)
		- 4
		+ 4 + 1);

	for (int i = 0; i < 4; i++)
	{
		if (casts[i] == -1) continue;
		at = &gtexta[casts[i] + TEXT_CAST_OFFSET];
		blit_text(at, 0,
			+((SCREEN_HEIGHT / 16) / 2)
			- 4
			+ i,
			+(
				+((SCREEN_WIDTH / 16) / 2)
				- (max / 2)
				)
			+ (at->size - 2),
			0);
	}

	blit_ls_cursor(
		&(sprdata[TEXT_ARROW_R_IDX]),
		xo,
		menu_sel_idx_2 + yo
	);
}

void blit_text_str_cnt(char* str, char blocksize)
{
	int w = 16, h = 8;

	int x0 = +((SCREEN_WIDTH / 16) / 2)
		- ((2 + w) / 2);
	int y0 = +((SCREEN_HEIGHT / 16) / 2)
		- floor(h / 2.0f) + 0 + 1
		- 2
		;
	int x1 = +((SCREEN_WIDTH / 16) / 2)
		+ ((2 + w) / 2);
	int y1 = +((SCREEN_HEIGHT / 16) / 2)
		+ ceil(h / 2.0f) + 1 - 4
		+ 2
		+ 1
		;

	blit_border(SPRITE_BORD_OFFSET + 6
		, x0
		, y0
		, x1
		, y1);

	s_spritedata_a* at = &gtexta[86 - 10];

	for (int i = 0; i < 4; i++)
	{
		at->size = 0;

		push_back_sprdataa_str(at, &str[blocksize * i]);
		blit_text(at, x0, y0, -1 + at->size, i - 1);
	}

	blit_ls_cursor(&sprdata[TEXT_ARROW_R_IDX], x0, y0 + menu_sel_idx_2);
}

void blit_text(s_spritedata_a* txt, int x0, int y0, int lo, int to)
{
	for (int i = 0; i < txt->size; i++)
	{
		if (TEXT_SPACE_INDEX == txt->arr[i] - sprdata)
			continue;

		s_spritedata* at = txt->arr[i];

		surfacerect = (SDL_Rect){ (x0 + i + 2 + 1 + (lo - txt->size)) * 16 - at->CX, (y0 + 2 + to) * 16 - at->CY, at->OW, at->OH };
		tilerect = (SDL_Rect){ at->OX, at->OY, at->OW, at->OH };
		SDL_RenderCopy(renderer, tile, &tilerect, &surfacerect);
	}
}

void blit_fill_rect(unsigned char lum)
{
	const SDL_Rect rr = (SDL_Rect){ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, };

	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, lum, lum, lum, 0xFF);
	SDL_RenderFillRect(renderer, &rr);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void blit_delay()
{
	blit_fill_rect(0xFF);
	SDL_RenderPresent(renderer);
	SDL_Delay(1024);
}

char display_init()
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

	return 0;
}

#ifndef COLOR_GRAY
#define COLOR_GRAY (0xFF / 2)
#endif

#ifndef COLOR_GRAY_R
#define COLOR_GRAY_R COLOR_GRAY
#endif
#ifndef COLOR_GRAY_G
#define COLOR_GRAY_G COLOR_GRAY
#endif
#ifndef COLOR_GRAY_B
#define COLOR_GRAY_B COLOR_GRAY
#endif

char display_loadmedia()
{
	SDL_Surface* bmp = SDL_LoadBMP(PATH_BMP);
	tilew = bmp->w;
	tileh = bmp->h;
	SDL_SetColorKey(bmp, SDL_TRUE, SDL_MapRGB(bmp->format, COLOR_GRAY_R, COLOR_GRAY_G, COLOR_GRAY_B));

	tile = SDL_CreateTextureFromSurface(renderer, bmp);
	SDL_FreeSurface(bmp);

	if (tile == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", PATH_BMP, SDL_GetError());
		return 1;
	}

	FILE* fp = fopen(PATH_STD, "rb");
	fread(sprdata, sizeof(s_spritedata), NUM_SPRITE, fp);
	fclose(fp);

	dctrl_init_all();

	return (tile == NULL) | (fp == NULL);
}

void display_close()
{
	dctrl_free_all();

	SDL_DestroyTexture(tile);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDL_Quit();
}

void render_select_elem()
{
	blit_fill_rect(0x00);

	for (int i = 0; i < 8 * (3); i++)
	{
		int x = ((i % 8) * 8);
		int y = 3 + 8 * (i / 8);

		blit_border(SPRITE_BORD_OFFSET + 6, x, y, x + (128 / 16) - 1, y + (128 / 16) - 1);
	}

	char m = (SCREEN_HEIGHT / 128) * (SCREEN_WIDTH / 128);
	char o = m * (menu_sel_idx_1 / m);

	for (int i = o; i < o + m && i < estore_top; i++)
	{
		int x = (i - o) % (SCREEN_WIDTH / 128);
		int y = (i - o) / (SCREEN_WIDTH / 128);

		if (i != menu_sel_idx_1 || seltik)
			blit_std(estore[i].id * 8 + SPRITE_ELEM_OFFSET, x * 128, y * 128 - ((128 * (SCREEN_HEIGHT / 128)) / 2) + SCREEN_HEIGHT / 2);
	}

	UPDATE_TICK();

	if (menu_sel_idx_2 != -1)
	{
		blit_confirm_menu();
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

void blit_std(int i, int x, int y)
{
	surfacerect = (SDL_Rect){ x, y, sprdata[i].OW, sprdata[i].OH };
	tilerect = (SDL_Rect){ sprdata[i].OX, sprdata[i].OY, sprdata[i].OW, sprdata[i].OH };
	SDL_RenderCopy(renderer, tile, &tilerect, &surfacerect);
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

				if (status_bar_mode && ii == npcai) isoblit(&(sprdata[SPRITE_TARGET_OFFSET]), xx, yy);
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

						if (status_bar_mode && ii == npcai) isoblit(&(sprdata[SPRITE_TARGET_OFFSET]), xx, yy);
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

					if (at->id >= 0)
					{
						point tscr = pt_world_screen(at->x + map.arr[i].ox, at->y + map.arr[i].oy, 0);

						int id = at->id;
						id += ((tik - at->deltatime) / (((1024 + 512) / 256) * (sprdata[id].FD + 1))) % (sprdata[id].FJ + 1);

						isoblit_z(
							&(sprdata[id]),
							tscr.x - chscr.x,
							tscr.y - chscr.y,
							at->z
						);
					}
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
			e1m < 0 ? -1 : (SPRITE_ELEM_OFFSET + e1m)
			, (npcai < 0 ? -1 : (e2m < 0 ? -1 : SPRITE_ELEM_OFFSET + e2m))
			, (abs(ch.sprb) + 1) / (unsigned int)SPR_BLOCK
			, (abs(ch.spra) + (SPR_BLOCK - (1 << 4))) / (unsigned int)SPR_BLOCK
			, ((npcai < 0 ? 0 : abs(npc->sprb)) + 1) / (unsigned int)SPR_BLOCK
			, ((npcai < 0 ? 0 : abs(npc->spra)) + (SPR_BLOCK - (1 << 4))) / (unsigned int)SPR_BLOCK
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

			blit_border(SPRITE_BORD_OFFSET + 6, x0, y0, x1, y1);

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

		blit_border(SPRITE_BORD_OFFSET + 6, x0, y0, x1, y1);

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

		blit_border(6 + SPRITE_BORD_OFFSET,
			(SCREEN_WIDTH / 2) / BORD_SIZE - (ELEM_AREA / BORD_SIZE) / 2,
			(SCREEN_HEIGHT / 2) / BORD_SIZE - (ELEM_AREA / BORD_SIZE) / 2,
			(SCREEN_WIDTH / 2) / BORD_SIZE + (ELEM_AREA / BORD_SIZE) / 2 - 1,
			(SCREEN_HEIGHT / 2) / BORD_SIZE + (ELEM_AREA / BORD_SIZE) / 2 - 1
		);
		blit_std(SPRITE_ELEM_OFFSET + disp_elem_idx, xc, yc);
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
		blit_ctg_menu_txt(TEXT_UM_NEWMAP_NAME_IDX - (2 - 1), 2, 16);
}
