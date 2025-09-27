#pragma once
#include "./libc.h"

#include "features/macros/bitops.h"
#include "features/macros/esc_seq.h"
#include "features/macros/iteration.h"
#include "features/macros/types.h"

#define namespace(name, ...) struct {__VA_ARGS__} name;
#define shortName(dotPath, name) typeof(dotPath)* name = &dotPath;
#define using_namespace(name, localName) typeof(name)* localName = &name;

#define maxof(type) 	\
_Generic(((type){0}), 	\
i32:    INT32_MAX,  	\
u32:    INT32_MAX,  	\
i64:    INT64_MAX,  	\
u64:    INT64_MAX,  	\
i16:    INT16_MAX, 	\
u16:    INT16_MAX, 	\
i8:     INT8_MAX,	\
u8:     INT8_MAX,	\
float:  FLT_MAX, 	\
double: DBL_MAX,	\
void*:  UINTPTR_MAX  	\
)

#define minof(type) 	\
_Generic(((type){0}),  	\
i32:    INT32_MIN,  	\
u32:    ((u32)0),  	\
i64:    INT64_MIN,  	\
u64:    ((u64)0),  	\
i16:    INT16_MIN, 	\
u16:    ((u16)0),	\
i8:     INT8_MIN,	\
u8:     ((u8)0),	\
float:  FLT_MIN, 	\
double: DBL_MIN,	\
void*:  ((void*)0)  	\
)

#define DO_LATER(...) static_assert(false , __VA_ARGS__);



