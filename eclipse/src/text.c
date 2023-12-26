#include "text.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

s_spritedata_a gtexta[GTEXT_SIZE];
const char* glob_textstr[GTEXT_SIZE] =
{
	"It doesn't seem to respond.",
	"You found a XXXXXXXXXXXXXXXX",
	"Welcome, here we dive! %%122%%121%%122%%121",
	"Here, I thought you should have this.",
	"Do you need more?",
	"Insert ELEMENT",
	"Remove ELEMENT",
	"Insert ITEM",
	"Remove ITEM",
	"Save",
	"Exit",
	"Insert Element?",
	"Remove Element?",
	"YES",
	"NO",
	"Transfer cannot be completed.",
	"Transfer successful!",
	"Save Successful!",
	"It's good to see you finally woke up! Where are you headed?",
	"What's that?",
	"The crate was opened!",
	"I was looking for that, thank you! Here, I'll give you this.",
	"You obtained SPIRIT!",
	"LAND OF RAIN: Where Everything Grows.",
	"MARKET: Find Anything You Need!",
	"Hello! How may I help you?",
	"Buy  ITEM",
	"Sell ITEM",
	"Not enough Tokens!",
	"Have a nice day!",
	"Trade XXXX Tokens?",
	"This item cannot be sold here.",
	"Confirm Trade?",
	"I see you have a XXXXXXXXXXXX Trade it for an XXXXXXXXXXXX",
	"Successfully applied cast!",
	"Cast cannot be applied here.",
	"An ARENA TICKET is needed to  enter.",
	"Oh, I see you have an ARENA TICKET!",
	"Continue Entry?",
	"Welcome! How are you?",
	"You received a XXXXXXXXXXXXXXXX",
	"Here, you should have this.",
	"ELEMENT",
	"TOKEN",
	"Goodbye!",
	"LAND OF MIST: A True One of a Kind.",
	"ARENA: Unlock Your True Potential.",
	"ATTN: Welcome to MOON PARK.",
	"ATTN: Entry is currently not permitted. Please come back later.",
	"A PARK TICKET is needed to enter.",
	"Oh, I see you have a PARK TICKET!",
	"ATTN: No more may be taken at this time. Have a nice day!",
	"MOON PARK: Truely Magical.",
	"LAND OF SKY: True Paradise.",
	"Come back once you have eight ELEMENTS.",
	"This item cannot be obtained more than once.",
	"Delete Element?",
	"HOLLOW CAVE",
	"What creates this space? %%122%%121%%122%%121",
	"LAND OF WIND: True greatness.",
	"MAINTENANCE OPERATIVE",
	"Is it time yet?",
	"MATERIAL cannot be placed.",
	"REMOVE MATERIAL",
	"REMOVE STACK",
	"REMOVE ALL",
	"SAVE MAP",
	"EXIT MAP",
	"Enter MAP",
	"Load MAP",
	"New MAP",
	"Remove MAP",
	"NEW MAP NAME:",
	"________________",
	"Map cannot be created at this time.",
	"Map successfully saved!",
	"XXXXXXXXXXXXXXXX",
	"MAP cannot be removed at this time.",
	"MAP successfully removed!",
	"New ITEM(s) received!",
	"New ELEMENT(s) received!",
	"ITEM cannot be used at this time.",
	"ELEMENT's SPIRIT has increased!",
	"Trade XXXXXXXXXXXXXXXXfor a XXXXXXXXXXXXXXXX?",
	"xXX",
	"BLUE CAVE: What a sight!",
	"CAST",
	"SPIRIT CAST",
	"FUME WAVE",
	"FLARE",
	"SURGE",
	"SHRED",
	"DIG",
	"RETURN",
	"DISPERSE",
	"ILLUMINATE",
	"SUSPENSION",
	"GLOOMY HAZE",
	"PURSUE",

	"STONEFALL",
	"ANCIENT WISDOM",
	"CASCADE",
	"SACRED SPORE",
	"TANGLE",
	"GROWTH",
	"DIFFUSION",
	"RESONANCE",
	"DEBUG RM",
	"ENIGMA SONG",
	"ETERNAL MIRAGE",
	"FLURRY",
	"SUNDER CAST",
	"DEBUG TP",

	"TORRENT",
	"TIDAL SURGE",
	"ANCIENT MIRAGE",
	"CYCLONE",
	"SONIC WAVE",
	"BERYL SONG",
	"RADIANT CREST",
	"THICKET",
	"LEAF DANCE",
	"PIN WHIRL",
	"WHIRL",
	"BELL SONG",

	"HIDDEN POWER",
	"DOWNRUSH",
	"CRYPTIC SYMBOL",
	"ROLL",

	"HARMONIC CAST",
	"RADIANT SURGE",
	"DREAM BARRIER",
	"ECHO",

	"MOON SURGE",
	"FRIGID GROVE",
	"MOON AURORA",
	"FOCUS STREAM",
	"FOCUS WIND",

	"HARMONIC TONE",
	"FOCUS RUSH",
	"STATIC AFFINITY",
	"SWIFT FOCUS",
	"SWIFT PULSE",
	"GENERATIVE PULSE",
	"GENERATIVE ESSENCE",
	"FONT GALE",
	"INVOKE FONT",
	"SECRET CHARGE",
	"DEEP ENVELOPE",
	"RIPTIDE",
	"DYNAMIC CURRENT",
	"SPIRIT ELEMENT",

	"TEMPEST MIST",
	"DEEP RESERVE",
	"ELUSIVE GALE",

	"DIVE",
	"BUBBLE",
};

