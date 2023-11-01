#include "maploader.h"

#include <stdio.h>
#include <stdlib.h>

m_maploader_a create_mapload(int capacity)
{
	m_maploader_a buf;
	buf.arr = (m_maploader*)malloc(capacity * sizeof(m_maploader));
	buf.size = 0;
	buf.capacity = capacity;
	return buf;
}

void free_mapload(m_maploader_a* array)
{
	free(array->arr);
}

void free_mapload_mapgen(m_maploader_a* array)
{
	if (array->size > 0) free_mapload(array);
}

char empty_mapload(m_maploader_a* array)
{
	return array->size == 0;
}

int get_size_mapload(m_maploader_a* array)
{
	return array->size;
}

int get_capacity_mapload(m_maploader_a* array)
{
	return array->capacity;
}

void resize_array_mapload(m_maploader_a* array, int new_capacity)
{
	m_maploader* new_arr = (m_maploader*)malloc(new_capacity * sizeof(m_maploader));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_mapload(m_maploader_a* array, m_maploader* value)
{
	if (array->size == array->capacity)
		resize_array_mapload(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}

m_maploader pop_back_mapload(m_maploader_a* array)
{
	if (empty_mapload(array))
	{
		printf("Error: array is empty\n");
		exit(1);
	}
	array->size--;
	if (array->size < array->capacity / 2)
		resize_array_mapload(array, array->capacity / 2);

	return array->arr[array->size];
}

m_maploader* get_element_mapload(m_maploader_a* array, int index)
{
	if (index < 0 || index >= array->size)
	{
		printf("Error: index out of range\n");
		exit(1);
	}
	return &(array->arr[index]);
}

void set_element_mapload(m_maploader_a* array, int index, m_maploader value)
{
	if (index < 0 || index >= array->size)
	{
		printf("Error: index out of range\n");
		exit(1);
	}
	array->arr[index] = value;
}

m_mapseek_a create_mapseek(int capacity)
{
	m_mapseek_a buf;
	buf.arr = (m_mapseek*)malloc(capacity * sizeof(m_mapseek));
	buf.size = 0;
	buf.capacity = capacity;
	return buf;
}

void free_mapseek(m_mapseek_a* array)
{
	free(array->arr);
}

void resize_array_mapseek(m_mapseek_a* array, int new_capacity)
{
	m_mapseek* new_arr = (m_mapseek*)malloc(new_capacity * sizeof(m_mapseek));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_mapseek(m_mapseek_a* array, m_mapseek* value)
{
	if (array->size == array->capacity)
		resize_array_mapseek(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}