#pragma once

#include <stdint.h>

typedef struct
{
	int ix, iy;
	int ox, oy;
	int32_t seek;
} m_maploader;

typedef struct
{
	m_maploader* arr;
	int size;
	int capacity;
} m_maploader_a;

m_maploader_a create_mapload(int capacity);
void free_mapload(m_maploader_a* array);
void free_mapload_mapgen(m_maploader_a* array);
char empty_mapload(m_maploader_a* array);
int get_size_mapload(m_maploader_a* array);
int get_capacity_mapload(m_maploader_a* array);
void resize_array_mapload(m_maploader_a* array, int new_capacity);
void push_back_mapload(m_maploader_a* array, m_maploader* value);
m_maploader pop_back_mapload(m_maploader_a* array);
m_maploader* get_element_mapload(m_maploader_a* array, int index);
void set_element_mapload(m_maploader_a* array, int index, m_maploader value);

typedef struct
{
	m_maploader load;
	int32_t seekin;
} m_mapseek;

typedef struct
{
	m_mapseek* arr;
	int size;
	int capacity;
} m_mapseek_a;


m_mapseek_a create_mapseek(int capacity);
void free_mapseek(m_mapseek_a* array);
void resize_array_mapseek(m_mapseek_a* array, int new_capacity);
void push_back_mapseek(m_mapseek_a* array, m_mapseek* value);