#pragma once
#include "./Mem.h"
#include "./Math.h"
#include "./Bits.h"

Interface(__XC_VEC,
    interface(__XC_VEC_MEM)  Mem;
    interface(__XC_VEC_MATH) Math;
    interface(__XC_VEC_BITS) Bits;
)
