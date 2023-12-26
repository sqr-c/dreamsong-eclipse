#include "keybinding.h"

#include "dglobal.h"

char actkeys[4] = { 0,0,0,0 };
int actkeyidx = 0;

char key_action_down_state()
{
	return 0
		|| keys[KEY_CH_ACTION]
		|| keys[KEY_CH_ACTION_0]
		|| keys[KEY_CH_ACTION_1]
		|| keys[KEY_CH_ACTION_2]
		;
}

char key_action_down()
{
	if (actkeys[0] = keys[KEY_CH_ACTION])
	{
		actkeyidx = 0;
		return 1;
	}
	else if (actkeys[1] = keys[KEY_CH_ACTION_0])
	{
		actkeyidx = 1;
		return 1;
	}
	else if (actkeys[2] = keys[KEY_CH_ACTION_1])
	{
		actkeyidx = 2;
		return 1;
	}
	else if (actkeys[3] = keys[KEY_CH_ACTION_2])
	{
		actkeyidx = 3;
		return 1;
	}
	else
	{
		actkeyidx = -1;
		return 0;
	}
}

char key_walk_down()
{
	return
		((keys[KEY_CH_WALK_XL]
			|| keys[KEY_CH_WALK_XR]
			|| keys[KEY_CH_WALK_YL]
			|| keys[KEY_CH_WALK_YR]) && !keys[KEY_CH_ACTION]);
}