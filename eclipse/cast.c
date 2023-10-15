#include "cast.h"

c_cast_a create_CastA(int capacity)
{
	c_cast_a buf;
	buf.arr = (c_cast*)malloc(capacity * sizeof(c_cast));
	buf.size = 0;
	buf.capacity = capacity;
	return buf;
}

void free_CastA(c_cast_a* array)
{
	free(array->arr);
}

void resize_array_CastA(c_cast_a* array, int new_capacity)
{
	c_cast* new_arr = (c_cast*)malloc(new_capacity * sizeof(c_cast));
	for (int i = 0; i < array->size; i++)
		new_arr[i] = array->arr[i];

	free(array->arr);
	array->arr = new_arr;
	array->capacity = new_capacity;
}

void push_back_CastA(c_cast_a* array, c_cast* value)
{
	if (array->size == array->capacity)
		resize_array_CastA(array, array->capacity * 2);

	array->arr[array->size] = *value;
	array->size++;
}

inline char assign_cast(short castin, short* castout)
{
	*castout = castin;
}