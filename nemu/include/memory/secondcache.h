#ifndef __SECONDCACHE_H__
#define __SECONDCACHE_H__

#include "common.h"

typedef struct
{
	bool valid;
	bool dirty;
	uint32_t addr;
	uint32_t value;
} SECOND_CACHE_BLOCK;

typedef struct
{
	CACHE_BLOCK block[8];/* data */
} SECOND_CACHE_SET;

typedef struct
{
	SECOND_CACHE_SET set[128];/* data */
	uint32_t total;
	uint32_t nothitnum;
} SECOND_CACHE;

SECOND_CACHE secondcache;

void init_secondcache();

void writesecondcache(hwaddr_t addr, size_t len, uint32_t data);
uint32_t readsecondcache(hwaddr_t addr, size_t len);

#endif