void inc_textb(s_spritedata_a* t, int inc)
{
	if (t_bidx != t->size - 1)
	{
		if ((t_bidx + inc) > -inc + t->size)
			t_bidx = t->size - 1;
		else
			t_bidx += inc;
	}
}

s_spritedata_a create_SpriteDataArr(int capacity)
{
	s_spritedata_a buf;
	buf.arr = (s_spritedata**)malloc(capacity * sizeof(s_spritedata*));
	buf.size = 0;
	buf.capacity = capacity;
	return buf;
}

void free_SpriteDataArr(s_spritedata_a* array)
{
	free(array->arr);
}

char empty_SpriteDataArr(s_spritedata_a* array)
{
	return array->size == 0;
}

int get_size_SpriteDataArr(s_spritedata_a* array)
{
	return array->size;
}

int get_capacity_SpriteDataArr(s_spritedata_a* array)
{
	return array->capacity;
}

void resize_array_SpriteDataArr(s_spritedata_a* array, int new_capacity)
{
	s_spritedata** new_arr = (s_spritedata**)malloc(new_capacity * sizeof(s_spritedata*));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_SpriteDataArr(s_spritedata_a* array, s_spritedata** value)
{
	if (array->size == array->capacity)
		resize_array_SpriteDataArr(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}

#include <stdio.h>
#include "dglobal.h"

void push_back_sprdataa_str(s_spritedata_a* array, const char* str)
{
	s_spritedata* c;

	for (int i = 0; i < strlen(str); i++)
	{
		if (str[i] == '%' && str[i + 1] == '%')
		{
			(i += 2);
			int buf =
				+pow(10, 2) * (str[i + 0] - '0')
				+ pow(10, 1) * (str[i + 1] - '0')
				+ pow(10, 0) * (str[i + 2] - '0');
			(i += (3 - 1));

			s_spritedata* tmp = (s_spritedata*)&sprdata[buf + SPRITE_TEXT_OFFSET];

			push_back_SpriteDataArr(array, &tmp);
		}
		else
		{
			if (str[i] == 0) c = NULL;
			else if (str[i] == 1)c = &(sprdata[+(!!1) + 4 + 134 - 28 + SPRITE_TEXT_OFFSET]);
			else c = &(sprdata[chtotid(str[i]) + SPRITE_TEXT_OFFSET]);
			push_back_SpriteDataArr(array, &c);
		}
	}
}

s_spritedata* pop_back_SpriteDataArr(s_spritedata_a* array)
{
	if (empty_SpriteDataArr(array))
		exit(1);
	array->size--;
	if (array->size < array->capacity / 2)
		resize_array_SpriteDataArr(array, array->capacity / 2);

	return array->arr[array->size];
}

s_spritedata** get_element_SpriteDataArr(s_spritedata_a* array, int index)
{
	if (index < 0 || index >= array->size)
		exit(1);
	return &(array->arr[index]);
}

void set_element_SpriteDataArr(s_spritedata_a* array, int index, s_spritedata* value)
{
	if (index < 0 || index >= array->size)
		exit(1);
	array->arr[index] = value;
}

void set_text_txt(s_spritedata_a* s, s_spritedata_a* s1, int tbufl, int toffs, s_spritedata* tail)
{
#define MIN(a, b)	(((a) < (b)) ? (a) : (b))
#define MAX(a, b)	(((a) > (b)) ? (a) : (b))

	int m = MIN(tbufl, s1->size);

	int i;
	for (i = toffs; i < toffs + m; i++)
		s->arr[i]
		=
		(s1->arr[i - toffs]);

	s->arr[i] = tail;

	i++;

	for (; i <= toffs + tbufl - 1; i++)
		s->arr[i] =
		&sprdata[TEXT_SPACE_IDX];
}

void set_text_char(s_spritedata_a* t, s_spritedata* ch, int a, int b)
{
	for (int i = a
		; i != b
		; i++) t->arr[i] = ch
		;
}

void set_trade1_text_num(int num, s_spritedata_a* a, s_spritedata_a* b)
{
	for (int i = 0; i < 2; i++)
		gtexta[TEXT_D2NUM_IDX].arr[i + 1]
		= &sprdata[TEXT_0 + (num / (int)pow(10, (2 - 1) - i)) % 10];

	set_text_txt(&gtexta[TEXT_TRADE1_IDX]
		, &gtexta[TEXT_D2NUM_IDX]
		, gtexta[TEXT_D2NUM_IDX].size
		, 6
		, &sprdata[TEXT_SPACE_IDX]
	);

	set_text_txt(&gtexta[TEXT_TRADE1_IDX]
		, a
		, a->size
		, 6 + gtexta[TEXT_D2NUM_IDX].size + 1
		, &sprdata[TEXT_SPACE_IDX]
	);
	
	s_spritedata** aa = &gtexta[TEXT_TRADE1_IDX].arr[6 + gtexta[TEXT_D2NUM_IDX].size + a->size + 2];
	s_spritedata** bb = &gtexta[TEXT_TRADE1_IDX].arr[22];

	memmove(aa, bb, gtexta[TEXT_TRADE1_IDX].size - (bb - aa));

	set_text_txt(&gtexta[TEXT_TRADE1_IDX]
		, b
		, b->size
		, gtexta[TEXT_TRADE1_IDX].size - 1 - 16 - (bb - aa)
		, &sprdata[TEXT_QMRK_IDX]
	);

	set_text_char(&gtexta[TEXT_TRADE1_IDX], &sprdata[TEXT_SPACE_IDX], gtexta[TEXT_TRADE1_IDX].size - 1 - 16 - (bb - aa) + b->size + 1, gtexta[TEXT_TRADE1_IDX].size);
}