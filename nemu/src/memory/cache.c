#include "common.h"
#include "nemu.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

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
	int setnum = addr % 128;
	bool ishit = false;
	int i = 0;
	for(i = 0; i < 8; i++)
		if(cache.set[setnum].block[i].valid && cache.set[setnum].block[i].addr == addr)
		{
			ishit = true;
			cache.set[setnum].block[i].addr = addr;
			cache.set[setnum].block[i].value = data & (~0u >> ((4 - len) << 3));
		}
	if(!ishit)
	{
		bool isfindempty = false;
		for(i = 0; i < 8; i++)
		{
			if(!cache.set[setnum].block[i].valid)
			{
				isfindempty = true;
				cache.set[setnum].block[i].valid = true;
				cache.set[setnum].block[i].addr = addr;
				cache.set[setnum].block[i].value = data & (~0u >> ((4 - len) << 3));
				break;
			}
		}
		if(!isfindempty)
		{
			//weed out here
		}
	}
	dram_write(addr, len, data);
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
			break ;
		}
	if(ishit)
	{
		return tmpresult & (~0u >> ((4 - len) << 3));
	}
	else
	{
		tmpresult = dram_read(addr, len) & (~0u >> ((4 - len) << 3));
		writecache(addr, len, tmpresult);
		return tmpresult;
	}
}