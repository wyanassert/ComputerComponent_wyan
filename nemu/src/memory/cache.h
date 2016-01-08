#ifndef _CACHE_H
#define _CACHE_H

#include "common.h"

typedef struct cache_block{
	bool valid;
	uint32_t addr;
	uint32_t value;
} CACHE_BLOCK;

typedef struct cache_set
{
	CACHE_BLOCK block[8];/* data */
} CACHE_SET;

typedef struct cache
{
	CACHE_SET set[128];/* data */
} CACHE;

CACHE cache;

#endif