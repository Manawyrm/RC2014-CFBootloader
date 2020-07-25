#pragma once

extern void* _romfunctions[32];

#define ROM_CF_INIT 0
#define ROM_CF_READ 1

/*static inline rom_cf_init()
{
	_romfunctions[ROM_CF_INIT]();
}*/

//cf_read(uint32_t sector, uint8_t* data)