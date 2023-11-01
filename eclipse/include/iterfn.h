#pragma once

typedef struct
{
	void(**arr)(void);
	int size;
	int capacity;
} it_iterfn_a;

char iter_fn_contains(it_iterfn_a* arr, void(*v)());
it_iterfn_a create_IterFNA(int capacity);
void free_IterFNA(it_iterfn_a* array);
void resize_array_IterFNA(it_iterfn_a* array, int new_capacity);
void push_back_IterFNA(it_iterfn_a* array, void(*value)());
void pushf_IterFNA(it_iterfn_a* arr, void(*v)(void));
void rm_IterFNA(it_iterfn_a* arr, int idx);
void rmf_IterFNA(it_iterfn_a* arr, void(*v)(void));

void itfn_umrefresh_update();
void itfn_auto_ref();