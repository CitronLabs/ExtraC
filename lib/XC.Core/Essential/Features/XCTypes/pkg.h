#pragma once
#define __XC_FEATURE__
#include "../pkg.h"

#include "../Type/pkg.h"

#include "utils.h"

#define package std


//integer types
asXCType(u64)
asXCType(u32)
asXCType(u16)
asXCType(u8)
asXCType(i64)
asXCType(i32)
asXCType(i16)
asXCType(i8)

asXCType(word)
asXCType(byte)
asXCType(uword)
asXCType(ubyte)
asXCType(len_t)

//float types
asXCType(float)
asXCType(double)

//character types
asXCType(c8)
asXCType(c16)
asXCType(c32)
asXCType(rune)

//string types
asXCType(strc8)
asXCType(strc16)
asXCType(strc32)

//pointer types
asXCType(pntr)
asXCType(constpntr)
asXCType(pntrval)

//boolean types
asXCType(bool)

asXCType(devHandle);
asXCType(registerHandle);
asXCType(streamHandle);

asXCType(XC_Time);
asXCType(XC_Locale_Data);
asXCType(XC_Memory_Info);

typefrom(constpntr, TypeID)

#undef package
