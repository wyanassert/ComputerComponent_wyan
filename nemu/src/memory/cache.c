#include "common.h"
#include "nemu.h"

void writesecondcache(hwaddr_t addr, size_t len, uint32_t data);
uint32_t readsecondcache(hwaddr_t addr, size_t len);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t dram_read(hwaddr_t, size_t);

extern CACHE cache;

int generaterandom(int random)
{
	return random * random + 422 + random / 3;
}

void init_cache()
{
	cache.total = 0;
	cache.nothitnum = 0;
	int i, j;
	for(i = 0; i < 128; i++)
	    for(j = 0; j < 8; j++)
	    {
	        cache.set[i].block[j].valid = 0;
	        cache.set[i].block[j].addr = 0;
	        cache.set[i].block[j].value = 0;
	    }
}

void writecache(hwaddr_t addr, size_t len, uint32_t data)
{
	dram_write(addr, len, data);

	static int random;
	int setnum = addr % 128;
	bool ishit = false;
	int i = 0;
	for(i = 0; i < 8; i++)
		if(cache.set[setnum].block[i].valid && cache.set[setnum].block[i].addr == addr)
		{
			ishit = true;
			cache.set[setnum].block[i].addr = addr;
			cache.set[setnum].block[i].value = dram_read(addr, 4);
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
				cache.set[setnum].block[i].value = dram_read(addr, 4);
				break;
			}
		}
		if(!isfindempty)
		{
			//weed out here
			int weednum = generaterandom(random) % 8;
			cache.set[setnum].block[weednum].addr = addr;
			cache.set[setnum].block[weednum].value = dram_read(addr, 4);
		}
	}
	writesecondcache(addr, len, data);
	
	if(addr == 0x8013b8)
		printf("write in 8013b8,dram(%x), cache(%x)\n", dram_read(addr, 4),readcache(addr, 4));

	random += 7;
	if(random > 1543)
		random -= 1543;
	return ;
}
uint32_t readcache(hwaddr_t addr, size_t len)
{
	cache.total ++;
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
		return tmpresult;
	}
	else
	{
		cache.nothitnum ++;
		tmpresult = readsecondcache(addr, len);
		writecache(addr, len, tmpresult);
		return tmpresult;
	}
}

