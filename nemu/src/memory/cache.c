#include "common.h"
#include "nemu.h"

extern CACHE cache;

void init_cache()
{
	int i, j;
	for(i = 0; i < 128; i++)
	    for(j = 0; j < 8; j++)
	    {
	        cache.set[i].block[j].valid = 0;
	        cache.set[i].block[j].addr = 0;
	        cache.set[i].block[j].value = 1;
	    }
}

void writecache(hwaddr_t addr, size_t len, uint32_t data)
{
	return ;
}
uint32_t readcache(hwaddr_t addr, size_t len)
{
	int setnum = addr % 128;
	bool ishit = false;
	uint32_t tmpresult;
	int i = 0;
	for(i = 0; i < 8; i++)
		if(cache.set[setnum].block[i].valid && cache.set[setnum].block[i].addr == addr)
		{
			ishit = true;
			tmpresult = cache.set[setnum].block[i].value;
		}
	if(ishit)
	{
		return tmpresult;
	}
	else
	{

	}

	return 0;
}