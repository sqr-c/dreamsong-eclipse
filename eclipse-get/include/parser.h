#pragma once

#include "path.h"

#include "../../eclipse/include/package.h"

typedef struct
{
	char mode;

	int size;
	void* data;
} get_eid_entry;

typedef enum
{
	GET_EID_MODE_PACKAGE,
} GET_EID_MODE;

void eid_write_ex(get_eid_entry* entry, const char* name, const char* mode);
void eid_parse_write(const char* name);

#define eid_write(entry, name)	eid_write_ex(entry, name, "wb")