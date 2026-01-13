#pragma once
#include "utils.h"

#if defined(__x86_64__) || defined(_M_X64)
    #define DIESEL_ARCH_X86 64
#elif defined(__aarch64__) || defined(_M_ARM64)
    #define DIESEL_ARCH_ARM 64
#elif defined(__riscv) && (__riscv_xlen == 64)
    #define DIESEL_ARCH_RISCV 64
#endif



