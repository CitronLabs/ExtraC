#include "./config.c"

#define new_use(name, allocator, ...) \
	create(name, (allocator)->Alloc(sizeof(name))__VA_OPT__(, __VA_ARGS__))

#define new_use_with(name, constructor, allocator, ...) \
	create_with(name, constructor, (allocator)->Alloc(sizeof(name))__VA_OPT__(, __VA_ARGS__))

#define del_use(allocator, ...) \
	std.Type.alloc.destructWith((allocator)->Free, (pntr[]){__VA_ARGS__}, sizeof((pntr[]){__VA_ARGS__}) / sizeof(pntr))

#undef alloca 
#undef malloc 
#undef realloc
#undef free   
#undef memcpy 
#undef memset 
#undef memcmp 
#undef calloc 

#define alloca(size) 			__MEMORY_STACK_ALLOC(size)
#define malloc(size) 			std.Memory.Allocator.Interface.Alloc (std.Memory.getHeap(), size)
#define realloc(pntr, size) 		std.Memory.Allocator.Interface.Resize(std.Memory.getHeap(), pntr, size)
#define free(pntr) 			std.Memory.Allocator.Interface.Free  (std.Memory.getHeap(), pntr)
#define memcpy(dest, from,  size) 	std.Memory.copyTo(dest, from, size)
#define memset(dest, value, size) 	std.Memory.setTo(dest, value, size)
#define memcmp(a, b,  size) 		std.Memory.compare(a, b, size)
#define calloc(num, size) 		memset(malloc(num * size), 0, num * size)

#define KiB(n) ((len_t)(n) * 1024)
#define MiB(n) (KiB(n) * 1024)
#define GiB(n) (MiB(n) * 1024)
#define TiB(n) (GiB(n) * 1024)
#define PiB(n) (TiB(n) * 1024)


#define KB(n)  ((len_t)(n) * 1000)
#define MB(n)  (KB(n) * 1000)
#define GB(n)  (MB(n) * 1000)
#define TB(n)  (GB(n) * 1000)
#define PB(n)  (TB(n) * 1000)

#define MemPage(n) ((len_t)(n) * core.System.Mem.getInfo().pageSize)
