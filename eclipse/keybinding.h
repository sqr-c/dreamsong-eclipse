#pragma once

#define alpha(x)	(SDL_SCANCODE_A - 'a' + (x))

#define KEY_WINDOW_EXIT	'q'

#define KEY_CH_WALK_XL	alpha('w')
#define KEY_CH_WALK_XR	alpha('s')
#define KEY_CH_WALK_YL	alpha('d')
#define KEY_CH_WALK_YR	alpha('a')

#define KEY_MENU_UP		'w'
#define KEY_MENU_DOWN	's'
#define KEY_MENU_RIGHT	'd'
#define KEY_MENU_LEFT	'a'

#define KEY_CH_ACTION			SDL_SCANCODE_BACKSLASH

#define KEY_CH_ACTION_0			SDL_SCANCODE_RIGHTBRACKET
#define KEY_CH_ACTION_1			SDL_SCANCODE_LEFTBRACKET
#define KEY_CH_ACTION_2			SDL_SCANCODE_P

#define KEY_STATUSBAR_TOGGLE		alpha('t')

#define KEY_SELECT_0		SDL_SCANCODE_RETURN
#define KEY_SELECT_1		SDL_SCANCODE_BACKSPACE
#define KEY_SELECT_2		SDL_SCANCODE_RSHIFT

#define KEY_SWAP_LEFT		SDL_SCANCODE_MINUS
#define KEY_SWAP_RIGHT		SDL_SCANCODE_EQUALS

#define KEY_SELECT_CLEAR	SDL_SCANCODE_PERIOD

#define KEY_APPLY_ICACHE	SDL_SCANCODE_0

#define KEY_ELEM_SWAP		SDL_SCANCODE_SEMICOLON

#define KEY_TEST_0		'z'
#define KEY_TEST_1		'x'

extern char actkeys[4];
extern int actkeyidx;
char action_down();
char action_down_state();