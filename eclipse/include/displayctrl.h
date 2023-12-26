#pragma once

#include "build.h"
#include SDL_SDL_INCLUDE

void dctrl_free_all();
void dctrl_init_texts();
void dctrl_zero_menu_select();
char dctrl_menu_up_down_curs_set(SDL_Keycode sym, int max);
char dctrl_menu_1_up_down_curs_set(SDL_Keycode sym, int max);
char dctrl_menu_2_up_down_curs_set(SDL_Keycode sym, int max);
void dctrl_exit_menu_set_tidx(int tidx);
void dctrl_toggle_elem_screen();
void dctrl_init_all();
void main_loop();