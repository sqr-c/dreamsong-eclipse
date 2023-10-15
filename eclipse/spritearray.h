#pragma once

#include "sprite.h"

typedef struct
{
	s_sprite* arr;
	int size;
	int capacity;
} s_sprite_a;

s_sprite_a create_sprarr(int capacity);
void free_sprarr(s_sprite_a* array);
void resize_array_sprarr(s_sprite_a* array, int new_capacity);
void push_back_sprarr(s_sprite_a* array, s_sprite* value);
s_sprite* get_element_sprarr(s_sprite_a* array, int index);
void set_element_sprarr(s_sprite_a* array, int index, s_sprite value);
void print_data(const char* data, int len);
void print_array(s_sprite_a* array);

typedef struct
{
	s_sprite_a* arr;
	int size;
	int capacity;
} s_sprite_aa;

s_sprite_aa create_spraa(int capacity);
void free_spraa(s_sprite_aa* array);
void resize_array_spraa(s_sprite_aa* array, int new_capacity);
void push_back_spraa(s_sprite_aa* array, s_sprite_a* value);
s_sprite_a* get_element_spraa(s_sprite_aa* array, int index);
void set_element_spraa(s_sprite_aa* array, int index, s_sprite_a* value);
void print_spraa(s_sprite_aa* array);

typedef struct
{
	s_spritedata** arr;
	int size;
	int capacity;
} s_spritedata_a;

s_spritedata_a create_sprdataa(int capacity);
void free_sprdata(s_spritedata_a* array);
void resize_sprdataa(s_spritedata_a* array, int new_capacity);
void push_back_sprdataa(s_spritedata_a* array, s_spritedata** value);
s_spritedata** get_element_sprdataa(s_spritedata_a* array, int index);