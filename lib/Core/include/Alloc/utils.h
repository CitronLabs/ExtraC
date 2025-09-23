
/**
@def Buffer(type)
@brief readability macro for being explicit about what the buffer is intended to store
*/
	#define Buffer(type) inst(Buffer)


#if __HijackMalloc
#define memalign(align, size) 	CMalloc.AlignNew(generic c_malloc, align, size)
#define malloc(size) 		CMalloc.Allocator.New(generic c_malloc, size, NULL)
#define calloc(nmemb, size) 	CMalloc.Allocator.New(generic c_malloc, nmemb, &(u64){size}) 
#define realloc(data, size) 	CMalloc.Allocator.Resize(generic c_malloc, data, size, NULL) 
#define free(ptr) 		CMalloc.Allocator.Delete(generic c_malloc, ptr, NULL)
#endif
