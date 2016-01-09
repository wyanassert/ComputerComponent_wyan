#include "common.h"
#include "nemu.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

extern SECOND_CACHE secondcache;

int secondgeneraterandom(int random)
{
	return random * random + 422 + random / 3;
}

void init_secondcache()
{
	secondcache.total = 0;
	secondcache.nothitnum = 0;
	int i, j;
	for(i = 0; i < 4096; i++)
	    for(j = 0; j < 16; j++)
	    {
	        secondcache.set[i].block[j].valid = 0;
	        secondcache.set[i].block[j].dirty = 0;
	        secondcache.set[i].block[j].addr = 0;
	        secondcache.set[i].block[j].value = 1;
	    }
}

void writesecondcache(hwaddr_t addr, size_t len, uint32_t data)
{
	static int random;
	int setnum = addr % 4096;
	bool ishit = false;
	int i = 0;
	for(i = 0; i < 16; i++)
		if(secondcache.set[setnum].block[i].valid && secondcache.set[setnum].block[i].addr == addr)
		{
			ishit = true;
			secondcache.set[setnum].block[i].dirty = true;
			secondcache.set[setnum].block[i].addr = addr;
			secondcache.set[setnum].block[i].value = data;
		}
	if(!ishit)
	{
		bool isfindempty = false;
		for(i = 0; i < 16; i++)
		{
			if(!secondcache.set[setnum].block[i].valid)
			{
				isfindempty = true;
				secondcache.set[setnum].block[i].dirty = false;
				secondcache.set[setnum].block[i].valid = true;
				secondcache.set[setnum].block[i].addr = addr;
				secondcache.set[setnum].block[i].value = data;
				break;
			}
		}
		if(!isfindempty)
		{
			//weed out here
			int weednum = secondgeneraterandom(random) % 16;
			if(secondcache.set[setnum].block[weednum].dirty)
				dram_write(secondcache.set[setnum].block[weednum].addr, len, secondcache.set[setnum].block[weednum].value);
			secondcache.set[setnum].block[weednum].addr = addr;
			secondcache.set[setnum].block[weednum].value = data;
		}
	}
	random += 7;
	if(random > 1543)
		random -= 1543;
	return ;
}
uint32_t readsecondcache(hwaddr_t addr, size_t len)
{
	secondcache.total ++;
	int setnum = addr % 128;
	bool ishit = false;
	uint32_t tmpresult;
	int i = 0;
	for(i = 0; i < 8; i++)
		if(secondcache.set[setnum].block[i].valid && secondcache.set[setnum].block[i].addr == addr)
		{
			ishit = true;
			tmpresult = secondcache.set[setnum].block[i].value;
			break ;
		}
	if(ishit)
	{
		return tmpresult;
	}
	else
	{
		secondcache.nothitnum ++;
		tmpresult = dram_read(addr, len);
		writesecondcache(addr, len, tmpresult);
		return tmpresult;
	}}