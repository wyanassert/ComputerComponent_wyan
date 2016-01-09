#include "common.h"
#include "nemu.h"


uint32_t dram_read(hwaddr_t, size_t);
// void dram_write(hwaddr_t, size_t, uint32_t);

void writecache(hwaddr_t addr, size_t len, uint32_t data);
uint32_t readcache(hwaddr_t addr, size_t len);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	if(addr == 0x100873)
		printf("read: addr:%x, len:%d\n", addr, len);
	uint32_t result_dram = dram_read(addr, len) & (~0u >> ((4 - len) << 3)), result_cache = readcache(addr, len) & (~0u >> ((4 - len) << 3));
	if(result_cache!= result_dram)
		printf("read [error],addr:%x, len:%d, dram:%x, cache:%x\n", addr, len, result_dram, result_cache);
	//else
		//printf("read [success], addr:%x len:%d, dram:%x, cache:%x\n", addr, len, result_dram, result_cache);
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	return readcache(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	//dram_write(addr, len, data);
	//printf("write: addr:%x, len:%d, data:%x\n", addr, len, data);
	writecache(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}

