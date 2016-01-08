#ifndef __CACHE_H__
#define __CACHE_H__

#include "common.h"

typedef struct
{
	bool valid;
	uint32_t addr;
	uint32_t value;
} CACHE_BLOCK;

typedef struct
{
	CACHE_BLOCK block[8];/* data */
} CACHE_SET;

typedef struct
{
	CACHE_SET set[128];/* data */
} CACHE;

CACHE cache;

void init_cache();
bool iscachehit(lnaddr_t addr);

#endif