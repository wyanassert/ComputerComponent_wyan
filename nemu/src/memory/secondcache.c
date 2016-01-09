#include "common.h"
#include "nemu.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

void init_secondcache()
{

}

void writesecondcache(hwaddr_t addr, size_t len, uint32_t data)
{

}
uint32_t readsecondcache(hwaddr_t addr, size_t len)
{
	return 0;
}