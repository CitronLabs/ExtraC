#include "pkg.h"


u32 __XC_Default_Hash(void* data_ptr, u64 size){

	char* data = data_ptr;
	len_t nblocks = size / 8;
	u64 hash = HASHMAP_HASH_INIT;
	for (len_t i = 0; i < nblocks; ++i)
	{
		hash ^=  (u64)data[0] << 0  | (u64)data[1] << 8  |
			 (u64)data[2] << 16 | (u64)data[3] << 24 |
			 (u64)data[4] << 32 | (u64)data[5] << 40 |
			 (u64)data[6] << 48 | (u64)data[7] << 56;
		hash *= 0xbf58476d1ce4e5b9;
		data += 8;
	}

	u64 last = size & 0xff;
	switch (size % 8)
	{
	case 7:
		last |= (u64)data[6] << 56; 
	case 6:
		last |= (u64)data[5] << 48; 
	case 5:
		last |= (u64)data[4] << 40; 
	case 4:
		last |= (u64)data[3] << 32; 
	case 3:
		last |= (u64)data[2] << 24; 
	case 2:
		last |= (u64)data[1] << 16; 
	case 1:
		last |= (u64)data[0] << 8;
		hash ^= last;
		hash *= 0xd6e8feb86659fd93;
	}

return (u32)(hash ^ hash >> 32);
}
